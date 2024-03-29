#include "driver/mcpwm_prelude.h"


typedef struct {
    int gpio;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    int current_angle;
    int min_angle;
    int mid_angle;
    int max_angle;
    int delay_ms;
} servo_t;





servo_t* servo_create();

static inline uint32_t example_angle_to_compare(int angle);

void init_servo(servo_t* servo, int gpio, int min_angle, int mid_angle, int max_angle, int delay_ms);

// Rotate the servo to the specified angle at a specified delay
void rotate_servo(servo_t* servo, int angle, int delay_ms);

// Rotate the servo to the minimum angle
void rotate_servo_to_min(servo_t* servo);

// Rotate the servo to the middle angle
void rotate_servo_to_mid(servo_t* servo);

// Rotate the servo to the maximum angle
void rotate_servo_to_max(servo_t* servo);
