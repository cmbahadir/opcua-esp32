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
#include "tcpip_adapter.h"
#include "open62541.h"
#include "DHT22.h"
#include "myNodeSet.h"


#define DEFAULT_SSID CONFIG_WIFI_SSID
#define DEFAULT_PWD CONFIG_WIFI_PASSWORD

#define TAG "APP_MAIN"
#define BLINK_GPIO 2

UA_ServerConfig *config;
float temperature;

static UA_Boolean running = true;

static UA_StatusCode
ledProcessCallBack(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output);

static void
addLEDMethod(UA_Server *server);

static void
addTemperatureNode(UA_Server *server);

void sensor_task(void *pvParameter) {
    //TODO: For now it only reads temperature once in parallel with opcua_task creation. Change this behaviour and make temperature dynamic.
    temperature = ReadTemperature(4);
    ESP_LOGI("Sensor_Task", "Temperature read from the sensor.");
    vTaskDelete(NULL);
}

void opcua_task(void *pvParameter) {
    ESP_LOGI(TAG, "Fire up OPC UA Server.");
    //config = UA_ServerConfig_new_customBuffer(4840, NULL, 8192, 8192);
    config = UA_ServerConfig_new_default();

    //Set the connection config
    UA_ConnectionConfig connectionConfig;
    connectionConfig.recvBufferSize = 16384;
    connectionConfig.sendBufferSize = 16384;

    UA_ServerNetworkLayer nl = UA_ServerNetworkLayerTCP(connectionConfig, 4840, NULL);

    //Set Discovery URL
    UA_String esp32url = UA_String_fromChars("opc.tcp://192.168.0.100:4840/");
    config->networkLayers = &nl;
    config->networkLayersSize = 1;
    config->networkLayers[0].discoveryUrl = UA_STRING("opc.tcp://espressif:4840");

    config->applicationDescription.discoveryUrls = &esp32url;
    config->applicationDescription.discoveryUrlsSize = 2;
    config->applicationDescription.applicationUri = UA_STRING("urn:SIMATIC.S7-1500.OPC-UA.Application:Mete");
    config->applicationDescription.applicationName = UA_LOCALIZEDTEXT("en-US","ESP32Server");
    config->applicationDescription.applicationType = UA_APPLICATIONTYPE_SERVER;
    //config->applicationDescription.gatewayServerUri = UA_STRING("192.168.0.1");
    UA_ServerConfig_set_customHostname(config, UA_STRING("espressif"));
    UA_Server *server = UA_Server_new(config);

    addLEDMethod(server);
    addTemperatureNode(server);
    // UA_Server_run(serer, &running);
    
    UA_Server_run_startup(server);
    UA_Boolean waitInternal = false;
    while(running){
        UA_UInt16 timeout = UA_Server_run_iterate(server, waitInternal);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = timeout * 1000;
        select(0,NULL,NULL,NULL,&tv);
    }

    ESP_LOGI(TAG, "Now going to stop the server.");
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    nl.deleteMembers(&nl);
    ESP_LOGI("OPC_TASK", "opcua_task going to return");
    vTaskDelete(NULL);
}

static UA_StatusCode
ledProcessCallBack(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
	UA_Int32 i = 0;
    UA_Int32 *inputVal = (UA_Int32*)input->data;
    UA_String tmp = UA_STRING_ALLOC("Data Received");
    if(*inputVal > 0) {
        tmp.data = (UA_Byte *)UA_realloc(tmp.data, tmp.length);
		while(i<*inputVal +1)
		{
			gpio_pad_select_gpio(BLINK_GPIO);
			gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
			gpio_set_level(BLINK_GPIO, 1);
			vTaskDelay(500 / portTICK_PERIOD_MS);
			gpio_set_level(BLINK_GPIO, 0);
			vTaskDelay(500 / portTICK_PERIOD_MS);
			i++;
		}
    }
    //TODO: Fix here to read temperature.
    temperature = ReadTemperature(4);
    UA_Variant_setScalarCopy(output, &temperature, &UA_TYPES[UA_TYPES_FLOAT]);
    UA_String_deleteMembers(&tmp);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Led bink called.");
    return UA_STATUSCODE_GOOD;
}

static void
addTemperatureNode(UA_Server *server) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    //TODO: Temperature value should be read with a cycle and parsed into variable attr value.
    UA_Int32 ambientTemperature = temperature;//ReadTemperature(4); 
    UA_Variant_setScalar(&attr.value, &ambientTemperature, &UA_TYPES[UA_TYPES_INT32]);

    attr.description = UA_LOCALIZEDTEXT("en-US", "Ambient Temperature in C");
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Ambient Temperature in C");
    attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId temperatureNodeId = UA_NODEID_STRING(1,"Temperature");
    UA_QualifiedName temperatureNodeName = UA_QUALIFIEDNAME(1,"Temperature Value");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);

    UA_Server_addVariableNode(server, temperatureNodeId, parentNodeId,
                              parentReferenceNodeId, temperatureNodeName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr, NULL, NULL);
}

static void
addLEDMethod(UA_Server *server) {
    //if (simple(server) != UA_STATUSCODE_GOOD) {
    // if (myNodeSet(server) != UA_STATUSCODE_GOOD) {
    //     UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Could not add the example nodeset. "
    //         "Check previous output for any error.");
    // } else {
        UA_NodeId createdNodeId;
        UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;

        object_attr.description = UA_LOCALIZEDTEXT("en-US", "A pump!");
        object_attr.displayName = UA_LOCALIZEDTEXT("en-US", "Pump1");

        // we assume that the myNS nodeset was added in namespace 2.
        // You should always use UA_Server_addNamespace to check what the
        // namespace index is for a given namespace URI. UA_Server_addNamespace
        // will just return the index if it is already added.
        UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, "Pump1"),
                                UA_NODEID_NUMERIC(2, 1002),
                                object_attr, NULL, &createdNodeId);

        UA_Argument inputArgument;
        UA_Argument_init(&inputArgument);
        inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Number of times to blink LED!");
        inputArgument.name = UA_STRING("Blink Count");
        inputArgument.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
        inputArgument.valueRank = -1; /* scalar */

        UA_Argument outputArgument;
        UA_Argument_init(&outputArgument);
        outputArgument.description = UA_LOCALIZEDTEXT("en-US", "Temperature Read");
        outputArgument.name = UA_STRING("MyOutput");
        //outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        outputArgument.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
        outputArgument.valueRank = -1; /* scalar */

        UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
        helloAttr.description = UA_LOCALIZEDTEXT("en-US","Enter the number of times you want LED to blin!");
        helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","Blink");
        helloAttr.executable = true;
        helloAttr.userExecutable = true;
        UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62541),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                                UA_QUALIFIEDNAME(1, "Blink"),
                                helloAttr, &ledProcessCallBack,
                                1, &inputArgument, 1, &outputArgument, NULL, &createdNodeId);
    // }
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
            xTaskCreate(&sensor_task, "sensor_task", 1024 * 8, NULL, 1, NULL);
            vTaskDelay(500 / portTICK_PERIOD_MS);
            xTaskCreate(&opcua_task, "opcua_task", 1024 * 8, NULL, 1, NULL);
            ESP_LOGI(TAG, "RAM left %d", esp_get_free_heap_size());
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
    tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA,"espressif");
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
    //xTaskCreate(&opcua_task, "opcua_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}

