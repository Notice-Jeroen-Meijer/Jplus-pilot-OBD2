#include "task_frame_handler.h"
/*
    In this task, we poll the incoming can frame buffer and process it if there is
    a frame available
*/

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Task handle
static TaskHandle_t frameHandler = NULL;

// other globat variables
uint32_t canFrameCounter = 1;


void processframe (CAN_frame_t &frame)
{
  led_set(LED, true);
  if (frame.MsgID < 0x700)
  { // free data is < 0x700. It is converted to a CAN_message and send off
    processUUDTframe(frame);
  }
  else
  { // iso-tp data 
    processASDTframe(frame);
  }
  led_set(LED, false);
}


// this is the main task
void process(void *parameters) {

    CAN_frame_t rx_frame;

    // endless running task
    while (1) {
        // pick up data from the queue (which is fed by the CANbus hardware
        // through ISR in can.c). This task will go in the blocked state
        // (read: other tasks will get slices) when no data is available
        // and will become runnable where there is data
        if (can_receive_blocked (&rx_frame)) {
            processframe(rx_frame);
            canFrameCounter++;
        }
    }
}


// create the task
void init() {
    xTaskCreatePinnedToCore (
        process,
        "frame handler",
        1024,
        NULL,
        1,
        &frameHandler,
        app_cpu
    );
}