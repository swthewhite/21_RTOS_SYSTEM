#include "log.h"


#ifdef _USE_HW_LOG
#include "uart.h"

static char log_buf[256];

extern osMutexId_t log_mutexHandle;





void logPrintf(const char *fmt, ...)
{
  va_list args;
  int len;

  va_start(args, fmt);

  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    osMutexAcquire(log_mutexHandle, osWaitForever);
  }
  len = vsnprintf(log_buf, 256, fmt, args);

  uartWrite(_DEF_UART1, (uint8_t *)log_buf, len);

  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    osMutexRelease(log_mutexHandle);
  }

  va_end(args);
}

#endif