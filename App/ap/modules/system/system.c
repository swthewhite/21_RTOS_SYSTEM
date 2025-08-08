#include "system.h"
#include "event/event.h"
#include "display/neopixel/neopixel.h"

enum
{
  SYSTEM_STATE_INIT,
  SYSTEM_STATE_IDLE,
  SYSTEM_STATE_WORK,
  SYSTEM_STATE_ERROR
};

static uint8_t state = SYSTEM_STATE_INIT;
static uint8_t state_req = SYSTEM_STATE_INIT;




bool systemInit(void)
{  
  return true;
}

void systemEvent(event_msg_t *p_msg)
{
  switch(p_msg->id)
  {
    case EVENT_ID_ERROR:
      state_req = SYSTEM_STATE_ERROR;
      break;

    case EVENT_ID_REQ_START:
      state_req = SYSTEM_STATE_WORK;
      break;
  }
}

void systemThread(void *argument)
{
  event_msg_t msg;


  logPrintf("[  ] systemThread()\n");

  systemInit();
  eventAddCallback(EVENT_ID_ALL, systemEvent);

  while(1)
  {
    bool is_changed = false;


    if (state_req != state)
    {
      logPrintf("[  ] system %d->%d\n", state, state_req);
      state = state_req;
      is_changed = true;
    }

    switch(state)
    {
      case SYSTEM_STATE_INIT:        
        state_req = SYSTEM_STATE_IDLE;
        break;

      case SYSTEM_STATE_IDLE: 
        if (is_changed)
        {
          neopixelSetMode(NEO_BREATH, WS2812_COLOR_RED, 1000);
        }
        break;

      case SYSTEM_STATE_WORK:
        if (is_changed)
        {
          msg.id = EVENT_ID_SYSTEM_WORK;
          eventPut(&msg);
          state_req = SYSTEM_STATE_WORK;

          neopixelSetMode(NEO_BREATH, WS2812_COLOR_GREEN, 1000);
        }
        break;
        
      case SYSTEM_STATE_ERROR:
        if (is_changed)
        {
          neopixelSetMode(NEO_BLINK, WS2812_COLOR_RED, 200);
        }
        break;
    }
    delay(10);
  }
}