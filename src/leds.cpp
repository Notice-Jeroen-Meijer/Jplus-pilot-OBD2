#include "leds.h"

static CS_CONFIG_t *leds_config;

void leds_init () {
  leds_config = getConfig ();
  if (leds_config->mode_leds == 0) {
     return;
  } else {
    // setup LED's
    pinMode(LED, OUTPUT);
    digitalWrite(LED,  LED_OFF);

    // startup sequence and RED (=power) on
    led_set(LED, true); delay(200); led_set(LED, false);
  }
}

void led_set (unsigned led, bool on) {
  static bool singleLedState = false;
  if (leds_config->mode_leds == 0) {
    return;
  } else {
    // single LED compleely ignores absolute LED state, just switches
  digitalWrite(LED_BUILTIN, (singleLedState = !singleLedState) ? LED_ON : LED_OFF);
  }
}
