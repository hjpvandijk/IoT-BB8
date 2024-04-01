#pragma once

#include <driver/gpio.h>
#include "waveshare_servo.h"
#include "motor_action_data.h"

servo_state_t servo_state;
servo_handle_t servo = {UART_NUM_2, 1, servo_state};

// Define motor pins
#define SERVO_GPIO GPIO_NUM_15 //?



#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define MOTOR_ALL -1

// void drive_servo_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
// void drive_servo_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);
// void drive_servo_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);
// void drive_servo_forward_action(motor_action_data_t motor_action_data);
// void drive_servo_backward_action(motor_action_data_t motor_action_data);
// void drive_servo_stop_action(motor_action_data_t motor_action_data);
// void drive_servo_configure(void);