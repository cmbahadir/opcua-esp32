#include <stdio.h>
#include <sys/param.h>
#include <unistd.h>
#include <lwip/sockets.h>
#include <esp_flash_encrypt.h>
#include "esp_netif.h"
#include <esp_task_wdt.h>
#include <esp_sntp.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "ethernet_connect.h"
#include "open62541.h"
#include "DHT22.h"
#include "model.h"
#include "custom_config.pb-c.h"

#include <wifi_provisioning/scheme_softap.h>
#include <wifi_provisioning/manager.h>

#define EXAMPLE_ESP_WIFI_SSID CONFIG_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS CONFIG_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY 10

#define TAG "OPCUA_ESP32"
#define SNTP_TAG "SNTP"
#define MEMORY_TAG "MEMORY"
#define ENABLE_MDNS 1
// Custom specific provisioning password
#define POP "abcd1234"

static bool obtain_time(void);
static void initialize_sntp(void);

UA_ServerConfig *config;

//provisioning
bool isprovisioned = false;
const int WIFI_PROVISIONED_EVENT = BIT0;
static EventGroupHandle_t wifi_event_group;

//custom_config
typedef struct
{
    char info[128];
    int version;
} custom_config_t;

typedef esp_err_t (*custom_prov_config_handler_t)(const custom_config_t *config);
custom_prov_config_handler_t custom_prov_handler;
esp_err_t custom_prov_config_data_handler(uint32_t session_id, const uint8_t *inbuf, ssize_t inlen,
                                          uint8_t **outbuf, ssize_t *outlen, void *priv_data);
struct
{
    int connection;
    int port;
    char *ip_address;
    char *netmask;
    char *gateway;
} serverConfig;

static UA_Boolean sntp_initialized = false;
static UA_Boolean running = true;
static UA_Boolean isServerCreated = false;
static struct tm timeinfo;
static time_t now = 0;

void initServerConfig(void)
{
    serverConfig.connection = 1; //0: Ethernet - 1: Wi-Fi
    serverConfig.port = 4840;
    serverConfig.ip_address = NULL;
    serverConfig.netmask = NULL;
    serverConfig.gateway = NULL;
}

