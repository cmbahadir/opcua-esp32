#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#include <time.h>
#include <unistd.h>
#include <lwip/sockets.h>
#include "open62541.h"


#define BLINK_GPIO CONFIG_BLINK_GPIO
#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#define TAG "APP_MAIN"

UA_ServerConfig *config;

static UA_Boolean running = true;

void blink_task(void *pvParameter)
{
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while(1) {
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void opcua_task(void *pvParameter) {

    ESP_LOGI(TAG, "Fire up OPC UA Server.");
    config = UA_ServerConfig_new_customBuffer(4840, NULL, 8192, 8192);
    UA_ServerNetworkLayer nl = 
    UA_ServerNetworkLayerTCP(UA_ConnectionConfig_default, 4840, NULL);
    config->networkLayers = &nl;
    config->networkLayersSize = 1;

    UA_Server *server = UA_Server_new(config);

    // server started, Code will be blocked here till running is set to false.
    UA_Server_run(server, &running); 

    ESP_LOGI(TAG, "Now going to stop the server.");
    // UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    // UA_ServerConfig_delete(config);
    // return (int)retval;
    nl.deleteMembers(&nl);
    ESP_LOGI(TAG, "opcua_task going to return");
    // return 0;
    vTaskDelete(NULL);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
            ESP_LOGI(TAG, "Got IP: %s\n",
                    ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            // TODO: Here I create task that start a OPC UA Server

            xTaskCreate(&opcua_task, "opcua_task", 1024 * 70, NULL, 5, NULL);


            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        default:
            break;
    }
    return ESP_OK;
}


static void wifi_scan(void)
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = DEFAULT_SSID,
            .password = DEFAULT_PWD 
        }, 
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

}

void app_main()
{

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    wifi_scan();

    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
