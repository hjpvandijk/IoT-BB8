#include <action_handler.h>
#include "state_machine.h"

#include "esp_log.h"

volatile int stop_counter = 0;
volatile int brake_pulse_counter = 0;

int objective_before_turning = OBJECTIVE_NONE;


int process_objective_switch(int previous_objective, int current_objective) {
    /**
     * Process the objective switch
     * 
     * @param previous_objective The previous objective
     * @param current_objective The current objective
     * 
     * @return 1 if the objective has changed, 0 otherwise
    */
    if (previous_objective != current_objective) {
        // if ((previous_objective == OBJECTIVE_FORWARD &&get_current_action() == ACTION_BACKWARD) || (previous_objective == OBJECTIVE_BACKWARD && get_current_action() == ACTION_FORWARD)) {
        //         set_current_action(ACTION_STOP);
        //         return 1;
        // } else 
        if (current_objective == OBJECTIVE_TURN_LEFT || current_objective == OBJECTIVE_TURN_RIGHT || current_objective == OBJECTIVE_GO_STRAIGHT) {
            objective_before_turning = previous_objective;
            return 0;
        } else if (previous_objective == OBJECTIVE_MOVETO) {
            if (get_current_action() == ACTION_FORWARD) {
                set_current_action(ACTION_STOP);
                return 1;
            } else if (get_current_action() == ACTION_TURN_LEFT || get_current_action() == ACTION_TURN_RIGHT) {
                stop_turn_action(true);
                return 1;
            }
        }
        
    }
    return 0;
}

void process_objective(State state, Target target) {
    /**
     * Process the objective based on the current state and target
     * Objectives are more complex than actions, e.g. move to a specific location
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */
    // ESP_LOGI("ACTION_HANDLER", "PROCESSING OBJECTIVE");
    if (process_objective_switch(state.previous_objective, state.objective)) {
        return;
    }


    if (state.objective == OBJECTIVE_NONE) {
        state.action = ACTION_NONE;
        return;
    }

    if (state.objective == OBJECTIVE_FORWARD) {
        // ESP_LOGI("ACTION_HANDLER", "RECEIVED FORWARDS, %d", state.action);
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_FORWARD);
            return;
        // }
        return;
    }  

    if (state.objective == OBJECTIVE_BACKWARD) {
        // ESP_LOGI("ACTION_HANDLER", "RECEIVED BACKWARDS, %d", state.action);
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_BACKWARD);
            return;
        // }
        return;
    }

    if (state.objective == OBJECTIVE_STOP) {
        // ESP_LOGI("ACTION_HANDLER", "RECEIVED STOP, %d", state.action);
        // if (state.action == ACTION_NONE) {
            set_previous_objective(get_current_objective());
            // set_current_objective(OBJECTIVE_STOP);
            set_current_action(ACTION_STOP);
            return;
        // }
        return;
    }  

    if (state.objective == OBJECTIVE_TURN_LEFT) {
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_TURN_LEFT);
            return;
        // }
        return;
    }

    if (state.objective == OBJECTIVE_TURN_RIGHT) {
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_TURN_RIGHT);
            return;
        // }
        return;
    }  

    if (state.objective == OBJECTIVE_GO_STRAIGHT) {
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_GO_STRAIGHT);
            return;
        // }
        return;
    }

    if (state.objective == OBJECTIVE_SWITCH_TO_SHELL_MODE) {
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_SWITCH_TO_SHELL_MODE);
            return;
        // }
        return;
    }

    if (state.objective == OBJECTIVE_SWITCH_TO_PULLEY_MODE) {
        // if (state.action == ACTION_NONE) {
            set_current_action(ACTION_SWITCH_TO_PULLEY_MODE);
            return;
        // }
        return;
    }

    if (state.objective == OBJECTIVE_REBOOT) {
        //set_current_objective(OBJECTIVE_INIT); Might not be needed
        esp_restart();
    }

    if (state.objective == OBJECTIVE_MOVETO) {

        // Wait with processing until the previous action is terminated
        if (state.action == ACTION_STOP) {
            return;
        }

        // Get the distance to the target
        float distance_to_target = sqrt(pow(target.x - state.x, 2) + pow(target.y - state.y, 2));

        // // If we are close enough to the target, stop
        // if (distance_to_target < TARGET_OFFSET) {
        //     if (state.action == ACTION_NONE) {
        //         set_previous_objective(get_current_objective());
        //         set_current_objective(OBJECTIVE_NONE);
        //     } else {
        //         set_current_action(ACTION_STOP);
        //     }
        //     return;
        // }

        // Get the angle to the target
        float angle_to_target = atan2(target.y - state.y, target.x - state.x) * (180.0 / M_PI);

        // Calculate the angle difference
        //      - Negative angle difference means turn right
        //      - Positive angle difference means turn left
        float angle_difference = calculate_angle_difference(angle_to_target, state.rotation);
        float angle_difference_abs = fabs(angle_difference);
        
        // Rotate until the angle difference is small enough
        if(get_current_action() != ACTION_FORWARD) {
            if (angle_difference_abs > ANGLE_OFFSET) {
                if (angle_difference > 0.0) {
                    set_current_action(ACTION_TURN_LEFT);
                    return;
                } else {
                    set_current_action(ACTION_TURN_RIGHT);
                    return;
                }
            } else {
                if (state.action == ACTION_TURN_LEFT || state.action == ACTION_TURN_RIGHT) {
                    // TODO: Should be an action, fix this in the future
                    stop_turn_action(true);
                    return;
                }
                
                // Angle to small enough the move forward!
                set_current_action(ACTION_FORWARD);
                return;
            }
        } else {
            if (angle_difference_abs > ANGLE_OFFSET*2) {
                set_current_action(ACTION_STOP);
                return;
            }
        }
        return;
    }
    return;
}

