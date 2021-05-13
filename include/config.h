#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <EEPROM.h>

#define CONFIG_MAGIC_NUMBER 0xd0020bd2 // juice+ obd2 => doozobd2 => d0020bd2

#define BLUETOOTH_OFF 0x00
#define BLUETOOTH_CLASSIC 0x01
#define BLUETOOTH_BLE 0x02

#define DEBUG_COMMAND_FF 0x08     //     unused
#define DEBUG_COMMAND 0x04        // (*) debug all command activity
#define DEBUG_NOTUSED 0x02        // (*) unused
#define DEBUG_BUS_RECEIVE_FF 0x01 //     debug all incoming freeframes
#define DEBUG_COMMAND_ISO 0x10    // (*) debug all incoming and outgoing ISOTP-frames

#define LED_SINGLE 1
#define LED_MULTI 2 // actually, anything but 0 and 1, but 2 is defined

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Config ********************************************************************
// structure that defines the firmware's configuration
typedef struct
{
    uint32_t magicnumber; // does that read CanSee?
    byte version;
    byte mode_serial;
    byte mode_bluetooth;
    byte mode_debug;
    byte mode_leds;
    char name_bluetooth[32];
    char pin_bluetooth[8];
    byte can0_rx;
    byte can0_tx;
    uint16_t can0_speed;
    //** past this line will NOT be initialized by EEPROM but it global data
    void (*command_handler)();        // command handler
    void (*output_handler)(String o); // output handler
    uint16_t boot_count;
} CS_CONFIG_t;

CS_CONFIG_t *config_get();
void config_write_to_eeprom(bool reset);

#endif
