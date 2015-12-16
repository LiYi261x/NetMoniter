#include <ucos_ii.h>
#include "stdio.h"
#include "uart.h"
#include "app.h"
#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_timer.h"
#include "Config.h"
#include "Setting.h"
#include "DMA.h"
#include "Check.h"
#include "sysConf.h"
#include "uart.h"
#include "SPI1.h"
#include "GUI.h"
#include "dlg.h"


//#ifndef test_test
//	#define test_test
//#endif

/*-------------------- Macro defines ---------------------*/
/* 定义任务优先级 */
#define UI_Task_PRIO             11
#define Insert_Task_PRIO         8
#define Refresh_Task_PRIO        9
#define Task_Stack_Use_PRIO      10  
/* 定义任务堆栈大小 */
#define USER_TASK_STACK_SIZE 2048
#define TOUCH_TASK_STACK_SIZE 256
#define KEY_TASK_STACK_SIZE 128
#define Task_Stack_Use_STACK_SIZE 128

/*------------------- static ----------------------------*/
/* 定义任务堆栈 */
static	OS_STK	UI_Task_Stack[USER_TASK_STACK_SIZE];


static	OS_STK	Insert_Task_Stack[TOUCH_TASK_STACK_SIZE];

static	OS_STK	Refresh_Task_Stack[KEY_TASK_STACK_SIZE];

//static  OS_STK_DATA UI_Task_Stack_Use;
//static  OS_STK_DATA Insert_Task_Stack_Use;
//static  OS_STK_DATA Refresh_Task_Stack_Use;



/*----------------- external function -------------------*/
void mntSetting_init(void);

/*----------------- Global   variables --------------------*/
///Insert , Refresh互斥信号量
int isKeyTrigged  = 0;

unsigned char isChecked  = 0;


int ReleasedDectSwitch  = 0;

OS_EVENT * Refresher;
OS_EVENT * Updater;

///--消息队列的定义部分---
OS_EVENT *QSem;//定义消息队列指针
void *MsgQeueTb[MSG_QUEUE_TABNUM];//定义消息指针数组，队列长度为10
OS_MEM   *PartitionPt;//定义内存分区指针
// #pragma arm section rwdata = "SD_RAM2", zidata = "SD_RAM2"
uint8_t  Partition[MSG_QUEUE_TABNUM][100];
// #pragma arm section rwdata
// uint8_t  Partition[20][300]__attribute__((at(0xA1FF0000)));
uint8_t myErr;
uint8_t myErr_2;
int list_endIndex  = -1;

///* ADDRESS: 0xAC000000  SIZE: 0x400000  */
#pragma arm section rwdata = "SD_RAM1", zidata = "SD_RAM1"
BERTH Berthes[BOAT_NUM_MAX];
SIMP_BERTH SimpBerthes[BOAT_NUM_MAX];
_boat_m24A boat_list_24A[BOAT_NUM_MAX];
_boat_m24A *boat_list_p24A[BOAT_NUM_MAX];


#pragma arm section rwdata


BERTH Berthes[BOAT_NUM_MAX]__attribute__((at(0xA1D00000)));
SIMP_BERTH SimpBerthes[BOAT_NUM_MAX]__attribute__((at(0xA1E00000)));

_boat_m24A boat_list_24A[BOAT_NUM_MAX]__attribute__((at(0xA1F00000)));;
_boat_m24A *boat_list_p24A[BOAT_NUM_MAX]__attribute__((at(0xA1F80000)));


_boat_m24B boat_list_24B[BOAT_NUM_MAX];
_boat_m24B *boat_lisp_p24B[BOAT_NUM_MAX];

/*----------------- External functions -----------------------*/
extern int insert_18(struct message_18 * p_msg);
extern int insert_24A(struct message_24_partA * p_msg);
extern int insert_24B(type_of_ship * p_msg);
extern void updateTimeStamp(void);

/*----------------- External variables -----------------------*/
boat mothership;
mapping center;

extern volatile int xlCnt;


struct message_18 msg_18;

int N_boat = 0;
/*----------------- local   function  --------------------*/


void SysTick_Init(void);



void UI_Task(void *p_arg)/*描述(Description):	任务UI_Task*/
{
		MainTask();
}



