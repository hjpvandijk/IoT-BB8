#include "servo_pwm.h"
#include <driver/gpio.h>

#define MODE_SWITCH_SERVO_PIN GPIO_NUM_42 //?


#define HORIZONTAL_POSITION 90
#define FULL_RIGHT 140
#define FULL_LEFT 40

#define DELAY_MS 0


typedef enum {
    SHELL_MODE = FULL_LEFT,
    PULLEY_MODE = FULL_RIGHT,
} ModeSwitchPosition;

ModeSwitchPosition target_position = SHELL_MODE;

void mode_switch_servo_init(void);
void mode_switch_servo_set_position(ModeSwitchPosition position);
void mode_switch_servo_task(void *args);