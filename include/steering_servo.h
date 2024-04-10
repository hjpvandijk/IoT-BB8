#include "servo_pwm.h"
#include <driver/gpio.h>

#ifndef _STEERING_SERVO_H
#define _STEERING_SERVO_H

#define STEERING_SERVO_PIN GPIO_NUM_43 //D6


#define HORIZONTAL_POSITION 90
#define FULL_RIGHT 140
#define FULL_LEFT 40

#define DELAY_MS 0


typedef enum {
    LEFT = FULL_LEFT,
    MID = HORIZONTAL_POSITION,
    RIGHT = FULL_RIGHT,
} SteeringPosition;

int steering_target_position = MID;

void steering_servo_init(void);
void steering_servo_set_position(SteeringPosition position);
void steering_servo_task(void *args);

#endif // _STEERING_SERVO_H