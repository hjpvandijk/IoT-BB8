#include "servo_pwm.h"
#include <driver/gpio.h>


#ifndef _MODE_SWITCH_SERVO_H
#define _MODE_SWITCH_SERVO_H


#define MODE_SWITCH_SERVO_PIN GPIO_NUM_42 //?


#define HORIZONTAL_POSITION 90
#define FULL_RIGHT 140
#define FULL_LEFT 40

#define DELAY_MS 0


typedef enum {
    SHELL_MODE = FULL_LEFT,
    PULLEY_MODE = FULL_RIGHT,
} ModeSwitchPosition;

int mode_switch_target_position = SHELL_MODE;

void mode_switch_servo_init(void);
void mode_switch_servo_set_position(ModeSwitchPosition position);
void mode_switch_servo_task(void *args);

#endif // _MODE_SWITCH_SERVO_H