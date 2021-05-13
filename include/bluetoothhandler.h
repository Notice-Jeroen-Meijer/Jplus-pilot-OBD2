#ifndef _BLUETOOTHHANDLER_H_
#define _BLUETOOTHHANDLER_H_

#include <Arduino.h>
#include <BluetoothSerial.h>

#include "config.h"
#include "leds.h"

// Repository included libraries includes, see ./lib/ ************************

void bluetooth_init ();
void bluetooth_write_outgoing (String o);
void bluetooth_read_incoming (String &readBuffer);
void bluetooth_set_active (bool active);

#endif