void Insert_Task(void *p_arg)  //等待接收采集到的数据
{ 
   int tmp  = 0;

   uint8_t *s; 
   INT8U err;
  //	static int a=0;
   message_18 text_out;
   message_24_partA text_out_24A;
   type_of_ship text_out_type_of_ship; 
  // USER_Init();
   while(1)
   {	

     s = OSQPend(QSem,0,&err);
    

      tmp  = translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship); 
      OSMutexPend(Refresher, 0, &myErr);        
      switch(tmp)
      {
         case 18:      
              insert_18(&text_out);
              break;
          case 240:
              insert_24A(&text_out_24A);
              break;
          case 241:       
              insert_24B(&text_out_type_of_ship);       
              break;
          default:
           break;
      }
    OSMutexPost(Refresher);    
    OSTimeDly(20); 

   }
}


void Refresh_Task(void *p_arg)//任务Refresh_Task
{
   while(1)
   {
    OSMutexPend(Refresher, 0, &myErr);
    updateTimeStamp();
    OSMutexPost(Refresher);

    check();
    isChecked  = 1;
         
    OSTimeDlyHMSM(0,0,5,0);
   }
}
 
 
 
void App_TaskStart(void)//初始化UCOS，初始化SysTick节拍，并创建三个任务
{
	INT8U err;

  mothership.latitude = MOTHERSHIP_LA;
  mothership.longitude = MOTHERSHIP_LG;
  mothership.true_heading  = 0;
  
  center.lgtude  = MOTHERSHIP_LG;
  center.lttude  = MOTHERSHIP_LA;
  
  SPI1_DMA_Init();
  SPI1_Int();
   
  OSInit();  
  SysTick_Init();/* 初始化SysTick定时器 */
  Refresher  = OSMutexCreate(6,&myErr);
  Updater    = OSMutexCreate(6,&myErr_2);
  QSem = OSQCreate(&MsgQeueTb[0],MSG_QUEUE_TABNUM); //创建消息队列，10条消息
  PartitionPt=OSMemCreate(Partition,MSG_QUEUE_TABNUM,100,&err);
  
  OSTaskCreateExt(UI_Task, (void *)0,(OS_STK *)&UI_Task_Stack[USER_TASK_STACK_SIZE-1],  UI_Task_PRIO, UI_Task_PRIO, (OS_STK *)&UI_Task_Stack[0], USER_TASK_STACK_SIZE,(void*)0, OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 UI_Task */
  OSTaskCreateExt(Insert_Task,(void *)0,(OS_STK *)&Insert_Task_Stack[TOUCH_TASK_STACK_SIZE-1],Insert_Task_PRIO,Insert_Task_PRIO,(OS_STK *)&Insert_Task_Stack[0],TOUCH_TASK_STACK_SIZE,(void*)0,OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR );/* 创建任务 Insert_Task */
  OSTaskCreateExt(Refresh_Task,  (void *)0,(OS_STK *)&Refresh_Task_Stack[KEY_TASK_STACK_SIZE-1],    Refresh_Task_PRIO,  Refresh_Task_PRIO  ,(OS_STK *)&Refresh_Task_Stack[0],  KEY_TASK_STACK_SIZE,(void*)0,  OS_TASK_OPT_STK_CHK+OS_TASK_OPT_STK_CLR);/* 创建任务 Refresh_Task */

//#if DEBUG_LEVEL >= DEBUG_LEVEL_TAG
//    INFO("DEBUG_LEVEL: TAG");
//#elif DEBUG_LEVEL >= DEBUG_LEVEL_WATCH
//    INFO("DEBUG_LEVEL: WATCH");
//#elif DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
//    INFO("DEBUG_LEVEL: WARNKNG");  
//#elif DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
//    INFO("DEBUG_LEVEL: ERROR");    
//#else 
//    INFO("DEBUG_LEVEL: 0");    
//#endif
       

  OSStart();
}

//		switch(translate_(s,&text_out,&text_out_24A,&text_out_type_of_ship))
int translate_(unsigned char *text,message_18 *text_out,message_24_partA *text_out_24A,type_of_ship *text_out_type_of_ship)
{
  int i=0,comma=0;
  int tmp  = 0;

  long shiftReg  = 0;
  
  if((text[0]!='!')&&(text[0]!='$'))
    return 0;
  if((text[1]=='A')&&(text[2]=='I')&&(text[3]=='V')&&(text[4]=='D')&&(text[5]=='M'))
  { 
     for(i=6; i<20; i++)
     {
        if(text[i] == ',')
        {
           comma++;
           if(comma == 5)
              break;
        }
     }    
     if(i<20)
     {     
          i++;     
          tmp  = change_table(text[i]);
           
          
          switch(tmp)
          {
            case 19: 
                 (*text_out)  = translate_m18(text,i);
                 return 18;
            case 18:
                 (*text_out)=translate_m18(text,i);
                 return 18;           
            case 24:            
                if(change_table(text[i+6])&12)
                {
                   *text_out_type_of_ship = translate_m24B(text,i);
                   return 241;
                }
                else
                {               
                   *text_out_24A = translate_m24A(text,i); 
                   return 240;
                }
                            
           default:
           
                return tmp;
           
         }
      }
      else
      {
         return 0;
      }
  }

	else if((text[4]=='M')&&(text[5]=='C')) //GPS GPRMC
	{
//    tempgprmc = text[6]; mothership.latitude = tempgprmc << 24;
//    tempgprmc = text[7]; mothership.latitude = mothership.latitude + (tempgprmc << 16);
//    tempgprmc = text[8]; mothership.latitude = mothership.latitude + (tempgprmc << 8);
//    mothership.latitude = mothership.latitude + text[9];
//    mothership.latitude = mothership.latitude/10;    
    shiftReg   = text[6];
    shiftReg   = (shiftReg << 8) | text[7];
    shiftReg   = (shiftReg << 8) | text[8];
    shiftReg   = (shiftReg << 8) | text[9];
    if(shiftReg )
       mothership.latitude  = shiftReg / 10;
    
    
//    tempgprmc = text[10]; mothership.longitude = tempgprmc << 24;
//    tempgprmc = text[11]; mothership.longitude = mothership.longitude + (tempgprmc << 16);
//    tempgprmc = text[12]; mothership.longitude = mothership.longitude + (tempgprmc << 8);
//    mothership.longitude = mothership.longitude + text[13];
//    mothership.longitude = mothership.longitude/10;
      
    shiftReg   = text[10];
    shiftReg   = (shiftReg << 8) | text[11];
    shiftReg   = (shiftReg << 8) | text[12];
    shiftReg   = (shiftReg << 8) | text[13];
    if(shiftReg)
       mothership.longitude  = shiftReg / 10;
    
//    tempgprmc = text[14]; mothership.SOG = mothership.SOG + (tempgprmc << 8);
//    mothership.SOG = mothership.SOG + text[15];

      shiftReg   = text[14];
      shiftReg   = (shiftReg << 8) | text[15];
      mothership.SOG  = shiftReg;
   
//    tempgprmc = text[16]; mothership.COG = mothership.COG + (tempgprmc << 8);
//    mothership.COG = mothership.COG + text[17];

      shiftReg   = text[16];
      shiftReg   = (shiftReg << 8) | text[17];
      mothership.COG  = shiftReg;

//    tempgprmc = text[18]; SYS_Date = tempgprmc << 24;
//    tempgprmc = text[19]; SYS_Date = SYS_Date + (tempgprmc << 16);
//    tempgprmc = text[20]; SYS_Date = SYS_Date + (tempgprmc << 8);
//    SYS_Date = SYS_Date + text[21];

      shiftReg   = text[18];
      shiftReg   = (shiftReg << 8) | text[19];
      shiftReg   = (shiftReg << 8) | text[20];
      shiftReg   = (shiftReg << 8) | text[21];
      SYS_Date   = shiftReg;
   
//    tempgprmc = text[22]; SYS_Time = tempgprmc << 24;
//    tempgprmc = text[23]; SYS_Time = SYS_Time + (tempgprmc << 16);
//    tempgprmc = text[24]; SYS_Time = SYS_Time + (tempgprmc << 8);
//    SYS_Time = SYS_Time + text[25];	

      shiftReg   = text[22];
      shiftReg   = (shiftReg << 8) | text[23];
      shiftReg   = (shiftReg << 8) | text[24];
      shiftReg   = (shiftReg << 8) | text[25];
      SYS_Time   = shiftReg;
	}

return 0;
}

/************************************* End *************************************/






