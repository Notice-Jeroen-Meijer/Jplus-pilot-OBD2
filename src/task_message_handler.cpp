#include "task_message_handler.h"

QueueHandle_t messageQueue; // feeding processes declare this external
static TaskHandle_t messageHandle = NULL;

void process_message(MESSAGE_t msg)
{
}

// this is the main task
void task_message_core(void *parameters)
{
    MESSAGE_t msg;

    // endless running task
    while (1)
    {
        // pick up data from the queue (which is fed by the freeframe and the
        // isotp handlers. This task will go in the blocked state (read: other
        // tasks will get slices) when no data is available and will become
        // runnable where there is data
        if (xQueueReceive(messageQueue, &msg, portMAX_DELAY) == pdTRUE)
        {
            process_message(msg);
        }
    }
}

void task_message_handler_init()
{
    // create a generic RTOS queue for message handling, with 10 positions
    // this queue is to pass on message to the data logger
    messageQueue = xQueueCreate(10, sizeof(MESSAGE_t));
    if (messageQueue == 0)
    {
        Serial.println("Can't create Message buffer. Stopping");
        while (1)
            ;
    }

    // create the task to handle all incoming frames
    xTaskCreatePinnedToCore(
        task_message_core,
        "message handler",
        1024,
        NULL,
        1,
        &messageHandle,
        app_cpu);
}