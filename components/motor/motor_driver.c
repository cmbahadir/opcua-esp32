#include "motor_driver.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "sdkconfig.h"

#define MOTOR_PWM_RESOLUTION LEDC_TIMER_8_BIT
#define MOTOR_PWM_MAX_DUTY   255

static int8_t s_driveSpeed = 0;
static int8_t s_steeringSpeed = 0;
static int64_t s_lastCommandUs = 0;

static void
set_motor_output(int8_t speed_percent, ledc_channel_t pwmChannel,
                 gpio_num_t forwardGpio, gpio_num_t reverseGpio) {
    if (speed_percent > 0) {
        gpio_set_level(forwardGpio, 1);
        gpio_set_level(reverseGpio, 0);
    } else if (speed_percent < 0) {
        gpio_set_level(forwardGpio, 0);
        gpio_set_level(reverseGpio, 1);
    } else {
        gpio_set_level(forwardGpio, 0);
        gpio_set_level(reverseGpio, 0);
    }

    int8_t magnitude = speed_percent < 0 ? (int8_t)-speed_percent : speed_percent;
    uint32_t duty = ((uint32_t)magnitude * MOTOR_PWM_MAX_DUTY) / 100;
    ledc_set_duty(LEDC_LOW_SPEED_MODE, pwmChannel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, pwmChannel);
}

void
motor_driver_init(void) {
    gpio_set_direction(CONFIG_MOTOR_DRIVE_IN1_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_MOTOR_DRIVE_IN2_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_MOTOR_STEERING_IN3_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_MOTOR_STEERING_IN4_GPIO, GPIO_MODE_OUTPUT);

    ledc_timer_config_t pwmTimer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = MOTOR_PWM_RESOLUTION,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = CONFIG_MOTOR_PWM_FREQUENCY_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&pwmTimer);

    ledc_channel_config_t driveChannel = {
        .gpio_num = CONFIG_MOTOR_DRIVE_ENA_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&driveChannel);

    ledc_channel_config_t steeringChannel = {
        .gpio_num = CONFIG_MOTOR_STEERING_ENB_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&steeringChannel);

    set_motor_output(0, LEDC_CHANNEL_0, CONFIG_MOTOR_DRIVE_IN1_GPIO, CONFIG_MOTOR_DRIVE_IN2_GPIO);
    set_motor_output(0, LEDC_CHANNEL_1, CONFIG_MOTOR_STEERING_IN3_GPIO, CONFIG_MOTOR_STEERING_IN4_GPIO);
    s_lastCommandUs = esp_timer_get_time();
}

esp_err_t
motor_driver_set_drive_speed(int8_t speed_percent) {
    if (speed_percent < -100 || speed_percent > 100) {
        return ESP_ERR_INVALID_ARG;
    }
    set_motor_output(speed_percent, LEDC_CHANNEL_0, CONFIG_MOTOR_DRIVE_IN1_GPIO, CONFIG_MOTOR_DRIVE_IN2_GPIO);
    s_driveSpeed = speed_percent;
    s_lastCommandUs = esp_timer_get_time();
    return ESP_OK;
}

esp_err_t
motor_driver_set_steering_speed(int8_t speed_percent) {
    if (speed_percent < -100 || speed_percent > 100) {
        return ESP_ERR_INVALID_ARG;
    }
    set_motor_output(speed_percent, LEDC_CHANNEL_1, CONFIG_MOTOR_STEERING_IN3_GPIO, CONFIG_MOTOR_STEERING_IN4_GPIO);
    s_steeringSpeed = speed_percent;
    s_lastCommandUs = esp_timer_get_time();
    return ESP_OK;
}

int8_t
motor_driver_get_drive_speed(void) {
    return s_driveSpeed;
}

int8_t
motor_driver_get_steering_speed(void) {
    return s_steeringSpeed;
}

void
motor_driver_failsafe_check(void) {
#if CONFIG_MOTOR_FAILSAFE_TIMEOUT_MS > 0
    if (s_driveSpeed == 0 && s_steeringSpeed == 0) {
        return;
    }
    int64_t elapsedMs = (esp_timer_get_time() - s_lastCommandUs) / 1000;
    if (elapsedMs >= CONFIG_MOTOR_FAILSAFE_TIMEOUT_MS) {
        set_motor_output(0, LEDC_CHANNEL_0, CONFIG_MOTOR_DRIVE_IN1_GPIO, CONFIG_MOTOR_DRIVE_IN2_GPIO);
        set_motor_output(0, LEDC_CHANNEL_1, CONFIG_MOTOR_STEERING_IN3_GPIO, CONFIG_MOTOR_STEERING_IN4_GPIO);
        s_driveSpeed = 0;
        s_steeringSpeed = 0;
    }
#endif
}
