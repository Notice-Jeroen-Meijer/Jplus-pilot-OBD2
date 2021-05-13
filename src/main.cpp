#include "Arduino.h"

#define VERSION "0.0.1"

#define SERIAL_BPS 115200

#include "config.h"
#include "leds.h"
#include "bluetoothhandler.h"
#include "task_message_handler.h"
#include "task_frame_handler.h"
#include "utils.h"

// Config ********************************************************************
CS_CONFIG_t *cansee_config;

void setup()
{
    Serial.begin(SERIAL_BPS); // init serial
    Serial.println("");
    Serial.println("");
    Serial.print("J+ pilot OBD2 starting...");

    delay(500); // give user chance to press BUT
    pinMode(0, INPUT);
    cansee_config = config_get(); // invalid config will reset too
    if (!digitalRead(0))
    { // if pressed
        Serial.println("Reset config...");
        config_write_to_eeprom(true);
    }

    if (cansee_config->mode_debug)
    {
        Serial.print("Version:   ");
        Serial.println(VERSION);
        Serial.println("Serial:    " + getHex(cansee_config->mode_serial));
        Serial.println("Bluetooth: " + getHex(cansee_config->mode_bluetooth));
        Serial.println("Leds:      " + getHex(cansee_config->mode_leds));
        Serial.println("Debug:     " + getHex(cansee_config->mode_debug));
        Serial.println("CANbus0:   " + getHex(cansee_config->can0_speed / 25) + getHex(cansee_config->can0_rx) + getHex(cansee_config->can0_tx));
        Serial.println("Boot count:" + getHex(cansee_config->boot_count));
    }
    cansee_config->boot_count++;
    config_write_to_eeprom(false);

    leds_init();

    // initialize the message queue
    /* TODO */
    // we might want to move / expand this to a more complete task that handles the messages
    task_message_handler_init();

    // initialize th CANbus and the task to handle incoming CANbus frames
    task_frame_handler_init();

    /* TODO */
    // initialize other components
}

void loop() {
    led_set(LED, true);
    vTaskDelay(1000);
    led_set(LED, false);
    vTaskDelay(1000);
}