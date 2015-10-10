#include "includes.h"
#include "stm32f10x_it.h"
#include "UART.h"
#include "Translate.h"
#include "key.h" 
#include "IWDG.h"
#include "timer.h"
#include "MUSIC.h"
#include "SPI.h"
void BSP_Init(void)
{
	unsigned short int i;
	SystemInit(); /* ����ϵͳʱ��Ϊ72M */
	SysTick_init(); /* ��ʼ����ʹ��SysTick��ʱ�� */
	
	GPIO_Configuration();   
  DMA_Configuration();
	UART_Configuration();
	NVIC_Configuration();   
  USART_Cmd(USART1, ENABLE);	
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	IWDG_Init(4,625);//���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s
	TIM4_Int_Init();  
	SPI2_Init();
  Music_Init();
	Key_Init();//��IOʱ��
}
 
void SysTick_init(void)
{
	SysTick_Config(SystemFrequency/OS_TICKS_PER_SEC);
}















