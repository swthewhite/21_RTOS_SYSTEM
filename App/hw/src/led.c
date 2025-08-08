#include "led.h"


#ifdef _USE_HW_LED



typedef struct 
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;

static led_tbl_t led_tbl[LED_MAX_CH] =
{
  {GPIOF, GPIO_PIN_12, GPIO_PIN_RESET, GPIO_PIN_SET},
  {GPIOF, GPIO_PIN_13, GPIO_PIN_RESET, GPIO_PIN_SET},
  {GPIOF, GPIO_PIN_14, GPIO_PIN_RESET, GPIO_PIN_SET},
};


bool ledInit(void)
{

  for (int i=0; i<LED_MAX_CH; i++)
  {
    ledOff(i);
  }

  return true;
}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}
#endif