float calculate_angle_difference(float angle_to_target, float yaw) {
    /**
     * Calculate the difference between two angles in cartesian plane
     * Assume that 0 degrees is straight ahead for the yaw
     * Assume that 0 degrees is right for the angle_to_target
     * 
     * @param angle_to_target The first angle
     * @param yaw The second angle
     * 
     * @return The difference between the two angles
    */

    float corrected_angle_to_target = angle_to_target - 90.0;

    if (corrected_angle_to_target < 0.0) {
        corrected_angle_to_target += 360.0;
    }

    corrected_angle_to_target = fabs(corrected_angle_to_target - 360.0);

    float angle_difference = yaw - corrected_angle_to_target;

    if (angle_difference > 180.0) {
        angle_difference -= 360.0;
    } else if (angle_difference < -180.0) {
        angle_difference += 360.0;
    }

    return angle_difference;
}

void process_action(State state, Target target, TickType_t* last_turn_pulse) {
    /**
     * Process the action based on the current state and target
     * Actions are the simplest form of movement, e.g. forward, backward, turn left, turn right
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */

   if(get_previous_action() == state.action) {
       return;
   }
   set_previous_action(state.action);

    if (state.action == ACTION_NONE) {
        return;
    }
    else if (state.action == ACTION_STOP) {
        ESP_LOGI("ACTION_HANDLER", "ACTION STOP");
        stop_action(state);
        return;
    }
    else if (state.action == ACTION_FORWARD) {
        ESP_LOGI("ACTION_HANDLER", "ACTION FORWARDS");
        forward_action(state, target);
        return;
    }
    else if (state.action == ACTION_BACKWARD) {
        ESP_LOGI("ACTION_HANDLER", "ACTION BACKWARDS");
        backward_action(state, target);
        return;
    }
    else if (state.action == ACTION_TURN_LEFT || state.action == ACTION_TURN_RIGHT || state.action == ACTION_GO_STRAIGHT) {
        // do_turn_pulse(state, last_turn_pulse);
        turn_action(state, state.action);
        return;
    } 
    else if (state.action == ACTION_SWITCH_TO_PULLEY_MODE || state.action == ACTION_SWITCH_TO_SHELL_MODE) {
        switch_mode_action(state.action, target);
        return;
    }
    return;
}

void stop_action(State state) {
    /**
     * Stop the motors
     * 
     * @param state The current state
     * 
     * @return void
    */
    drive_servo_stop();
    set_current_speed(0);
    return;
}

void forward_action(State state, Target target) {
    /**
     * Move forward
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */
    // motor_action_data_t motor_action_data;
    // motor_action_data.motor_id = MOTOR_ALL;

    // float adjusted_duty_cycle = current_state.duty_cycle;

    // if (current_state.duty_cycle < target.speed - ACCEL_STEP_SIZE) {
    //     adjusted_duty_cycle += ACCEL_STEP_SIZE;
    // } else {
    //     adjusted_duty_cycle = target.speed;
    // }

    // motor_action_data.duty_cycle_left = adjusted_duty_cycle;
    // motor_action_data.duty_cycle_right = adjusted_duty_cycle;

    drive_servo_forward(target.speed);
    set_current_speed(target.speed);
    
    return;
}

