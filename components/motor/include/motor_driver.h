#pragma once

#include "esp_err.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Configures the ENA/ENB PWM channels and IN1..IN4 direction GPIOs and
 * stops both motors. Call once before the first speed write. */
void motor_driver_init(void);

/* speed_percent range is -100..100, 0 = stop.
 * Drive motor (rear wheel): negative = backward, positive = forward.
 * Steering motor (front wheel): negative = left, positive = right. */
esp_err_t motor_driver_set_drive_speed(int8_t speed_percent);
esp_err_t motor_driver_set_steering_speed(int8_t speed_percent);

int8_t motor_driver_get_drive_speed(void);
int8_t motor_driver_get_steering_speed(void);

/* Call periodically from the server loop. Stops both motors if
 * CONFIG_MOTOR_FAILSAFE_TIMEOUT_MS has elapsed since the last speed
 * write. No-op when the timeout is disabled (0). */
void motor_driver_failsafe_check(void);

#ifdef __cplusplus
}
#endif
