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

// void install_uart(){
//     uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
// }

void task1(void *pvParameters) {
    // Install UART driver on UART_NUM
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0));

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 1000000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .source_clk = UART_SCLK_DEFAULT,
        .rx_flow_ctrl_thresh = 122,
    };
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    //Set UART pins (using UART0 default pins ie no changes.)
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Your additional code for task 1
    vTaskDelete(NULL);

}

void task2(void *pvParameters) {
    // Initialize servo
    int servores = servo_init(&drive_servo, 10, 10);
    ESP_LOGI("DRIVE_SERVO", "servo_init: %d", servores); 
    // Your additional code for task 2

    vTaskDelete(NULL);
}

void drive_servo_configure(void) {
    drive_servo.uart = UART_NUM;
    drive_servo.servo_id = 1;

    servo_state_t servo_state = {0}; // Initialize all members to zero
    drive_servo.state = servo_state;

    
    
    // esp_ipc_call(1, install_uart, NULL);
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0));
    // xTaskCreatePinnedToCore(
    //     task1,        // Task function
    //     "Task1",      // Task name
    //     10000,        // Stack size
    //     NULL,         // Task parameters
    //     1,            // Task priority
    //     NULL,         // Task handle
    //     0             // Core number (0 for core 0)
    // );

   /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 1000000,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,    };

    // install_uart(); 
    // ESP_LOGI("DRIVE_SERVO", "uart_driver_install: %d", res);
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    // //Set UART pins (using UART0 default pins ie no changes.)
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TX_GPIO_PIN, RX_GPIO_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    int servores = servo_init(&drive_servo, 10, 10);
    ESP_LOGI("DRIVE_SERVO", "servo_init: %d", servores); 
    // Create task 2 and pin it to core 1
    // xTaskCreatePinnedToCore(
    //     task2,        // Task function
    //     "Task2",      // Task name
    //     10000,        // Stack size
    //     NULL,         // Task parameters
    //     1,            // Task priority
    //     NULL,         // Task handle
    //     1             // Core number (1 for core 1)
    // );




}

