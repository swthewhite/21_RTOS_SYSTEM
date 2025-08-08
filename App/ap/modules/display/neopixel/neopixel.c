#include "neopixel.h"


static void cliCmd(cli_args_t *args);

static NeopixelMode_t neo_mode     = NEO_OFF;
static NeopixelMode_t neo_mode_req = NEO_OFF;
static NeopixelMode_t neo_mode_pre = NEO_OFF;
static uint32_t       neo_color    = WS2812_COLOR(0, 0, 0);
static uint32_t       neo_blink_time = 0;
static uint32_t       neo_blink_pre_time = 0;
static int32_t        neo_blink_cnt = 0;
static int32_t        neo_blink_step = 0;
static int32_t        neo_blink_dir = 1;



bool neopixelInit(void)
{
  cliAdd("neopixel", cliCmd);
  return true;
}

void neopixelSetMode(NeopixelMode_t mode, uint32_t color, uint32_t blink_time)
{
  neo_color = color;
  neo_blink_time = blink_time;
  neo_mode_req = mode;
}

void neopixelUpdate(void)
{
  bool is_changed = false;

  
  if (neo_mode_req != neo_mode)
  {
    logPrintf("[  ] neo mode %d->%d\n", neo_mode, neo_mode_req);
    neo_mode = neo_mode_req;
    is_changed = true;
  }

  switch(neo_mode)
  {
    case NEO_OFF:
      if (is_changed)
      {
        for (int i=0; i<WS2812_MAX_CH; i++)
        {
          ws2812SetColor(i, WS2812_COLOR_OFF);          
        }
        ws2812Refresh();
      }
      break;

    case NEO_ON:
      if (is_changed)
      {
        for (int i=0; i<WS2812_MAX_CH; i++)
        {
          ws2812SetColor(i, neo_color);          
        }
        ws2812Refresh();
      }
      break;

    case NEO_BLINK:
      if (is_changed)
      {        
        for (int i=0; i<WS2812_MAX_CH; i++)
        {
          ws2812SetColor(i, neo_color);          
        }
        ws2812Refresh();
        neo_blink_cnt = 0;
        neo_blink_pre_time = millis();
      }
      if (millis()-neo_blink_pre_time >= neo_blink_time)
      {
        neo_blink_pre_time = millis();
        neo_blink_cnt ^= 1;  

        uint32_t color;
        
        color = neo_blink_cnt & 1 ? WS2812_COLOR_OFF:neo_color;
        for (int i=0; i<WS2812_MAX_CH; i++)
        {
          ws2812SetColor(i, color);          
        }
        ws2812Refresh();        
        
      }
      break;

    case NEO_BREATH:
      if (is_changed)
      {        
        for (int i=0; i<WS2812_MAX_CH; i++)
        {
          ws2812SetColor(i, WS2812_COLOR_OFF);          
        }
        ws2812Refresh();
        neo_blink_step = 1;
        neo_blink_cnt = 0;
        neo_blink_dir = 1;
        neo_blink_pre_time = millis();
      }
      if (millis()-neo_blink_pre_time >= neo_blink_time)
      {
        neo_blink_pre_time = millis();
        neo_blink_dir *= -1;  
      }        
      
      neo_blink_cnt += neo_blink_step * neo_blink_dir;

      if (neo_blink_cnt > 400)
      {
        neo_blink_cnt = 400;
      }
      if (neo_blink_cnt < 2)
      {
        neo_blink_cnt = 2;
      }

      uint8_t r, g, b;
      
      r = neo_color >> 16;
      g = neo_color >> 8;
      b = neo_color >> 0;

      r = r * neo_blink_cnt / 1000;
      g = g * neo_blink_cnt / 1000;
      b = b * neo_blink_cnt / 1000;

      for (int i=0; i<WS2812_MAX_CH; i++)
      {
        ws2812SetColor(i, WS2812_COLOR(r, g, b));          
      }
      ws2812Refresh();        
      break;      

    default:
      break;
  }

  neo_mode_pre = neo_mode;
}

void neopixelThread(void *argument)
{
  neopixelInit();

  logPrintf("[  ] neopixelThread()\n");

  while(1)
  {
    neopixelUpdate();
    delay(10);
  }
}

void cliCmd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    cliPrintf("mode\n");
    cliPrintf("  NEO_OFF    : 0\n");
    cliPrintf("  NEO_ON     : 1\n");
    cliPrintf("  NEO_BLINK  : 2\n");
    cliPrintf("  NEO_BREATH : 3\n");
  
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "mode"))
  {
    uint8_t mode;
    uint16_t time_ms;

    mode = constrain(args->getData(1), NEO_OFF, NEO_BREATH);
    time_ms = args->getData(2);

    neopixelSetMode(mode, WS2812_COLOR_RED, time_ms);

    while(cliKeepLoop())
    {
      delay(10);
    }
    neopixelSetMode(NEO_OFF, WS2812_COLOR_GREEN, 0);

    ret = true;
  }
  

  if (!ret)
  {
    cliPrintf("neopixel info\n");
    cliPrintf("neopixel mode [mode] [time_ms]\n");
  }
}