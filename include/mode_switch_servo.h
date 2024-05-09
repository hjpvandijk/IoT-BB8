#include "servo_pwm.h"
#include <driver/gpio.h>


#ifndef _MODE_SWITCH_SERVO_H
#define _MODE_SWITCH_SERVO_H


//DEVKIT V1
// #define MODE_SWITCH_SERVO_PIN GPIO_NUM_5
//S3
#define MODE_SWITCH_SERVO_PIN GPIO_NUM_4



#define HORIZONTAL_POSITION_MS 35   
#define FULL_RIGHT_MS 90
#define FULL_LEFT_MS 20

#define DELAY_MS 50


typedef enum {
    SHELL_MODE = FULL_RIGHT_MS,
    PULLEY_MODE = FULL_LEFT_MS,
    IN_BETWEEN_MODES = HORIZONTAL_POSITION_MS,
} ModeSwitchPosition;

extern int mode_switch_target_position;

void mode_switch_servo_init(void);
void mode_switch_servo_set_position(ModeSwitchPosition position);
void mode_switch_servo_task(void *args);

#endif // _MODE_SWITCH_SERVO_H