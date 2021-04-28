#include <stdio.h>
#include "esp_attr.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "servo.h"

/* Global definitions */
int16_t current_angle = 0;
bool done_rotating = false;

default_servo_t servo_config;
static uint32_t servo_per_degree_init(int32_t);
static void set_gpio_config(int);

static void set_gpio_config(int gpio_number)
{
    gpio_set_level(gpio_number, 0);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, gpio_number);
}

static uint32_t servo_per_degree_init(int32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}

int16_t get_current_angle(void)
{
    return current_angle;
}

static void mcpwm_example_servo_control(int32_t angle_input, int servo_pin)
{
    int32_t angle;
    bool direction = angle_input > current_angle;
    set_gpio_config(servo_pin);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    while (angle_input != current_angle)
    {
        angle = servo_per_degree_init(current_angle);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
        direction ? current_angle++ : current_angle--;
        vTaskDelay(2);
    }
}

static void servoTask(void *servo_config)
{
    done_rotating = false;
    default_servo_t *servo_config_to_task = (default_servo_t *)servo_config;
    mcpwm_example_servo_control(servo_config_to_task->angle, servo_config_to_task->servo_pin);
    done_rotating = true;
    vTaskDelete(NULL);
}

void create_servo_task(void *servo_config)
{
    BaseType_t xStatus;
    static unsigned int iteration = 1;

    xStatus = xTaskCreatePinnedToCore(
        servoTask,
        "Servo Task",
        12000,
        (void *)servo_config,
        0,
        NULL,
        1);

    if (xStatus == pdPASS)
    {
        iteration++;
    }
}
