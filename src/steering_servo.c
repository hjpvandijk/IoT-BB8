#include "state_machine.h"
#include <steering_servo.h>

#include "esp_log.h"

static const char* STEERING_SERVO_TAG = "STEERING_SERVO";


servo_t * steering_servo;

int steering_target_position = MID;

void steering_servo_init(void){
    steering_servo = servo_create();
    init_servo(steering_servo, STEERING_SERVO_PIN, FULL_LEFT, HORIZONTAL_POSITION, FULL_RIGHT, DELAY_MS);

    
}

void steering_servo_set_position(SteeringPosition position){
    switch (position)
    {
    case LEFT:
        ESP_LOGI(STEERING_SERVO_TAG, "Setting position to LEFT");
        rotate_servo_to_min(steering_servo);
        break;
    case MID:
        ESP_LOGI(STEERING_SERVO_TAG, "Setting position to MID");
        rotate_servo_to_mid(steering_servo);
        break;
    case RIGHT:
        ESP_LOGI(STEERING_SERVO_TAG, "Setting position to RIGHT");
        rotate_servo_to_max(steering_servo);
        break;
    default:
        break;
    }
}

void steering_servo_task(void *args) {
    int pos = -50;
    while(1){
        // steering_servo_set_position(pos++);
        // ESP_LOGI(STEERING_SERVO_TAG, "Current position: %d", pos);
        // if(pos>100) pos = -50;
        // if(steering_servo->current_angle != steering_target_position){
        //     steering_servo_set_position(steering_target_position);
        // }
        //Active balancing
        //Basically rotate the servo to the opposite direction of the current IMU angle relative to target orientation
        vTaskDelay(100 / portTICK_PERIOD_MS);   
    }

}