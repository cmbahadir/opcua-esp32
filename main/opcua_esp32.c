#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sys/param.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include <time.h>
#include <unistd.h>
#include <lwip/sockets.h>
#include <esp_flash_encrypt.h>
#include "esp_netif.h"
#include "open62541.h"
#include "DHT22.h"
#include "model.h"
#include <esp_task_wdt.h>
#include <esp_sntp.h>

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  10

#define TAG "OPCUA_ESP32"
#define ENABLE_MDNS

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

UA_ServerConfig *config;
static UA_Boolean running = true;
static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

static bool obtain_time(void)
{
    initialize_sntp();

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry <= retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(esp_task_wdt_reset());
    }
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
    return timeinfo.tm_year > (2016 - 1900);
}

static UA_StatusCode
UA_ServerConfig_setUriName(UA_ServerConfig *uaServerConfig, const char *uri, const char *name) {
    // delete pre-initialized values
    UA_String_deleteMembers(&uaServerConfig->applicationDescription.applicationUri);
    UA_LocalizedText_deleteMembers(&uaServerConfig->applicationDescription.applicationName);

    uaServerConfig->applicationDescription.applicationUri = UA_String_fromChars(uri);
    uaServerConfig->applicationDescription.applicationName.locale = UA_STRING_NULL;
    uaServerConfig->applicationDescription.applicationName.text = UA_String_fromChars(name);

    for (size_t i = 0; i < uaServerConfig->endpointsSize; i++) {
        UA_String_deleteMembers(&uaServerConfig->endpoints[i].server.applicationUri);
        UA_LocalizedText_deleteMembers(
                &uaServerConfig->endpoints[i].server.applicationName);

        UA_String_copy(&uaServerConfig->applicationDescription.applicationUri,
                       &uaServerConfig->endpoints[i].server.applicationUri);

        UA_LocalizedText_copy(&uaServerConfig->applicationDescription.applicationName,
                              &uaServerConfig->endpoints[i].server.applicationName);
    }

    return UA_STATUSCODE_GOOD;
}

void opcua_task(void *pvParameter)
{
    UA_Int32 sendBufferSize = 16000;
    UA_Int32 recvBufferSize = 16000;

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    ESP_LOGI(TAG, "Fire up OPC UA Server.");
    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    // UA_ServerConfig_setDefault(config);
    UA_ServerConfig_setMinimalCustomBuffer(config, 4840, 0, sendBufferSize, recvBufferSize);

    const char* appUri = "open62541.esp32.server";

    #ifdef ENABLE_MDNS
    config->discovery.mdnsEnable = true;
    config->discovery.mdns.mdnsServerName = UA_String_fromChars(appUri);
    config->discovery.mdns.serverCapabilitiesSize = 2;
    UA_String *caps = (UA_String *) UA_Array_new(2, &UA_TYPES[UA_TYPES_STRING]);
    caps[0] = UA_String_fromChars("LDS");
    caps[1] = UA_String_fromChars("NA");
    config->discovery.mdns.serverCapabilities = caps;

    // We need to set the default IP address for mDNS since internally it's not able to detect it.
    tcpip_adapter_ip_info_t default_ip;
    esp_err_t ret = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &default_ip);
    if ((ESP_OK == ret) && (default_ip.ip.addr != INADDR_ANY)) {
        config->discovery.ipAddressListSize = 1;
        config->discovery.ipAddressList = (uint32_t *)UA_malloc(sizeof(uint32_t)*config->discovery.ipAddressListSize);
        memcpy(config->discovery.ipAddressList, &default_ip.ip.addr, sizeof(uint32_t));
    } else {
        ESP_LOGI(TAG, "Could not get default IP Address!");
    }
    #endif
    UA_ServerConfig_setUriName(config, appUri, "OPC UA Server - ESP32");

    /* Add Information Model Objects Here */
    addLEDMethod(server);
    addCurrentTemperatureDataSourceVariable(server);
    addRelay0ControlNode(server);
    addRelay1ControlNode(server);
    // addRelay2ControlNode(server);
    // addRelay3ControlNode(server);

    ESP_LOGI(TAG, "Heap Left : %d", xPortGetFreeHeapSize());
    UA_Server_run_startup(server);
    while (running)
    {
        UA_Server_run_iterate(server, false);
        ESP_ERROR_CHECK(esp_task_wdt_reset());
        taskYIELD();
    }
    UA_Server_run_shutdown(server);
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
}


static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);

        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        // Is time set? If not, tm_year will be (1970 - 1900).
        if (timeinfo.tm_year < (2016 - 1900)) {
            ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
            if (!obtain_time()) {
                ESP_LOGE(TAG, "Could not get time from NTP. Using default timestamp.");
            }
            // update 'now' variable with current time
            time(&now);
        }
        localtime_r(&now, &timeinfo);
        ESP_LOGI(TAG, "Current time: %d-%02d-%02d %02d:%02d:%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon+1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);


        xTaskCreatePinnedToCore(&opcua_task, "opcua_task", 24336, NULL, 1, NULL, 1);
        ESP_LOGI(TAG, "Heap size after OPC UA Task : %d", esp_get_free_heap_size());
    }
}

static void wifi_scan(void)
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();

    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS},
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    ESP_ERROR_CHECK(tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "ESP32_OPCUA_Server"));
    ESP_ERROR_CHECK(tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA));

    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
}

void app_main()
{
    //Encryption improvement w.r.t: 
    //Espressif security advisory concerning fault injection and secure boot (CVE-2019-15894)
    if (esp_flash_encryption_enabled()) 
    {
        esp_flash_write_protect_crypt_cnt();
    }
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    wifi_scan();
    ESP_ERROR_CHECK(esp_task_wdt_init(10, true));
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0)));
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1)));
}
