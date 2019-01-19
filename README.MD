# OPC UA Server on ESP32

It is aimed to run OPC UA server on ESP32 board (xtensa toolchain). There is a node with below callback function, it blinks the on board led with given number of times.

    static UA_StatusCode
    ledProcessCallBack(UA_Server *server,
                            const UA_NodeId *sessionId, void *sessionHandle,
                            const UA_NodeId *methodId, void *methodContext,
                            const UA_NodeId *objectId, void *objectContext,
                            size_t inputSize, const UA_Variant *input,
                            size_t outputSize, UA_Variant *output) {
        UA_Int32 i = 0;
        UA_Int32 *inputVal = (UA_Int32*)input->data;
        UA_String tmp = UA_STRING_ALLOC("Data Received ");
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
        UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_deleteMembers(&tmp);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Led bink called.");
        return UA_STATUSCODE_GOOD;
    }

## Getting Started

It is required to have ESP-IDF (https://github.com/espressif/esp-idf) and this repo can be directly cloned under examples. Open62541 stack functionaliy is provided by open62541.c and open62541.h files. If you want to dive into OPC UA part, it is advised to check Open62541 project.

Also, if you want to build open62541.c/.h/.lib from source, beware of the changes on https://github.com/cmbahadir/opcua-esp32/blob/master/components/open62541lib/README.md in order to be xtensa-toolchain compatible.

- I haven't analyzed the required modifications on open62541.c/.h  files, these are just a result of trial and error process, so an explanatory contribution would be great. :)

### Prerequisites

- ESP-IDF : https://docs.espressif.com/projects/esp-idf/en/latest/get-started/
- Open62541 (Optional) : https://github.com/open62541/open62541

## License

This project is licensed under the Mozilla Public License Version 2.0 which is inherited from Open62541 project - see the [LICENSE.md](LICENSE.md) file for details

### Caution
This code is written for experimental purposes.I do NOT suggest to use it on a real world application.Also ı do not claim any reponsibility for the consequences.
