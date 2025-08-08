#include "ap.h"
#include "cli/cli_thread.h"
#include "event/event.h"




void apInit(void)
{
  for (int i = 0; i < 32; i += 1)
  {
    lcdClearBuffer(black);
    lcdPrintfResize(0, 40 - i, green, 16, "  -- BARAM --");
    lcdDrawRect(0, 0, LCD_WIDTH, LCD_HEIGHT, white);
    lcdUpdateDraw();
    delay(10);
  }
  delay(500);
  lcdClear(black);    

  cliThreadInit();
}

void apMain(void)
{
  uint32_t pre_time;  
  uint8_t button_pre = 0;
  uint8_t button_cur = 0;

  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);

      gpioPinWrite(0, _DEF_HIGH);
    }
    delay(1);

    button_cur = buttonGetPressed(_DEF_BUTTON1);
    if (button_cur != button_pre)
    {
      event_msg_t msg;

      msg.id = EVENT_ID_REQ_START;
      eventPut(&msg);
    }
    button_pre = button_cur;

    if (buttonGetPressed(_DEF_BUTTON4))
    {
      event_msg_t msg;

      msg.id = EVENT_ID_ERROR;
      eventPut(&msg);      
    }
  }
}