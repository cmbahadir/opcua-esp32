#include "driver/gpio.h"
#include "open62541.h"
#include "model.h"
#include "servo.h"

/* Servo */

UA_StatusCode
readServo0State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue)
{
    UA_Double servo0_angle = 0;
    if (done_rotating){
        servo0_angle = (UA_Double) get_current_angle();
    }
    UA_Variant_setScalarCopy(&dataValue->value, &servo0_angle,
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
    UA_Double *angle_input = (UA_Double *)data->value.data;
    UA_StatusCode retval = UA_Variant_setScalarCopy(&value, angle_input,
                                                    &UA_TYPES[UA_TYPES_DOUBLE]);
    default_servo_t *servo_input = (default_servo_t*)pvPortMalloc(sizeof(default_servo_t));
    servo_input->angle = (int32_t) *angle_input;
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
    servo0.read = readServo0State;
    servo0.write = setServo0State;
    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
                                        parentReferenceNodeId, currentName,
                                        variableTypeNodeId, attr,
                                        servo0, NULL, NULL);
}
