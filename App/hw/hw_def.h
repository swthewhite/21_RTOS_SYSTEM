#ifndef HW_DEF_H
#define HW_DEF_H


#include "main.h"
#include "def.h"
#include "cmsis_os.h"


#define _USE_HW_FLASH
#define _USE_HW_LOG


#define _USE_HW_LED
#define      HW_LED_MAX_CH          3

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          2
#define      HW_I2C_CH_OLED         _DEF_I2C1

#define _USE_HW_LCD
#define _USE_HW_SSD1306
#define      HW_LCD_WIDTH           128
#define      HW_LCD_HEIGHT          32

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       4

#define _USE_HW_WS2812
#define      HW_WS2812_MAX_CH       2


void logPrintf(const char *fmt, ...);
void delay(uint32_t ms);
uint32_t millis(void);

#endif