#pragma once

#include <stdbool.h>
#include <ctype.h>

static float bound_max_speed(uint16_t max_speed) {
    /**
     * Bound the max speed to be between 0 and 100
     * 
     * @param max_speed The max speed
     * 
     * @return The bounded max speed
    */

    uint16_t bounded_max_speed = 0;

    if (max_speed < 0) {
        bounded_max_speed = 0;
    } else if (max_speed > 3073) {
        bounded_max_speed = 3073;
    } else {
        bounded_max_speed = max_speed;
    }
    return bounded_max_speed;
}