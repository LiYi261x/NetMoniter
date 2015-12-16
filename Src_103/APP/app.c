#include "ucos_ii.h"
#include "includes.h"
//#include "analysis_function.h"
#include "UART.h"
#include "key.h" 
#include "Translate.h"
#include "IWDG.h"
#include "SPI.h"

#define UATx_Length					1			//�����Ҫ����ȥ�鿴

extern uint8_t isAlarm;
extern uint8_t armState;
extern Music_Info Music_Information;
extern uint8_t Music_flag;

volatile uint8_t Res_Flag,Deal_Flag;
uint8_t APP_Rx[60];

char UART_Tx[UATx_Length] = {0};//DMA���͵�����
u8 Flag_Uart_Send ;
///--��Ϣ���еĶ��岿��---
OS_EVENT *QSem;//������Ϣ����ָ��
void *MsgQeueTb[MSG_QUEUE_TABNUM];//������Ϣָ�����飬���г���Ϊ20
OS_MEM   *PartitionPt;//�����ڴ����ָ��
uint8_t  Partition[MSG_QUEUE_TABNUM][400];


void Task_Translate(void *p_arg)
{
	uint8_t *s; 
	INT8U err;
//  OS_MEM_DATA MemInfo;

	 while (1)
	 {
      s = OSQPend(QSem,0,&err);
      if(err==OS_NO_ERR)
      {
         translate_(s);
      }      
      
		   	OSTimeDly(10);
   }
}





void Task_KEY(void *p_arg)
{
	u8 i=0;
	//u32 Key_L=0,Key_L1=0,Key_L2=0,Key_L3=0,Key_L4=0,Key_L5=0;	
		u32 curKeyValue = NO_KEY_PRESS;           	//��ǰ�����ļ�ֵ
	 static u32 keyValueBackup = NO_KEY_PRESS;		//��ֵ�ı���
	 static unsigned char curStatus = KEY_STAT_noKeyPress; // ��ǰ��״̬	
  
	while (1)
	{
 		OSTimeDlyHMSM(0, 0,0,5);
	 	curKeyValue = Get_KeyValue();
  
		switch(curStatus) 
		{
				case KEY_STAT_noKeyPress: /* -----�ް������µ�״̬----- */
						   if( curKeyValue != NO_KEY_PRESS ) 
						   {
           if(++i==2)
           {
             keyValueBackup = curKeyValue;
             curStatus = KEY_STAT_filter; 
             i=0;
           }									
						   }
//						   else
//						   {
//           
//						   }
         break;
				case KEY_STAT_filter:			/* -----����״̬----- */		
							  if( curKeyValue == keyValueBackup )
							  {
             UART_Tx[0] &= 0;//printf("%d",keyValueBackup);
             switch(keyValueBackup)
             {							 
                case 0x00001:UART_Tx[0] |=14;Flag_Uart_Send = 1;break;// ����
                case 0x00002:UART_Tx[0] |=18;Flag_Uart_Send = 1;break;//��  
                case 0x00004:UART_Tx[0] |=19;Flag_Uart_Send = 1;break;//��    
                case 0x00008:UART_Tx[0] |=0 ;/*Flag_Uart_Send = 1;*/break;//ȱʡ
                case 0x00010:UART_Tx[0] |=8 ;Flag_Uart_Send = 1;break;//����  
                case 0x00020:UART_Tx[0] |=17;Flag_Uart_Send = 1;break;//��  
                case 0x00040:UART_Tx[0] |=0;Flag_Uart_Send = 1;break;//ȱʡ
                case 0x00080:UART_Tx[0] |=13;Flag_Uart_Send = 1;break;//OK 
                case 0x00100:UART_Tx[0] |=11;Flag_Uart_Send = 1;break;//��ؿ� 
                case 0x00200:UART_Tx[0] |=6 ;Flag_Uart_Send = 1;break;//�Ŵ�  
                case 0x00400:UART_Tx[0] |=5 ;Flag_Uart_Send = 1;break;//�˵�	
                case 0x00800:UART_Tx[0] |=16;Flag_Uart_Send = 1;break;//��
                case 0x01000:UART_Tx[0] |=12;Flag_Uart_Send = 1;break;//��ع�
                case 0x02000:UART_Tx[0] |=7 ;Flag_Uart_Send = 1;break;//��С
                case 0x04000:UART_Tx[0] |=31;Flag_Uart_Send = 1;break;//������ 
                case 0x08000:UART_Tx[0] |=28;Flag_Uart_Send = 1;break;//������						
                case 0x10000:UART_Tx[0] |=21;Flag_Uart_Send = 1;break;//����
                case 0x20000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//ȱʡ
                case 0x40000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//ȱʡ
                case 0x80000:UART_Tx[0] |=0 ;Flag_Uart_Send = 1;break;//ȱʡ								
                default :break;
             }
             
             if(Flag_Uart_Send)
             {                                    
               Putc_UART1(UART_Tx[0]);
             }
           curStatus = KEY_STAT_clicking; 
							  }						 
					  	 else
						   {
							 //returnKeyValue = NO_KEY_PRESS; 
						   }
         break;//���ؼ�ֵ	
		 	case KEY_STAT_clicking:/* -----����״̬----- */	 
						   if( curKeyValue == NO_KEY_PRESS )	//�жϰ����Ƿ�̧��������밴���ͷţ����򱣳�
								 curStatus = KEY_STAT_Release; 
						   else //if( ++keyPressTimer > LONG_PRESS_TIME )			
						   { 
							   curStatus = KEY_STAT_clicking; 
						   } 
         break;	
				case KEY_STAT_Release:/* -----�����ͷ�״̬----- */						 

         keyValueBackup = NO_KEY_PRESS;
         if(Flag_Uart_Send){UART_Tx[0]|=0x80;Putc_UART1(UART_Tx[0]);Flag_Uart_Send=0;}
           curStatus = KEY_STAT_noKeyPress;			
         break;	
				default:
    /// State Err!
      if(curStatus > KEY_STAT_Release)
      {
         keyValueBackup  = NO_KEY_PRESS;
         curStatus       = NO_KEY_PRESS;
         Flag_Uart_Send  = 0;
      }
      break;
			}						
	}
}


