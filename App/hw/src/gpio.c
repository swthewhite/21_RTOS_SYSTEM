#include "gpio.h"
#include "cli.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState init_state;
} gpio_tbl_t;

static gpio_tbl_t gpio_tbl[GPIO_MAX_CH] = {
    {GPIOE, GPIO_PIN_1, GPIO_PIN_RESET},
    {GPIOE, GPIO_PIN_2, GPIO_PIN_RESET},
    {GPIOE, GPIO_PIN_5, GPIO_PIN_RESET},
    {GPIOE, GPIO_PIN_6, GPIO_PIN_RESET},
};

static void cliGpio(cli_args_t *arg);

bool gpioInit(void)
{
    for (int i = 0; i < GPIO_MAX_CH; i++)
    {
        HAL_GPIO_WritePin(gpio_tbl[i].port, gpio_tbl[i].pin, gpio_tbl[i].init_state);
    }

    cliAdd("gpio", cliGpio);

    return true;
}
bool gpioPinWrite(uint8_t ch, uint8_t pin_state)
{
    HAL_GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, pin_state);

    return true;
}
uint8_t gpioPinRead(uint8_t ch)
{
    HAL_GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin);

    return 0;
}

void cliGpio(cli_args_t *arg)
{
    bool ret = false;

    if (arg->argc == 3 && arg->isStr(0, "write"))
    {
        uint8_t ch;
        uint8_t state;

        ch = arg->getData(1);
        state = arg->getData(2);

        cliPrintf("gpioPinWrite ch %d, state %d\n", ch, state);
        gpioPinWrite(ch, state);
    }

    if (!ret)
    {
        cliPrintf("gpio write [ch] [state]\n");
    }
}
