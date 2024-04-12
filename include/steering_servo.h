#include "servo_pwm.h"
#include <driver/gpio.h>

#ifndef _STEERING_SERVO_H
#define _STEERING_SERVO_H

//DEVKIT V1
// #define STEERING_SERVO_PIN GPIO_NUM_4
//S3
#define STEERING_SERVO_PIN GPIO_NUM_3


#define HORIZONTAL_POSITION 10
#define FULL_RIGHT -30
#define FULL_LEFT 50

#define DELAY_MS 0


typedef enum {
    LEFT = FULL_LEFT,
    MID = HORIZONTAL_POSITION,
    RIGHT = FULL_RIGHT,
} SteeringPosition;

extern int steering_target_position;

void steering_servo_init(void);
void steering_servo_set_position(SteeringPosition position);
void steering_servo_task(void *args);

#endif // _STEERING_SERVO_H