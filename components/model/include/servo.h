#include <stdio.h>
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH 2000 //Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE 90       //Maximum angle in degree upto which servo can rotate
#define GPIO_SERVO_PWM 18

typedef struct {
    int32_t angle;
    int servo_pin;
} default_servo_t;

void create_servo_task(void*);
