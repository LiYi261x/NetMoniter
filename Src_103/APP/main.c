#include "includes.h"
#include "app.h"
#include "UART.h"
//uint8_t flag_translate=0;

OS_STK task_translate_stk[TASK_TRANSLATE_STK_SIZE];
OS_STK task_key_stk[TASK_KEY_STK_SIZE];

int main(void)
{
	INT8U err;
	u32 i;
	BSP_Init();
	
	OSInit();
	
	QSem = OSQCreate(&MsgQeueTb[0],MSG_QUEUE_TABNUM); //������Ϣ���У�10����Ϣ
	PartitionPt=OSMemCreate(Partition,MSG_QUEUE_TABNUM,400,&err);
	
  OSTaskCreate(Task_Translate,(void *)0, &task_translate_stk[TASK_TRANSLATE_STK_SIZE-1], TASK_TRANSLATE_PRIO);
  OSTaskCreate(Task_KEY,(void *)0, &task_key_stk[TASK_KEY_STK_SIZE-1], TASK_KEY_PRIO);

	OSStart();
	
	return 0;
}
