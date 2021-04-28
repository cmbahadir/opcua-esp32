#include <stdio.h>
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#define SERVO_MIN_PULSEWIDTH 1000 
#define SERVO_MAX_PULSEWIDTH 2000 
#define SERVO_MAX_DEGREE 90       

typedef struct {
    int32_t angle;
    int servo_pin;
} default_servo_t;

extern int16_t current_angle;
extern bool done_rotating;
int16_t get_current_angle(void);

void create_servo_task(void*);