static UA_StatusCode
UA_ServerConfig_setUriName(UA_ServerConfig *uaServerConfig, const char *uri, const char *name)
{
    // Delete pre-initialized values
    UA_String_deleteMembers(&uaServerConfig->applicationDescription.applicationUri);
    UA_LocalizedText_deleteMembers(&uaServerConfig->applicationDescription.applicationName);

    uaServerConfig->applicationDescription.applicationUri = UA_String_fromChars(uri);
    uaServerConfig->applicationDescription.applicationName.locale = UA_STRING_NULL;
    uaServerConfig->applicationDescription.applicationName.text = UA_String_fromChars(name);

    for (size_t i = 0; i < uaServerConfig->endpointsSize; i++)
    {
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

static void opcua_task(void *arg)
{

    UA_Int32 sendBufferSize = 32768;
    UA_Int32 recvBufferSize = 32768;

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

    ESP_LOGI(TAG, "Fire up OPC UA Server.");
    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setMinimalCustomBuffer(config, serverConfig.port, 0, sendBufferSize, recvBufferSize);

    const char *appUri = "open62541.esp32.server";
    UA_String hostName = UA_STRING("opcua-esp32");
#ifdef ENABLE_MDNS
    config->discovery.mdnsEnable = true;
    config->discovery.mdns.mdnsServerName = UA_String_fromChars(appUri);
    config->discovery.mdns.serverCapabilitiesSize = 2;
    UA_String *caps = (UA_String *)UA_Array_new(2, &UA_TYPES[UA_TYPES_STRING]);
    caps[0] = UA_String_fromChars("LDS");
    caps[1] = UA_String_fromChars("NA");
    config->discovery.mdns.serverCapabilities = caps;

    // Set the default IP address for mDNS since internally it's not able to detect it.
    tcpip_adapter_ip_info_t default_ip;
    esp_err_t ret = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &default_ip);
    if ((ESP_OK == ret) && (default_ip.ip.addr != INADDR_ANY))
    {
        config->discovery.ipAddressListSize = 1;
        config->discovery.ipAddressList = (uint32_t *)UA_malloc(sizeof(uint32_t) * config->discovery.ipAddressListSize);
        memcpy(config->discovery.ipAddressList, &default_ip.ip.addr, sizeof(uint32_t));
    }
    else
    {
        ESP_LOGI(TAG, "Could not get default IP Address!");
    }
#endif
    UA_ServerConfig_setUriName(config, appUri, "OPC_UA_Server_ESP32");
    UA_ServerConfig_setCustomHostname(config, hostName);

    // Add Information Model Objects Here
    addLEDMethod(server);
    addCurrentTemperatureDataSourceVariable(server);
    addRelay0ControlNode(server);
    addRelay1ControlNode(server);

    ESP_LOGI(TAG, "Heap Left : %d", xPortGetFreeHeapSize());
    UA_StatusCode retval = UA_Server_run_startup(server);
    if (retval == UA_STATUSCODE_GOOD)
    {
        while (running)
        {
            UA_Server_run_iterate(server, false);
            ESP_ERROR_CHECK(esp_task_wdt_reset());
            taskYIELD();
        }
        UA_Server_run_shutdown(server);
    }
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(SNTP_TAG, "Notification of a time synchronization event");
}

static void initialize_sntp(void)
{
    ESP_LOGI(SNTP_TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
    sntp_initialized = true;
}

static bool obtain_time(void)
{
    initialize_sntp();
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    memset(&timeinfo, 0, sizeof(struct tm));
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry <= retry_count)
    {
        ESP_LOGI(SNTP_TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(esp_task_wdt_reset());
    }
    time(&now);
    localtime_r(&now, &timeinfo);
    ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
    return timeinfo.tm_year > (2016 - 1900);
}

static void opc_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
{
    if (sntp_initialized != true)
    {
        if (timeinfo.tm_year < (2016 - 1900))
        {
            ESP_LOGI(SNTP_TAG, "Time is not set yet. Settting up network connection and getting time over NTP.");
            if (!obtain_time())
            {
                ESP_LOGE(SNTP_TAG, "Could not get time from NTP. Using default timestamp.");
            }
            time(&now);
        }
        localtime_r(&now, &timeinfo);
        ESP_LOGI(SNTP_TAG, "Current time: %d-%02d-%02d %02d:%02d:%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }

    if (!isServerCreated)
    {
        xTaskCreatePinnedToCore(opcua_task, "opcua_task", 24336, NULL, 10, NULL, 1);
        ESP_LOGI(MEMORY_TAG, "Heap size after OPC UA Task : %d", esp_get_free_heap_size());
        isServerCreated = true;
    }
}

// static void disconnect_handler(void *arg, esp_event_base_t event_base,
// int32_t event_id, void *event_data)
// {
// }

// Provisioning
static void get_device_service_name(char *service_name, size_t max)
{
    uint8_t eth_mac[6];
    const char *ssid_prefix = "PROV_";
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    snprintf(service_name, max, "%s%02X%02X%02X",
             ssid_prefix, eth_mac[3], eth_mac[4], eth_mac[5]);
}

static esp_err_t custom_config_handler(const custom_config_t *config)
{
    char *p;
    uint8_t i;
    char *req = (char *)config->info;
    for (i = 1, p = strtok(req, ","); p != NULL; p = strtok(NULL, ","), i++)
    {
        switch (i)
        {
        case 1:
            serverConfig.connection = atoi(p);
            break;
        case 2:
            if (atoi(p) <= 0 && atoi(p) >= 65536)
                break; // Use default port 4840 if the set value is beyond the limits.
            serverConfig.port = atoi(p);
            break;
        case 3:
            serverConfig.ip_address = strdup(p);
            break;
        case 4:
            serverConfig.netmask = strdup(p);
            break;
        case 5:
            serverConfig.gateway = strdup(p);
            break;
        default:
            break;
        }
    }
    // Break event loop if connection type is set as "Ethernet"
    if (serverConfig.connection == 0)
    {
        xEventGroupSetBits(wifi_event_group, WIFI_PROVISIONED_EVENT);
        isprovisioned = true;
    }
    return ESP_OK;
}

custom_prov_config_handler_t custom_prov_handler = custom_config_handler;

esp_err_t custom_prov_config_data_handler(uint32_t session_id, const uint8_t *inbuf, ssize_t inlen,
                                          uint8_t **outbuf, ssize_t *outlen, void *priv_data)
{
    CustomConfigRequest *req;
    CustomConfigResponse resp;
    custom_prov_config_handler_t app_handler_custom_config = (custom_prov_config_handler_t)priv_data;

    req = custom_config_request__unpack(NULL, inlen, inbuf);
    if (!req)
    {
        ESP_LOGE("custom_config", "Unable to unpack config data");
        return ESP_ERR_INVALID_ARG;
    }

    custom_config_response__init(&resp);
    resp.status = CUSTOM_CONFIG_STATUS__ConfigFail;

    if (app_handler_custom_config)
    {
        custom_config_t config;
        strlcpy(config.info, req->info, sizeof(config.info));
        config.version = req->version;
        esp_err_t err = app_handler_custom_config(&config);
        resp.status = (err == ESP_OK) ? CUSTOM_CONFIG_STATUS__ConfigSuccess : CUSTOM_CONFIG_STATUS__ConfigFail;
    }
    custom_config_request__free_unpacked(req, NULL);

    resp.dummy = 47; // Set a non zero value of dummy

    *outlen = custom_config_response__get_packed_size(&resp);
    if (*outlen <= 0)
    {
        ESP_LOGE(TAG, "Invalid encoding for response");
        return ESP_FAIL;
    }

    *outbuf = (uint8_t *)malloc(*outlen);
    if (*outbuf == NULL)
    {
        ESP_LOGE(TAG, "System out of memory");
        return ESP_ERR_NO_MEM;
    }

    custom_config_response__pack(&resp, *outbuf);
    return ESP_OK;
}

static void provisioning_event_handler(void *arg, esp_event_base_t event_base,
                                       int event_id, void *event_data)
{
    if (event_base == WIFI_PROV_EVENT)
    {
        switch (event_id)
        {
        case WIFI_PROV_START:
            ESP_LOGI(TAG, "Provisioning started");
            break;
        case WIFI_PROV_CRED_RECV:
        {
            wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
            ESP_LOGI(TAG, "Received Wi-Fi credentials"
                          "\n\tSSID     : %s\n\tPassword : %s",
                     (const char *)wifi_sta_cfg->ssid,
                     (const char *)wifi_sta_cfg->password);
            break;
        }
        case WIFI_PROV_CRED_FAIL:
        {
            wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
            ESP_LOGE(TAG, "Provisioning failed!\n\tReason : %s"
                          "\n\tPlease reset to factory and retry provisioning",
                     (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Wi-Fi station authentication failed" : "Wi-Fi access-point not found");
            break;
        }
        case WIFI_PROV_CRED_SUCCESS:
            ESP_LOGI(TAG, "Provisioning successful");
            xEventGroupSetBits(wifi_event_group, WIFI_PROVISIONED_EVENT);
            isprovisioned = true;
            break;
        case WIFI_PROV_END:
            // De-initialize manager once provisioning is finished
            wifi_prov_mgr_deinit();
            break;
        default:
            break;
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
        esp_wifi_connect();
    }
}

static void provision(void)
{
    char service_name[12];
    const char *pop = POP;
    const char *service_key = NULL;

    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &provisioning_event_handler, NULL));

    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_softap,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE};
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));
    ESP_LOGI(TAG, "Starting provisioning");
    
    get_device_service_name(service_name, sizeof(service_name));
    wifi_prov_security_t security = WIFI_PROV_SECURITY_1;
    wifi_prov_mgr_endpoint_create("custom-config");

    ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, service_key));
    wifi_prov_mgr_endpoint_register("custom-config", custom_prov_config_data_handler, (void *)custom_prov_handler);
    xEventGroupWaitBits(wifi_event_group, WIFI_PROVISIONED_EVENT, false, true, portMAX_DELAY);
}

