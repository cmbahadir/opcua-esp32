#include <stdio.h>
#include "esp_attr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

#include "servo.h"

static void mcpwm_example_gpio_initialize(void);
static uint32_t servo_per_degree_init(uint32_t degree_of_rotation);

static void mcpwm_example_gpio_initialize(void)
{
    gpio_set_level(GPIO_SERVO_PWM, 0);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_SERVO_PWM); //Set GPIO 18 as PWM0A, to which servo is connected
}


static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}


void mcpwm_example_servo_control(uint32_t max_angle)
{
    uint32_t angle, count;
    mcpwm_example_gpio_initialize();

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50; //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;     //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;     //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
    for (count = 0; count < max_angle; count++)
    {
        angle = servo_per_degree_init(count);
        printf("Hop!\n");
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
        vTaskDelay(10); //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
    }
}
