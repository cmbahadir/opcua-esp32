#include <stdio.h>
#include "esp_attr.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "servo.h"

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

static void mcpwm_example_servo_control(int32_t max_angle, int servo_pin)
{
    int32_t angle, count;
    set_gpio_config(servo_pin);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50; //frequency = 50Hz
    pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
    for (count = 0; count < max_angle; count++)
    {
        angle = servo_per_degree_init(count);
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
        vTaskDelay(5); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    }
}

static void servoTask(void *servo_config)
{  
    default_servo_t* servo_config_to_task = (default_servo_t*)servo_config;   
    mcpwm_example_servo_control(servo_config_to_task->angle, servo_config_to_task->servo_pin);
    vTaskDelete(NULL); 
}

void create_servo_task(void *servo_config)
{                                      
    BaseType_t xStatus;                
    static unsigned int iteration = 1; 

    xStatus = xTaskCreatePinnedToCore(
        servoTask,              /* Function to implement the task */
        "Servo Task",           /* Name of the task */
        1000,                   /* Stack size in words */
        (void *)servo_config,   /* Task input parameter */
        0,                      /* Priority of the task */
        NULL,                   /* Task handle. */
        1);

    if (xStatus == pdPASS)
    { 
        ESP_LOGI("SERVO", "Task triggered...");
        iteration++;
    }
    else
    { 
        ESP_LOGI("SERVO", "Stack full for Servo Task...");
    }
}
