#include "open62541.h"

#pragma once

/* GPIO Numbers */
// #define BLINK_GPIO 2
#define DHT22_GPIO 4
//RELAYS
#define RELAY_0_GPIO 32
#define RELAY_1_GPIO 33

//SERVOS
#define SERVO_0_PWM GPIO_NUM_14
#define SERVO_1_PWM GPIO_NUM_19
#define SERVO_2_PWM GPIO_NUM_20
#define SERVO_3_PWM GPIO_NUM_21
#define SERVO_4_PWM GPIO_NUM_22
#define SERVO_5_PWM GPIO_NUM_23
#define SERVO_6_PWM GPIO_NUM_24
#define SERVO_7_PWM GPIO_NUM_25

/* Servo 0 */
UA_StatusCode
readServo0State(UA_Server *server,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext,
                UA_Boolean sourceTimeStamp, const UA_NumericRange *range,
                UA_DataValue *dataValue);

UA_StatusCode
setServo0State(UA_Server *server,
                  const UA_NodeId *sessionId, void *sessionContext,
                  const UA_NodeId *nodeId, void *nodeContext,
                 const UA_NumericRange *range, const UA_DataValue *data);

void
addServo0ControlNode(UA_Server *server);