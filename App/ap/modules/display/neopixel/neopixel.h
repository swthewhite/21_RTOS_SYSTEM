#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_


#include "ap_def.h"


typedef enum
{
  NEO_OFF,
  NEO_ON,
  NEO_BLINK,
  NEO_BREATH
} NeopixelMode_t;


void neopixelSetMode(NeopixelMode_t mode, uint32_t color, uint32_t blink_time);


#endif