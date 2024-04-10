#pragma once

#include <driver/gpio.h>
#include "waveshare_servo.h"

servo_state_t servo_state; // Initialize all members to zero
servo_handle_t servo;

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