#include <stdint.h>


typedef struct 
{
  uint16_t position;
  uint16_t speed;
  uint16_t load;
  uint8_t voltage_V;
  uint16_t current_mA;
} servo_state_t;