#include "lpc177x_8x_gpio.h"
#include "lpc177x_8x_clkpwr.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_lcd.h"

#include "uart.h"
#include "dma.h"
#include "timer.h"
#include "lcd43.h"
//#include "w25qxx.h"
#include "pwm.h"
#include <ucos_ii.h>
// #include "exfuns.h"

extern volatile int TimeMS;  // Defined in GUI_X.c

void SysTick_Handler (void);
void Delay (int tick);
void App_TaskStart(void);

void SysTick_Handler (void)//	SysTick中断服务函数
{
	OS_CPU_SR  cpu_sr;
	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();
	OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
	TimeMS++;
	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}

void SysTick_Init(void)//SysTick_Init
{
	uint32_t cclk = CLKPWR_GetCLK(CLKPWR_CLKTYPE_CPU);	/* 获取系统时钟频率 */
	SysTick_Config(cclk/OS_TICKS_PER_SEC - 1);	/* 配置SysTick为10ms中断一次 */
}

int main(void)
{	
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);/* 打开GPIO外设电源，上电后GPIO电源默认是打开的，此行可去掉 */



	USER_Init();//初始化UART0 ，UART2，打开Timer2
	lpc1788_Lcd_Init();	  /* 初始化LCD */	
	LCD_test();
  //fs_test();
//  	W25QXX_Init();
//  	exfuns_init();
//  	load_font(); //加载flash字库
	
	App_TaskStart();

	while(1);
}

/************************************* End *************************************/