void app_main(void)
{
    // Workaround for CVE-2019-15894
    spi_flash_init();
    if (esp_flash_encryption_enabled())
    {
        esp_flash_write_protect_crypt_cnt();
    }

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    initServerConfig();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_task_wdt_init(10, true));
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0)));
    ESP_ERROR_CHECK(esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(1)));

    // Initialize Wi-Fi including netif with default config
    esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_netif_ip_info_t ipInfo = { .ip.addr = 0, .gw.addr = 0, .netmask.addr = 0};
    if (isprovisioned == false)
    {
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, WIFI_PROV_END, &opc_event_handler, NULL));
        provision();
    }

    if (serverConfig.connection == 0)
    {
        wifi_prov_mgr_deinit();
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &opc_event_handler, NULL));
        if (serverConfig.ip_address != NULL && serverConfig.gateway != NULL && serverConfig.netmask != NULL)
        {
            ipInfo.ip.addr = esp_ip4addr_aton(serverConfig.ip_address);
            ipInfo.gw.addr = esp_ip4addr_aton(serverConfig.gateway);
            ipInfo.netmask.addr = esp_ip4addr_aton(serverConfig.netmask);
        }
        ESP_ERROR_CHECK(ethernet_connect(ipInfo));
    }
    else 
    {
        ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &provisioning_event_handler, NULL));
        // ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &provisioning_event_handler, NULL));
        ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &opc_event_handler, NULL));
    }
}
