#include "servo_pwm.h"
#include <driver/gpio.h>


#ifndef _MODE_SWITCH_SERVO_H
#define _MODE_SWITCH_SERVO_H


#define MODE_SWITCH_SERVO_PIN GPIO_NUM_22 //?


#define HORIZONTAL_POSITION 0
#define FULL_RIGHT 50
#define FULL_LEFT -50

#define DELAY_MS 0


typedef enum {
    SHELL_MODE = FULL_LEFT,
    PULLEY_MODE = FULL_RIGHT,
} ModeSwitchPosition;

extern int mode_switch_target_position;

void mode_switch_servo_init(void);
void mode_switch_servo_set_position(ModeSwitchPosition position);
void mode_switch_servo_task(void *args);

#endif // _MODE_SWITCH_SERVO_H