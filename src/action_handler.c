#include <action_handler.h>
#include "state_machine.h"


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
        set_current_action(ACTION_STOP);
        return 1;
    }
    return 0;
}

void process_objective(State state, Target target, int previous_objective) {
    /**
     * Process the objective based on the current state and target
     * Objectives are more complex than actions, e.g. move to a specific location
     * 
     * @param state The current state
     * @param target The target state
     * @param previous_objective The previous objective
     * 
     * @return void
    */

    if (process_objective_switch(previous_objective, state.objective)) {
        return;
    }

    if (state.objective == OBJECTIVE_NONE) {
        return;
    }

    if (state.objective == OBJECTIVE_FORWARD) {
        if (state.action == ACTION_NONE) {
            set_current_action(ACTION_FORWARD);
            return;
        }
        return;
    }  

    if (state.objective == OBJECTIVE_BACKWARD) {
        if (state.action == ACTION_NONE) {
            set_current_action(ACTION_BACKWARD);
            return;
        }
        return;
    }

    if (state.objective == OBJECTIVE_STOP) {
        if (state.action == ACTION_NONE) {
            set_current_objective(OBJECTIVE_NONE);
            return;
        }
        return;
    }  

    if (state.objective == OBJECTIVE_TURN_LEFT) {
        if (state.action == ACTION_NONE) {
            set_current_action(ACTION_TURN_LEFT);
            return;
        }
        return;
    }

    if (state.objective == OBJECTIVE_TURN_RIGHT) {
        if (state.action == ACTION_NONE) {
            set_current_action(ACTION_TURN_RIGHT);
            return;
        }
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

        // If we are close enough to the target, stop
        if (distance_to_target < TARGET_OFFSET) {
            if (state.action == ACTION_NONE) {
                set_current_objective(OBJECTIVE_NONE);
            } else {
                set_current_action(ACTION_STOP);
            }
            return;
        }

        // Get the angle to the target
        float angle_to_target = atan2(target.y - state.y, target.x - state.x) * (180.0 / M_PI);

        // Calculate the angle difference
        //      - Negative angle difference means turn left
        //      - Positive angle difference means turn right
        float angle_difference = calculate_angle_difference(angle_to_target, state.rotation);
        float angle_difference_abs = fabs(angle_difference);
        
        // Rotate until the angle difference is small enough
        if (angle_difference_abs > ANGLE_OFFSET) {
            if (angle_difference > 0.0) {
                set_current_action(ACTION_TURN_RIGHT);
                return;
            } else {
                set_current_action(ACTION_TURN_LEFT);
                return;
            }
        } else {
            if (state.action == ACTION_TURN_LEFT || state.action == ACTION_TURN_RIGHT) {
                set_current_action(ACTION_STOP);
                return;
            }
            
            // Angle to small enough the move forward!
            set_current_action(ACTION_FORWARD);
            return;
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

void process_action(State state, Target target) {
    /**
     * Process the action based on the current state and target
     * Actions are the simplest form of movement, e.g. forward, backward, turn left, turn right
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */
    if (state.action == ACTION_NONE) {
        return;
    }
    else if (state.action == ACTION_STOP) {
        stop_action(state);
        return;
    }
    else if (state.action == ACTION_FORWARD) {
        forward_action(state, target);
        return;
    }
    else if (state.action == ACTION_BACKWARD) {
        backward_action(state, target);
        return;
    }
    else if (state.action == ACTION_TURN_LEFT) {
        turn_left_action(state);
        return;
    }
    else if (state.action == ACTION_TURN_RIGHT) {
        turn_right_action(state);
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
    motor_action_data_t motor_action_data;
    motor_action_data.motor_id = MOTOR_ALL;

    // Check safely if duty cycle is float 0
    if (state.duty_cycle < EPSILON) {
        pwm_stop_action(motor_action_data);
        set_current_action(ACTION_NONE);
        return;
    }

    float adjusted_duty_cycle = state.duty_cycle;

    if (state.duty_cycle > EPSILON + BRAKE_STEP_SIZE) {
        adjusted_duty_cycle -= BRAKE_STEP_SIZE;
    } else {
        adjusted_duty_cycle = 0.0f;
    }

    motor_action_data.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data.duty_cycle_right = adjusted_duty_cycle;
    pwm_forward_action(motor_action_data);
    set_current_duty_cycle(adjusted_duty_cycle);    

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
    motor_action_data_t motor_action_data;
    motor_action_data.motor_id = MOTOR_ALL;

    float adjusted_duty_cycle = current_state.duty_cycle;

    if (current_state.duty_cycle < target.duty_cycle - ACCEL_STEP_SIZE) {
        adjusted_duty_cycle += ACCEL_STEP_SIZE;
    } else {
        adjusted_duty_cycle = target.duty_cycle;
    }

    motor_action_data.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data.duty_cycle_right = adjusted_duty_cycle;

    pwm_forward_action(motor_action_data);
    set_current_duty_cycle(adjusted_duty_cycle);
    
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
    motor_action_data_t motor_action_data;
    motor_action_data.motor_id = MOTOR_ALL;

    float adjusted_duty_cycle = current_state.duty_cycle;

    if (current_state.duty_cycle < target.duty_cycle - ACCEL_STEP_SIZE) {
        adjusted_duty_cycle += ACCEL_STEP_SIZE;
    } else {
        adjusted_duty_cycle = target.duty_cycle;
    }

    motor_action_data.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data.duty_cycle_right = adjusted_duty_cycle;

    pwm_backward_action(motor_action_data);
    set_current_duty_cycle(adjusted_duty_cycle);
    
    return;
}

void turn_left_action(State state) {
    /**
     * Turn left
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */
    motor_action_data_t motor_action_data_left;
    motor_action_data_t motor_action_data_right;

    float adjusted_duty_cycle = current_state.duty_cycle;

    if (current_state.duty_cycle < TURN_DUTY_CYCLE - TURN_STEP_SIZE) {
        adjusted_duty_cycle += TURN_STEP_SIZE;
    } else {
        adjusted_duty_cycle = TURN_DUTY_CYCLE;
    }

    motor_action_data_left.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data_left.duty_cycle_right = adjusted_duty_cycle;
    motor_action_data_right.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data_right.duty_cycle_right = adjusted_duty_cycle;

    motor_action_data_left.motor_id = MOTOR_LEFT;
    pwm_backward_action(motor_action_data_left);
    motor_action_data_right.motor_id = MOTOR_RIGHT;
    pwm_forward_action(motor_action_data_right);

    set_current_duty_cycle(adjusted_duty_cycle);
}

void turn_right_action(State state) {
    /**
     * Turn right
     * 
     * @param state The current state
     * @param target The target state
     * 
     * @return void
    */
    motor_action_data_t motor_action_data_left;
    motor_action_data_t motor_action_data_right;

    float adjusted_duty_cycle = current_state.duty_cycle;

    if (current_state.duty_cycle < TURN_DUTY_CYCLE - TURN_STEP_SIZE) {
        adjusted_duty_cycle += TURN_STEP_SIZE;
    } else {
        adjusted_duty_cycle = TURN_DUTY_CYCLE;
    }

    motor_action_data_left.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data_left.duty_cycle_right = adjusted_duty_cycle;
    motor_action_data_right.duty_cycle_left = adjusted_duty_cycle;
    motor_action_data_right.duty_cycle_right = adjusted_duty_cycle;

    motor_action_data_left.motor_id = MOTOR_LEFT;
    pwm_forward_action(motor_action_data_left);
    motor_action_data_right.motor_id = MOTOR_RIGHT;
    pwm_backward_action(motor_action_data_right);

    set_current_duty_cycle(adjusted_duty_cycle);
}
