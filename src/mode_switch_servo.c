#include "state_machine.h"
#include <mode_switch_servo.h>

#include "esp_log.h"

static const char* MODE_SWITCH_SERVO_TAG = "MODE_SWITCH_SERVO";

servo_t * mode_switch_servo;

int mode_switch_servo_target_position = SHELL_MODE;

void mode_switch_servo_init(void){
    mode_switch_servo = servo_create();
    init_servo(mode_switch_servo, MODE_SWITCH_SERVO_PIN, FULL_LEFT_MS, HORIZONTAL_POSITION_MS, FULL_RIGHT_MS, DELAY_MS);
}

void mode_switch_servo_set_position(ModeSwitchPosition position){
    switch (position)
    {
    case SHELL_MODE:
        ESP_LOGI(MODE_SWITCH_SERVO_TAG, "Setting position to SHELL_MODE");
        rotate_servo_to_min(mode_switch_servo);
        break;
    case PULLEY_MODE:
        ESP_LOGI(MODE_SWITCH_SERVO_TAG, "Setting position to PULLEY_MODE");
        rotate_servo_to_max(mode_switch_servo);
        break;
    case IN_BETWEEN_MODES:
        ESP_LOGI(MODE_SWITCH_SERVO_TAG, "Setting position to IN_BETWEEN_MODES");
        rotate_servo_to_mid(mode_switch_servo);
        break;
    default:
        break;
    }
}

void mode_switch_servo_task(void *args) {
    while(1){
        if(mode_switch_servo->current_angle != mode_switch_target_position){
            mode_switch_servo_set_position(mode_switch_target_position);
        }  
    }

}

