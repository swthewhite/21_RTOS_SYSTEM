#include "ws2812.h"



#ifdef _USE_HW_WS2812
#include "cli.h"

#define BIT_PERIOD      (218) // 1300ns, 168Mhz
#define BIT_HIGH        (118) // 700ns
#define BIT_LOW         (59)  // 350ns
#define BIT_ZERO        (50)

#define BIT_LEN         16


bool is_init = false;


typedef struct
{
  TIM_HandleTypeDef *p_htim;  
  uint32_t channel;
  uint16_t led_cnt;
} ws2812_t;

extern TIM_HandleTypeDef htim8;

static uint16_t bit_buf[BIT_ZERO + 24*(HW_WS2812_MAX_CH+1)];


ws2812_t ws2812;

#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *args);
#endif
static bool ws2812InitHw(void);





bool ws2812Init(void)
{


  memset(bit_buf, 0, sizeof(bit_buf));

  ws2812.p_htim  = &htim8;
  ws2812.channel = TIM_CHANNEL_2;

  ws2812InitHw();

  ws2812.led_cnt = WS2812_MAX_CH;
  is_init = true;

  for (int i=0; i<WS2812_MAX_CH; i++)
  {
    ws2812SetColor(i, WS2812_COLOR_OFF);
  }
  ws2812Refresh();

#ifdef _USE_HW_CLI
  cliAdd("ws2812", cliCmd);
#endif
  return true;
}

bool ws2812InitHw(void)
{
  return true;
}

bool ws2812Refresh(void)
{
  HAL_TIM_PWM_Stop_DMA(ws2812.p_htim, ws2812.channel);
  HAL_TIM_PWM_Start_DMA(ws2812.p_htim, ws2812.channel,  (const uint32_t *)bit_buf, sizeof(bit_buf)/(sizeof(bit_buf[0])));
  return true;
}

void ws2812SetColor(uint32_t ch, uint32_t color)
{
  uint16_t r_bit[8];
  uint16_t g_bit[8];
  uint16_t b_bit[8];
  uint16_t red;
  uint16_t green;
  uint16_t blue;
  uint32_t offset;

  if (ch >= WS2812_MAX_CH)
    return;

  red   = (color >> 16) & 0xFF;
  green = (color >> 8) & 0xFF;
  blue  = (color >> 0) & 0xFF;


  for (int i=0; i<8; i++)
  {
    if (red & (1<<7))
    {
      r_bit[i] = BIT_HIGH;
    }
    else
    {
      r_bit[i] = BIT_LOW;
    }
    red <<= 1;

    if (green & (1<<7))
    {
      g_bit[i] = BIT_HIGH;
    }
    else
    {
      g_bit[i] = BIT_LOW;
    }
    green <<= 1;

    if (blue & (1<<7))
    {
      b_bit[i] = BIT_HIGH;
    }
    else
    {
      b_bit[i] = BIT_LOW;
    }
    blue <<= 1;
  }

  offset = BIT_ZERO;

  memcpy(&bit_buf[offset + ch*24 + 8*0], g_bit, BIT_LEN);
  memcpy(&bit_buf[offset + ch*24 + 8*1], r_bit, BIT_LEN);
  memcpy(&bit_buf[offset + ch*24 + 8*2], b_bit, BIT_LEN);
}

#ifdef _USE_HW_CLI
void cliCmd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    cliPrintf("ws2812 led cnt : %d\n", WS2812_MAX_CH);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "test"))
  {
    uint32_t color[6] = {WS2812_COLOR_RED,
                         WS2812_COLOR_OFF,
                         WS2812_COLOR_GREEN,
                         WS2812_COLOR_OFF,
                         WS2812_COLOR_BLUE,
                         WS2812_COLOR_OFF};

    uint8_t color_idx = 0;

    while(cliKeepLoop())
    {
      ws2812SetColor(0, color[color_idx]);
      ws2812Refresh();
      color_idx = (color_idx + 1) % 6;

      delay(500);
    }
    ret = true;
  }


  if (args->argc == 5 && args->isStr(0, "color"))
  {
    uint8_t  ch;
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    ch    = (uint8_t)args->getData(1);
    red   = (uint8_t)args->getData(2);
    green = (uint8_t)args->getData(3);
    blue  = (uint8_t)args->getData(4);

    ws2812SetColor(ch, WS2812_COLOR(red, green, blue));
    ws2812Refresh();

    while(cliKeepLoop())
    {
    }
    ws2812SetColor(ch, 0);
    ws2812Refresh();
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("ws2812 info\n");
    cliPrintf("ws2812 test\n");
    cliPrintf("ws2812 color ch r g b\n");
  }
}
#endif

#endif