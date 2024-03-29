#include "state_machine.h"
#include <steering_servo.h>


servo_t * steering_servo;

void steering_servo_init(void){
    steering_servo = servo_create();
    init_servo(steering_servo, STEERING_SERVO_PIN, FULL_LEFT, HORIZONTAL_POSITION, FULL_RIGHT, DELAY_MS);

    
}

void steering_servo_set_position(SteeringPosition position){
    switch (position)
    {
    case LEFT:
        ESP_LOGI("steering_servo", "Setting position to LEFT");
        rotate_servo_to_min(steering_servo);
        break;
    case MID:
        ESP_LOGI("steering_servo", "Setting position to MID");
        rotate_servo_to_mid(steering_servo);
        break;
    case RIGHT:
        ESP_LOGI("steering_servo", "Setting position to RIGHT");
        rotate_servo_to_max(steering_servo);
        break;
    default:
        break;
    }
}

void steering_servo_task(void *args) {
    while(1){
        if(steering_servo->current_angle != target_position){
            steering_servo_set_position(target_position);
        }
        //Active balancing
        //Basically rotate the servo to the opposite direction of the current IMU angle relative to target orientation
        vTaskDelay(100 / portTICK_PERIOD_MS);   
    }

}