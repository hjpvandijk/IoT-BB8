#pragma once

#ifndef _DRIVE_SERVO_H
#define _DRIVE_SERVO_H

#include <driver/gpio.h>
#include "waveshare_servo.h"

extern servo_handle_t drive_servo;


// Define uart pins
//DEVKIT V1
// #define UART_NUM UART_NUM_2
// #define TX_GPIO_PIN GPIO_NUM_17
// #define RX_GPIO_PIN GPIO_NUM_16

//S3
#define UART_NUM UART_NUM_0
#define TX_GPIO_PIN GPIO_NUM_43
#define RX_GPIO_PIN GPIO_NUM_44



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