void backward_action(State state, Target target) {
    /**
     * Move backward
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */


    drive_servo_backward(target.speed);
    set_current_speed(target.speed);
    
    return;
}

// void do_turn_pulse(State state, TickType_t* last_turn_pulse) {
//     /**
//      * Do a turn pulse
//      * 
//      * @param state The current state
//      * @param target The target state
//      * 
//      * @return void
//     */

//     if ((xTaskGetTickCount() - *last_turn_pulse) < (TURN_INTERVAL_MS+TURN_PULSE_MS) / portTICK_PERIOD_MS) {
//         if ((xTaskGetTickCount() - *last_turn_pulse) > TURN_INTERVAL_MS / portTICK_PERIOD_MS) {
//             // Turn for a short pulse if we have waited for TURN_INTERVAL_MS
//             turn_action(state, state.action);
//             return;
//         }
//         return;
//     } else {
//         // set the value of last turn pulse to the current tick count
//         *last_turn_pulse = xTaskGetTickCount();
//         // Stop turning gracefully
//         stop_turn_action(false);
//         return;
//     }
// }

void stop_turn_action(bool final_turn) {
    /**
     * Stop the turn action
     * 
     * @param state The current state
     * @param action The current action
     * 
     * @return void
    */

    // drive_servo_stop_action();

    steering_servo_set_position(MID);
    
    if(objective_before_turning != OBJECTIVE_TURN_LEFT && objective_before_turning != OBJECTIVE_TURN_RIGHT && objective_before_turning != OBJECTIVE_GO_STRAIGHT) {
        set_current_objective(objective_before_turning);
        return;
    }

    // set_current_speed(0.0f);
    return;
}

void turn_action(State state, int action) {
    /**
     * Logic for turning
     * 
     * @param state The current state
     * @param action The current action
     * 
     * @return void
    */


   if (action == ACTION_TURN_LEFT) {
        // steering servo left
        steering_servo_set_position(LEFT);
    } else if (action == ACTION_TURN_RIGHT) {
        //steering servo right
        steering_servo_set_position(RIGHT);
    } else if (action == ACTION_GO_STRAIGHT) {
        //steering servo mid
        steering_servo_set_position(MID);
    }

    if(objective_before_turning != OBJECTIVE_TURN_LEFT && objective_before_turning != OBJECTIVE_TURN_RIGHT && objective_before_turning != OBJECTIVE_GO_STRAIGHT) {
        set_current_objective(objective_before_turning);
        return;
    }

    return;
}

void switch_mode_action(int action, Target target){
    /**
     * Switch to driving mode
     * 
     * @return void
    */
    ESP_LOGI("ACTION_HANDLER", "ST servo backwards");
    drive_servo_backward(target.speed);
    set_current_speed(target.speed);
    mode_switch_servo_set_position(IN_BETWEEN_MODES);
    ESP_LOGI("ACTION_HANDLER", "ST servo forwards"); 
    vTaskDelay(3000 / portTICK_PERIOD_MS); //Give time to detach from first gear
    drive_servo_forward(target.speed);
    set_current_speed(target.speed);
    vTaskDelay(500 / portTICK_PERIOD_MS); //Give time to change direction
   if(action == ACTION_SWITCH_TO_PULLEY_MODE){
        // switch to pulley mode
        mode_switch_servo_set_position(PULLEY_MODE);
    } else if(action == ACTION_SWITCH_TO_SHELL_MODE){
        // switch to shell mode
        mode_switch_servo_set_position(SHELL_MODE);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    set_current_objective(OBJECTIVE_STOP);
    return;

}
void reset_stop_counter() {
    /**
     * Reset the stop counter
     * 
     * @return void
    */
    stop_counter = 0;
    return;
}

int get_stop_counter() {
    /**
     * Get the stop counter
     * 
     * @return The stop counter
    */
    return stop_counter;
}

void increment_stop_counter() {
    /**
     * Increment the stop counter
     * 
     * @return void
    */
    stop_counter++;
    return;
}

int get_brake_pulse_counter() {
    /**
     * Set the break pulse counter
     * 
     * @param value The value to set the break pulse counter to
     * 
     * @return void
    */
    return brake_pulse_counter;
}

void reset_brake_pulse_counter() {
    /**
     * Reset the break pulse counter
     * 
     * @return void
    */
    brake_pulse_counter = 0;
    return;
}

void increment_brake_pulse_counter() {
    /**
     * Increment the break pulse counter
     * 
     * @return void
    */
    brake_pulse_counter++;
    return;
}