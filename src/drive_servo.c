#include "drive_servo.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_ipc.h"

servo_handle_t drive_servo;

#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

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
    servo_set_speed(&drive_servo, speed);
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
    servo_set_speed(&drive_servo, speed_backwards);

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

    servo_set_speed(&drive_servo, 0);

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

void install_uart(){
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
}

void drive_servo_configure(void) {
    drive_servo.uart = UART_NUM_2;
    drive_servo.servo_id = 1;

    servo_state_t servo_state = {0}; // Initialize all members to zero
    drive_servo.state = servo_state;

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 1000000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // esp_ipc_call(1, install_uart, NULL);
    install_uart(); 
    // ESP_LOGI("DRIVE_SERVO", "uart_driver_install: %d", res);
    int res = uart_param_config(UART_NUM_2, &uart_config);
    ESP_LOGI("DRIVE_SERVO", "uart_param_config: %d", res);

    //Set UART pins (using UART0 default pins ie no changes.)
    res = uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_LOGI("DRIVE_SERVO", "uart_set_pin: %d", res);

        int servores = servo_init(&drive_servo, 10, 10);
    ESP_LOGI("DRIVE_SERVO", "servo_init: %d", servores);




}

