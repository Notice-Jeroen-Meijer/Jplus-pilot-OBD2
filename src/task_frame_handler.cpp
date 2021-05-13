#include "task_frame_handler.h"
/*
    In this task, we poll the incoming can frame buffer and process it if there is
    a frame available
*/

// Task handle
static TaskHandle_t frameHandler = NULL;

// other global variables
uint32_t canFrameCounter = 1;

void process_frame(CAN_frame_t &frame)
{
    led_set(LED, true);
    if (frame.MsgID < 0x700)
    { // free data is < 0x700. It is converted to a CAN_message and send off
        uudt_process_frame(frame);
    }
    else
    { // iso-tp data
        asdt_process_frame(frame);
    }
    led_set(LED, false);
}

// this is the main task
void task_frame_core(void *parameters)
{

    CAN_frame_t rx_frame;

    // endless running task
    while (1)
    {
        // pick up data from the queue (which is fed by the CANbus hardware
        // through ISR in can.c). This task will go in the blocked state
        // (read: other tasks will get slices) when no data is available
        // and will become runnable where there is data
        if (can_receive_blocked(&rx_frame))
        {
            process_frame(rx_frame);
            canFrameCounter++;
        }
    }
}

void task_frame_handler_init()
{
    // initialize the frame type handlers, because "process" will start
    // pushing frames on these handlers
    uudt_init();
    asdt_init();

    // initialize the CANbus and the receive queue
    can_init();

    // create the task to handle all incoming frames
    xTaskCreatePinnedToCore(
        task_frame_core,
        "frame handler",
        1024,
        NULL,
        1,
        &frameHandler,
        app_cpu);
}