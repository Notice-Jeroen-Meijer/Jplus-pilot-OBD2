#include "uudt_handler.h"

static CS_CONFIG_t *uudt_config;
static FREEFRAME_t uudts[FREEFRAMEARRAYSIZE];

void uudt_init()
{
    uudt_config = config_get();
    // clear the free frame timer
    for (int id = 0; id < FREEFRAMEARRAYSIZE; id++)
    {
        // allow 25-30 seconds after boot to display empty frames
        uudts[id].age = 0xff;
    }
}

void uudt_process_frame(CAN_frame_t &frame)
{
    uint32_t id = frame.MsgID;
    uint16_t age = uudts[id].age;
    MESSAGE_t msg;

    // if the age is 0xff ignore the frame
    if (age == 0xff)
        return;

    // convert the frame to a message
    if (age <= 1)
    {
        msg.id = frame.MsgID;
        msg.length = frame.FIR.B.DLC;
        for (int i = 0; i < frame.FIR.B.DLC; i++)
        {
            msg.data[i] = frame.data.u8[i];
        }

        // queue it. If the message queue does not exist or is full, dispose the
        // message
        if (messageQueue)
            xQueueSend(messageQueue, &msg, 0);
    }

    // if the age is 0 we are done
    if (age == 0)
    {
        return;
    }

    // reset age to never
    else if (age == 1)
        uudts[id].age = 0xff;

    // do nothing
}

// note that optionally an ager can be implemented here
// forall age in ages
//   if age > 1 & age < 0xff
//     age--;
// if so, it would be important to protect the uudts with a mutex

// convert a frame to readable hex output format
String uudt_to_string(CAN_frame_t &frame)
{
    String dataString = String(frame.MsgID, HEX) + ",";
    for (int i = 0; i < frame.FIR.B.DLC; i++)
        dataString += getHex(frame.data.u8[i]);
    dataString += "\n";
    return dataString;
}
