#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "leds.h"
#include "canhandler.h"
#include "uudt_handler.h"
#include "asdt_handler.h"

// The only public function is the init, as it starts the CANBus controller,
// ISR and an RTOS task process incoming framess.
void task_frame_handler_init();
