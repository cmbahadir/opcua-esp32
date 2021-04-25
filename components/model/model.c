#include "driver/gpio.h"
#include "open62541.h"
#include "model.h"
#include "DHT22.h"
#include "servo.h"

static void
configureGPIO();

/* GPIO Configuration */
static void
configureGPIO(void)
{
    gpio_set_direction(RELAY_0_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(RELAY_1_GPIO, GPIO_MODE_INPUT_OUTPUT);
}

/* Temperature */

UA_StatusCode
readCurrentTemperature(UA_Server *server,
                       const UA_NodeId *sessionId, void *sessionContext,
                       const UA_NodeId *nodeId, void *nodeContext,
                       UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                       UA_DataValue *dataValue)
{
    UA_Float temperature = ReadTemperature(DHT22_GPIO);
    UA_Variant_setScalarCopy(&dataValue->value, &temperature,
                             &UA_TYPES[UA_TYPES_FLOAT]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

void addCurrentTemperatureDataSourceVariable(UA_Server *server)
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Temperature");
    attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "temperature");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Ambient Temperature");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource timeDataSource;
    timeDataSource.read = readCurrentTemperature;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        timeDataSource, NULL, NULL);
}

/* Servo */

UA_StatusCode
readServo0State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue)
{
    UA_Double servo0_State = 0;
    UA_Variant_setScalarCopy(&dataValue->value, &servo0_State,
                             &UA_TYPES[UA_TYPES_DOUBLE]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setServo0State(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data)
{
    UA_Variant value;
    UA_Double *max_angle = (UA_Double *)data->value.data;
    UA_StatusCode retval = UA_Variant_setScalarCopy(&value, max_angle,
                                                    &UA_TYPES[UA_TYPES_DOUBLE]);
    default_servo_t *servo_input = (default_servo_t*)pvPortMalloc(sizeof(default_servo_t));
    servo_input->angle = (int32_t) *max_angle;
    servo_input->servo_pin = GPIO_NUM_14;
    create_servo_task(servo_input);
    // This has to be called somewhere, but where.
    // vPortFree(servo_input);
    return retval;
}

void addServo0ControlNode(UA_Server *server)
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Servo0");
    attr.dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Servo_0");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Servo_0");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource servo0;
    //Configure GPIOs just before adding relay 0 - not a good practice.
    configureGPIO();
    servo0.read = readServo0State;
    servo0.write = setServo0State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        servo0, NULL, NULL);
}

/* Relay 0 */

UA_StatusCode
readRelay0State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue)
{
    UA_Boolean relay0_State = gpio_get_level(RELAY_0_GPIO);
    UA_Variant_setScalarCopy(&dataValue->value, &relay0_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRelay0State(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data)
{
    UA_Boolean currentState = gpio_get_level(RELAY_0_GPIO);
    int level = currentState == true ? 0 : 1;
    gpio_set_level(RELAY_0_GPIO, level);
    UA_Boolean relay0_state_after_write = gpio_get_level(RELAY_0_GPIO);
    UA_StatusCode status = relay0_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

void addRelay0ControlNode(UA_Server *server)
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Relay0");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Relay number 0.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Relay number 0.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource relay0;
    //Configure GPIOs just before adding relay 0 - not a good practice.
    configureGPIO();
    relay0.read = readRelay0State;
    relay0.write = setRelay0State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        relay0, NULL, NULL);
}

/* Relay 1 */

UA_StatusCode
readRelay1State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue)
{
    UA_Boolean relay1_State = gpio_get_level(RELAY_1_GPIO);
    UA_Variant_setScalarCopy(&dataValue->value, &relay1_State,
                             &UA_TYPES[UA_TYPES_BOOLEAN]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
setRelay1State(UA_Server *server,
               const UA_NodeId *sessionId, void *sessionContext,
               const UA_NodeId *nodeId, void *nodeContext,
               const UA_NumericRange *range, const UA_DataValue *data)
{
    UA_Boolean currentState = gpio_get_level(RELAY_1_GPIO);
    int level = currentState == true ? 0 : 1;
    gpio_set_level(RELAY_1_GPIO, level);
    UA_Boolean relay1_state_after_write = gpio_get_level(RELAY_1_GPIO);
    UA_StatusCode status = relay1_state_after_write == level ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADINTERNALERROR;
    return status;
}

void addRelay1ControlNode(UA_Server *server)
{
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "Relay1");
    attr.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;

    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "Control Relay number 1.");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "Control Relay number 1.");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);

    UA_DataSource relay1;
    relay1.read = readRelay1State;
    relay1.write = setRelay1State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        relay1, NULL, NULL);
}