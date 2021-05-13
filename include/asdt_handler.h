#ifndef _ASDTHANDLER_H_
#define _ASDTHANDLER_H_

#include <Arduino.h>

#include "config.h"
#include "canhandler.h"
#include "task_message_handler.h"
#include "leds.h"
#include "utils.h"
#include "zoe.h"

#define DEBUG_COMMAND 0x04
#define DEBUG_BUS_RECEIVE_ISO 0x02

extern QueueHandle_t messageQueue;

// see https://en.wikipedia.org/wiki/ISO_15765-2 for ISO-TP

// ISO-TP message ************************************************************
typedef struct
{
  uint32_t id = 0xffffffff;     // the from-address of the responding device
  uint16_t length = 0;          // size, max 4096
  uint16_t index = 0;           // pointer
  uint8_t next = 1;             // sequence of next frame
  uint8_t data[4096];           // max ISOTP multiframe message
  unsigned long flow_delay = 0; // delay between outgoing isoMessageToString
  uint8_t flow_counter = 0;     // frames to send (until new flow control)
  uint8_t flow_active = 0;      // 0=no, 1=yes
} ISO_MESSAGE_t;

void asdt_init();
void asdt_process_frame(CAN_frame_t &frame);
void asdt_request(uint32_t id, int16_t length, uint8_t *request, uint8_t bus);
String asdt_to_string(ISO_MESSAGE_t &message);

#endif
