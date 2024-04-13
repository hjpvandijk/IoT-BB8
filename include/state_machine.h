#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "state_machine.h"
#include "mqtt.h"
#include <mqtt_client.h>

#define ACTION_NONE 0
#define ACTION_FORWARD 1
#define ACTION_BACKWARD 2
#define ACTION_STOP 3
#define ACTION_TURN_LEFT 4
#define ACTION_TURN_RIGHT 5
#define ACTION_GO_STRAIGHT 6
#define ACTION_SWITCH_TO_SHELL_MODE 7
#define ACTION_SWITCH_TO_PULLEY_MODE 8
#define ACTION_UNDEFINED -1

#define OBJECTIVE_NONE 0
#define OBJECTIVE_MOVETO 1
#define OBJECTIVE_FORWARD 2
#define OBJECTIVE_BACKWARD 3
#define OBJECTIVE_STOP 4
#define OBJECTIVE_TURN_LEFT 5
#define OBJECTIVE_TURN_RIGHT 6
#define OBJECTIVE_GO_STRAIGHT 7
#define OBJECTIVE_SWITCH_TO_SHELL_MODE 8
#define OBJECTIVE_SWITCH_TO_PULLEY_MODE 9
#define OBJECTIVE_INIT 10
#define OBJECTIVE_REBOOT 11
#define OBJECTIVE_UNDEFINED -1

#define STATE_INTERVAL_TIME_MS 1000

#define SWITCH_MODE_SPEED 400


typedef struct {
    float x;
    float y;
    float rotation;
    float pitch;
    float roll;
    uint16_t speed;
    float real_speed;
    float acceleration;
    float total_displacement;
    int action;
    int objective;
    int previous_objective;
} State;

typedef struct {
    float x;
    float y;
    uint16_t speed;
} Target;


extern volatile State current_state;
extern volatile Target target;

void set_current_coordinates(float x, float y);
void set_current_action(int action);
void set_current_objective(int objective);
void set_current_rotation(float rotation);
void set_current_speed(uint16_t speed);
void set_current_acceleration(float acceleration);
void set_current_pitch(float pitch);
void set_current_roll(float roll);
void set_total_displacement(float total_displacement);
void set_target_coordinates(float x, float y);
void set_target_speed(uint16_t speed);
void set_previous_objective(int previous_objective);
float get_current_x_pos();
float get_current_y_pos();
float get_current_rotation();
uint16_t get_current_speed();
uint16_t get_current_speed();
float get_current_acceleration();
float get_current_pitch();
float get_current_roll();
float get_total_displacement();
float get_target_x_pos();
float get_target_y_pos();
uint16_t get_target_speed();
int get_current_action();
int get_current_objective();
int get_previous_objective();
State get_current_state();
Target get_target();
void report_state_task(void *args);