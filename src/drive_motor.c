#include "drive_servo.h"

void drive_servo_forward(uint16_t speed) {
    /**
     * Set the motor to go forward
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * @param speed The speed
     * 
     * @return void
    */
    servo_set_speed(&servo, speed);
}

void drive_servo_backward(uint16_t speed) {
    /**
     * Set the motor to go backward
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * @param speed The speed
     * 
     * @return void
    */
    uint16_t speed_backwards = speed | (1 << 15);
    servo_set_speed(&servo, speed_backwards);

}

void drive_servo_stop() {
    /**
     * Stop the motor
     * 
     * @param mcpwm_num The MCPWM unit number
     * @param timer_num The MCPWM timer number
     * 
     * @return void
    */

    servo_set_speed(&servo, 0);

}

void drive_servo_forward_action(uint16_t speed) {
    /**
     * Set the motor to go forward. This function is called from the state machine.
     * 
     * @param speed The servo speed
     * 
     * @return void
    */
    drive_servo_forward(speed);
}

void drive_servo_backward_action(uint16_t speed) {
    /**
     * Set the motor to go backward. This function is called from the state machine.
     * 
     * @param speed The servo speed
     * 
     * @return void
    */
    drive_servo_backward(speed);
}

void drive_servo_stop_action() {
    /**
     * Stop the motor. This function is called from the state machine.
     * 
     * @return void
    */
    drive_servo_stop();
}

void drive_servo_configure(void) {
    servo.uart = UART_NUM_2;
    servo.servo_id = 1;
    servo.state = servo_state;
}