#include "event.h"

#define EVENT_FUNC_MAX        16



extern osMessageQueueId_t event_queueHandle;

static uint16_t event_func_cnt = 0;
static void (*event_func[EVENT_FUNC_MAX])(event_msg_t *p_msg);
static uint16_t event_id_tbl[EVENT_FUNC_MAX];




bool eventInit(void)
{  
  return true;
}

bool eventAddCallback(uint16_t event_id, void (*p_func)(event_msg_t *p_msg))
{
  bool ret = false;

  if (event_func_cnt < EVENT_FUNC_MAX)
  {
    taskENTER_CRITICAL();
    event_id_tbl[event_func_cnt] = event_id;
    event_func[event_func_cnt] = p_func;
    event_func_cnt++;
    taskEXIT_CRITICAL();
  }
  return ret;
}

bool eventPut(event_msg_t *p_msg)
{
  osStatus_t os_ret;

  os_ret = osMessageQueuePut(event_queueHandle, p_msg, 0U, osWaitForever);  

  return os_ret == osOK ? true:false;
}

void eventThread(void *argument)
{
  logPrintf("[  ] eventThread()\n");
  eventInit();

  while(1)
  {
    osStatus_t ret;
    event_msg_t msg;

    ret = osMessageQueueGet(event_queueHandle, &msg, NULL, osWaitForever);
    if (ret == osOK)
    {
      #if 0
      // 메세지를 받았을때 로그를 출력한다.
      //
      logPrintf("Msg Received\n");
      logPrintf("    ID : %d\n", msg.id);
      logPrintf("    DATA : %d\n", msg.data);
      #endif

      #if 1
      for (int i=0; i<event_func_cnt; i++)
      {
        if (event_func[i] != NULL)
        {
          if (event_id_tbl[i] == EVENT_ID_ALL)
          {
            event_func[i](&msg);
          }
          else
          {
            for (int id_idx=0; id_idx<EVENT_ID_MAX; id_idx++)
            {
              if (event_id_tbl[i] == msg.id)              
              {
                event_func[i](&msg);
                break;
              }
            }
          }
        }
      }
      #endif
    }    
  }
}