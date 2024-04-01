#include "pwm_motor.h"

void drive_servo_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle) {
    /**
     * Set the motor to go forward
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * @param duty_cycle The duty cycle
     * 
     * @return void
    */
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

void drive_servo_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle) {
    /**
     * Set the motor to go backward
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * @param duty_cycle The duty cycle
     * 
     * @return void
    */
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
}

void drive_servo_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num) {
    /**
     * Stop the motor
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * 
     * @return void
    */
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A);
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B);
}

void drive_servo_forward_action(motor_action_data_t motor_action_data) {
    /**
     * Set the motor to go forward. This function is called from the state machine.
     * 
     * @param motor_action_data The motor action data
     * 
     * @return void
    */
    if (motor_action_data.motor_id == MOTOR_LEFT) {
        drive_servo_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, motor_action_data.duty_cycle_left);
    } else if (motor_action_data.motor_id == MOTOR_RIGHT) {
        drive_servo_forward(MCPWM_UNIT_1, MCPWM_TIMER_0, motor_action_data.duty_cycle_right);
    } else if (motor_action_data.motor_id == MOTOR_ALL) {
        drive_servo_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, motor_action_data.duty_cycle_left);
        drive_servo_forward(MCPWM_UNIT_1, MCPWM_TIMER_0, motor_action_data.duty_cycle_right);
    }
}

void drive_servo_backward_action(motor_action_data_t motor_action_data) {
    /**
     * Set the motor to go backward. This function is called from the state machine.
     * 
     * @param motor_action_data The motor action data
     * 
     * @return void
    */
    if (motor_action_data.motor_id == MOTOR_LEFT) {
        drive_servo_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, motor_action_data.duty_cycle_right);
    } else if (motor_action_data.motor_id == MOTOR_RIGHT) {
        drive_servo_backward(MCPWM_UNIT_1, MCPWM_TIMER_0, motor_action_data.duty_cycle_left);
    } else if (motor_action_data.motor_id == MOTOR_ALL) {
        drive_servo_backward(MCPWM_UNIT_0, MCPWM_TIMER_0, motor_action_data.duty_cycle_left);
        drive_servo_backward(MCPWM_UNIT_1, MCPWM_TIMER_0, motor_action_data.duty_cycle_right);
    }
}

void drive_servo_stop_action(motor_action_data_t motor_action_data) {
    /**
     * Stop the motor. This function is called from the state machine.
     * 
     * @param motor_action_data The motor action data
     * 
     * @return void
    */
    if (motor_action_data.motor_id == MOTOR_LEFT) {
        drive_servo_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
    } else if (motor_action_data.motor_id == MOTOR_RIGHT) {
        drive_servo_stop(MCPWM_UNIT_1, MCPWM_TIMER_0);
    } else if (motor_action_data.motor_id == MOTOR_ALL) {
        drive_servo_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
        drive_servo_stop(MCPWM_UNIT_1, MCPWM_TIMER_0);
    }
}

void drive_servo_configure(void) {
    
}