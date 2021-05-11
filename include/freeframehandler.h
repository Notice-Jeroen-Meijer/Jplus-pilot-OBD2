#ifndef _FREEFRAMEHANDLER_H_
#define _FREEFRAMEHANDLER_H_

#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "config.h"
#include "canhandler.h"
#include "message.h"
#include "utils.h"

// A freeframe is the same as an UUDT frame. UUDT frames are generally sent
// spontaniously by connected ECUs, and other ECUs can decide to act on the
// information provided.

// Note that in CANbus there are no "source" and "destination" addresses
// though these might be defined by higher layer protocols. A frame simply
// has an ID and any ECU can decide to process or ignore that ID.

// Free frames timer storage *************************************************
// Free frames timers are stored in an array for all 0x700 free frames. The
// timer is a very simple mechanism to throttle incoming frames to avoid
// overloading processing tasks further downstream. The meaning of the timer
// value for each ID is:
// 0x00: no timer, pass on every incoming frame
// 0x01: pass the next incoming frame, then reset the timer to 0xff
// 0x02-0xfe: do not pass the next incoming frame. A ticker will decrement
//     the age to 0x01
//     note: that this is NOT implemented. All timing is handled by the queue
//           manager
// 0xff: do not pass the next incoming frame
// All timers are initialized to 0xff meaning no free frame will pass
// If a frame ID is to be passed as fast as they come in, the value
// should simply be set to 0x00.
// If it is to be passed once the next time it is seen, the value is to
// be set to 0x01

#define FREEFRAMEARRAYSIZE 0x700
typedef struct {
  uint8_t age;
} FREEFRAME_t;

extern QueueHandle_t messageQueue;

void freeframe_init ();
void storeFreeframe (CAN_frame_t &frame, uint8_t bus);
void ageFreeFrame ();
FREEFRAME_t *getFreeframe (uint32_t id, uint8_t bus);
void requestFreeframe  (uint32_t id, uint8_t bus);
String bufferedFrameToString (uint32_t id, uint8_t bus);
void processUUDTframe (CAN_frame_t &frame);

#endif
