#include "message.h"

QueueHandle_t messageQueue;

void message_init () {
  // create a generic RTOS queue for message handling, with 10 positions
  // this queue is to pass on message to the data logger
  messageQueue = xQueueCreate(10, sizeof(MESSAGE_t));
  if (messageQueue == 0) {
    Serial.println("Can't create Message buffer. Stopping");
    while (1);
  }
}  