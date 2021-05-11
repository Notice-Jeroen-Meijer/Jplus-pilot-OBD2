#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "config.h"

typedef struct {
  uint32_t id = 0xffffffff;                        // the from-address of the responding device
  uint16_t length = 0;                             // size, max 4096
  uint8_t data[4096];                              // max ISOTP multiframe message
} MESSAGE_t;

#endif