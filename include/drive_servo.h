#pragma once

#ifndef _DRIVE_SERVO_H
#define _DRIVE_SERVO_H

#include <driver/gpio.h>
#include "waveshare_servo.h"

extern servo_handle_t drive_servo;

// Define motor pins
#define SERVO_GPIO GPIO_NUM_15 //?



#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define MOTOR_ALL -1

void drive_servo_forward(uint16_t speed);
void drive_servo_backward(uint16_t speed);
void drive_servo_stop();
void drive_servo_forward_action(uint16_t speed);
void drive_servo_backward_action(uint16_t speed);
void drive_servo_stop_action();
void drive_servo_configure(void);

#endif // _DRIVE_SERVO_H