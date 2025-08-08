#include "button.h"



#ifdef _USE_HW_BUTTON
#include "cli.h"


typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;      
} button_t;


static void cliButton(cli_args_t *args);

static const button_t button_tbl[BUTTON_MAX_CH] = 
{
  {GPIOG, GPIO_PIN_2, _DEF_LOW},
  {GPIOG, GPIO_PIN_3, _DEF_LOW},
  {GPIOG, GPIO_PIN_4, _DEF_LOW},
  {GPIOG, GPIO_PIN_5, _DEF_LOW},
};

static bool is_init = false;





bool buttonInit(void)
{
  is_init = true;

  cliPrintf("[OK] buttonInit()\n");

  cliAdd("button", cliButton);
  return true;
}

bool buttonGetPressed(uint8_t ch)
{
  bool ret = false;


  if (ch >= BUTTON_MAX_CH)
    return false;

  if (HAL_GPIO_ReadPin(button_tbl[ch].port, button_tbl[ch].pin) == button_tbl[ch].on_state)
  {
    ret = true;
  }
  
  return ret;
}

void cliButton(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 1 && args->isStr(0, "info"))
  {
    cliPrintf("is_init : %d\n", is_init);
    cliPrintf("btn max : %d\n", BUTTON_MAX_CH);
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "show"))
  {
    while(cliKeepLoop())
    {
      for (int i=0; i<BUTTON_MAX_CH; i++)
      {
        cliPrintf("%d", buttonGetPressed(i));
      }
      cliPrintf("\n");
      delay(100);
    }
    ret = true;
  }

  if (!ret)
  {
    cliPrintf("button info\n");
    cliPrintf("button show\n");
  }
}
#endif