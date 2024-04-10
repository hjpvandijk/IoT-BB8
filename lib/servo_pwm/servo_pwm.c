/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "servo_pwm.h"

static const char *TAG = "example";

// Please consult the datasheet of your servo before changing the following parameters
#define SERVO_MIN_PULSEWIDTH_US 900  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2100  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE        -90   // Minimum angle
#define SERVO_MAX_DEGREE        90    // Maximum angle

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms




servo_t* servo_create() {
    servo_t* new_servo = (servo_t*)malloc(sizeof(servo_t));
    if(new_servo == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for new servo");
        return NULL;
    }
    return new_servo;
}

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void init_servo(servo_t* servo, int gpio, int min_angle, int mid_angle, int max_angle, int delay_ms) {

    ESP_LOGI(TAG, "Setting gpio, min, mid and max angles, and delay");
    servo->gpio = gpio;
    servo->min_angle = min_angle;
    servo->mid_angle = mid_angle;
    servo->max_angle = max_angle;
    servo->delay_ms = delay_ms;

    ESP_LOGI(TAG, "Create timer and operator");
    servo->timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &servo->timer));

    servo->oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0 // operator must be in the same group as the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &servo->oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(servo->oper, servo->timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    servo->comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(servo->oper, &comparator_config, &servo->comparator));

    servo->generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = servo->gpio,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(servo->oper, &generator_config, &servo->generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(servo->generator,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(servo->generator,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, servo->comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(servo->timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(servo->timer, MCPWM_TIMER_START_NO_STOP));

    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, example_angle_to_compare(0))); //Set servo to middle
    servo->current_angle = 0;


}

// Rotate the servo to the specified angle at a specified delay
void rotate_servo(servo_t* servo, int angle, int delay_ms){
    ESP_LOGI(TAG, "Angle of rotation: %d", angle);
    if(delay_ms > 0) {
        while(servo->current_angle != angle) {
            if(servo->current_angle < angle) {
                servo->current_angle++;
            } else {
                servo->current_angle--;
            }
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, example_angle_to_compare(servo->current_angle)));
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    } else {
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(servo->comparator, example_angle_to_compare(angle)));
    }
}

// Rotate the servo to the minimum angle
void rotate_servo_to_min(servo_t* servo) {
    rotate_servo(servo, servo->min_angle, servo->delay_ms);
}

// Rotate the servo to the middle angle
void rotate_servo_to_mid(servo_t* servo) {
    rotate_servo(servo, servo->mid_angle, servo->delay_ms);
}

// Rotate the servo to the maximum angle
void rotate_servo_to_max(servo_t* servo) {
    rotate_servo(servo, servo->max_angle, servo->delay_ms);
}
