#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "leds.h"
#include "canhandler.h"
#include "freeframehandler.h"
#include "isotphandler.h"

// The only public function is the init, as it starts the CANBus controller,
// ISR and an RTOS task process incoming framess.
void init();
