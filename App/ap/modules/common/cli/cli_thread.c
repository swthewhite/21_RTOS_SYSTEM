#include "cli_thread.h"




bool cliThreadInit(void)
{
  cliOpen(_DEF_UART1, 115200);
  
  return true;
}

void cliThread(void *argument)
{
  logPrintf("[  ] cliThread()\n");

  delay(100);
  cliLogo();
  
  while(1)
  {
    cliMain();
    delay(1);
  }
}