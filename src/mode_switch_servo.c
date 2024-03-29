#include "state_machine.h"
#include <mode_switch_servo.h>


servo_t * mode_switch_servo;

void mode_switch_servo_init(void){
    mode_switch_servo = servo_create();
    init_servo(mode_switch_servo, MODE_SWITCH_SERVO_PIN, FULL_LEFT, HORIZONTAL_POSITION, FULL_RIGHT, DELAY_MS);

    
}

void mode_switch_servo_set_position(ModeSwitchPosition position){
    switch (position)
    {
    case SHELL_MODE:
        ESP_LOGI("mode_switch_servo", "Setting position to SHELL_MODE");
        rotate_servo_to_min(mode_switch_servo);
        break;
    case PULLEY_MODE:
        ESP_LOGI("mode_switch_servo", "Setting position to PULLEY_MODE");
        rotate_servo_to_max(mode_switch_servo);
        break;
    default:
        break;
    }
}

void mode_switch_servo_task(void *args) {
    while(1){
        if(mode_switch_servo->current_angle != target_position){
            mode_switch_servo_set_position(target_position);
        }
        //Active balancing
        //Basically rotate the servo to the opposite direction of the current IMU angle relative to target orientation
        vTaskDelay(100 / portTICK_PERIOD_MS);   
    }

}