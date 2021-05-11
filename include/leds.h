#ifndef _LEDS_H_
#define _LEDS_H_

#include <Arduino.h>

#include "config.h"

#define LED 2
#define LED_ON HIGH   // Active HIGH
#define LED_OFF LOW

void leds_init();
void led_set(unsigned led, bool on);

#endif
