#include "UART.h"
#include "Translate.h"
#include "misc.h"
#include "ucos_ii.h"
#include "app.h"

uint8_t Uart_Rx[UART_RX_LEN];//UART2����
//uint8_t Uart_Tx[UART_TX_LEN];//UART2����
//uint8_t Message_AIS[100];
//extern uint8_t flag_translate;
//volatile int myCnt = 0;
//volatile uint8_t Res_Flag;

void GPIO_Configuration(void)
{	
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//�򿪴���1�Ͷ�ӦIOʱ��      
  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);//UART1��ӳ�䵽PB6��7��ʹ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; /* Configure USART1 Rx (PB.7) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  /* Configure USART1 Tx (PB.6) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
////////////////////////////////////////////////////////////////////////////////////////////////	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//����USART2ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; /* Configure USART2 Rx (PA.3) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  /* Configure USART2 Tx (PA.2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//////////////////////////////////////////////////////////////////////
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//�򿪴���3�Ͷ�ӦIOʱ��      
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; /* Configure USART3 Rx (PB.11) as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  /* Configure USART2 Tx (PA.2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
}

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //����DMAʱ��  

    DMA_DeInit(DMA1_Channel6); 	//DMA1ͨ��5����   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);//�����ַ  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart_Rx;     //�ڴ��ַ  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma���䷽������赽�ڴ� 
    DMA_InitStructure.DMA_BufferSize = UART_RX_LEN;               //����DMA�ڴ���ʱ�������ĳ���  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA�������ַ���� 
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ� 8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //�ڴ������ֳ�8λ   
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //����DMA�Ĵ���ģʽ   
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //����DMA�����ȼ��� ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //����DMA��2��memory�еı����������  
    DMA_Init(DMA1_Channel6,&DMA_InitStructure);  
  
    DMA_Cmd(DMA1_Channel6,ENABLE);     //ʹ��ͨ��5  
}

void UART_Configuration(void)
{
	  USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      
    USART_InitStructure.USART_BaudRate = 115200;   
 
    USART_Init(USART1,&USART_InitStructure);          

    USART_ITConfig(USART1,USART_IT_TC,DISABLE);    //��ֹ 
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  //��ֹ 
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);   //����
		USART_ITConfig(USART2,USART_IT_ERR,ENABLE);//���������ж�
	  USART_ITConfig(USART2,USART_IT_PE,ENABLE);//���������ж�


//    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//����DMA��ʽ����  
/////////////////////////////////////////////////////////////////////////////////////////////
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode =  USART_Mode_Rx;      
    USART_InitStructure.USART_BaudRate = 38400;   
 
    USART_Init(USART2,&USART_InitStructure);          

    USART_ITConfig(USART2,USART_IT_TC,DISABLE);    //��ֹ 
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  //��ֹ 
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //����

    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	//ʹ��UART2����DMA��ʽ����
//////////////////////////////////////////////////////////////////////////////		
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx ;      
    USART_InitStructure.USART_BaudRate = 115200;   
 
    USART_Init(USART3,&USART_InitStructure);          

    USART_ITConfig(USART3,USART_IT_TC,DISABLE);    //��ֹ 
    USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);  //��ֹ 
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);   //����
		USART_ITConfig(USART3,USART_IT_ERR,DISABLE);//���������ж�
	  USART_ITConfig(USART3,USART_IT_PE,DISABLE);//���������ж�


    //USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	//����DMA��ʽ����
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//����UART3�ж�    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;               //ͨ������Ϊ����3�ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;       //�ж�ռ�ȵȼ�0    
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //�ж���Ӧ���ȼ�0    
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�    
    NVIC_Init(&NVIC_InitStructure);  
}

void USART1_IRQHandler(void)                                 
{     
	uint32_t Length = 0;
//	uint8_t index,*pt,*pt0,err;	

	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{  
		//flag_translate=1;
		  DMA_Cmd(DMA1_Channel5,DISABLE); 
		  Length = USART1->SR;  
 		  Length = USART1->DR; //��USART_IT_IDLE��־ 
 			Length = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);		
      //if(45<Length<55)
			//{		
					//OSQPost(QSem,(void *)Uart_Rx);
				printf("\r\n%d",Length);
//         for(i=0;i<Length;i++)
// 			  {
// 					Message_AIS[i]=Uart_Rx[i];//Message_AIS++;
// 					//printf("%c",Uart_Rx[i]);
//         }		
// 			pt=OSMemGet(PartitionPt,&err);
// 			pt0=pt;
// 			for(index=0;index<50;index++)
// 			{
// 				*pt=Uart_Rx[index];
// 				pt++;
// 			}
// 			OSQPost(QSem,(void *)pt0);	
//		   printf("\r\nDMA OK ");
			 
//           for(index=0;index<50;index++)	
//          {
//            Partition[myCnt][index]=Uart_Rx[index];
//          }
//        OSQPost(QSem,(void *)Partition[myCnt]); 
//        myCnt++;
//        myCnt  = myCnt%(MSG_QUEUE_TABNUM);   
//        printf("myCnt:%d\n\r!",myCnt);	
		  //}			 
		  DMA1_Channel5->CNDTR = UART_RX_LEN;//��װ��,���ý��յ�ַƫַ��0��ʼ
		  DMA_Cmd(DMA1_Channel5, ENABLE);//������,�ؿ�DMA   
	}

  if((USART_GetITStatus(USART1, USART_IT_ORE) != RESET)||(USART_GetITStatus(USART1, USART_IT_NE) != RESET)||(USART_GetITStatus(USART1, USART_IT_FE) != RESET)||(USART_GetITStatus(USART1, USART_IT_PE) != RESET))
	{
		  printf("\r\nERROR!");
 			Length = USART1->SR;  
 			Length = USART1->DR; //��USART_IT_IDLE��־  
  }	
	__nop();   
}

void USART2_IRQHandler(void)                                 
{  
	uint32_t Length = 0;//,i=0;
//	uint16_t Res;	
// 	if(USART1->SR&(1<<5))//(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
// 	{
// 		Res=USART1->DR;//USART_ReceiveData(USART3);
// 		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	
// 			USART_SendData(USART1, Res);
// 	
// // 		if(Res=='!')
// // 		{
// // 			Uart_Rx[0]=Res;
// // 			i=0;
// // 			Res_Flag=1;
// //     }
// // 		else if(Res=='\n')
// // 		{
// // 			Res_Flag=0;
// // 			i=0;
// 			//OSQPost(QSem,(void *)Uart_Rx);
// //     }
// // 		else if(Res_Flag)
// // 		{
// // 			i++;
// // 			Uart_Rx[i]=Res;
// //     }
//   }
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	{  
		//flag_translate=1;
		  DMA_Cmd(DMA1_Channel6,DISABLE); 
		  Length = USART3->SR;  
 		  Length = USART3->DR; //��USART_IT_IDLE��־ 
 			Length = UART_RX_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);		
//      if(45<Length<55)
   if(Length > 45  &&  Length < 55)
			{		
					OSQPost(QSem,(void *)Uart_Rx);
			}
			//printf("\r\n%d",Length);	
			DMA1_Channel6->CNDTR = UART_RX_LEN;//��װ��,���ý��յ�ַƫַ��0��ʼ
		  DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA 
  }
}
void  Putc_UART1(u8 ch)
{
	USART_SendData(USART1, ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
}
void  Putc_UART2(u8 ch)
{
	USART_SendData(USART2, ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
}
void  Putc_UART3(u8 ch)
{
	USART_SendData(USART3, ch);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
}
int fputc(int ch, FILE *f)
{
	USART_SendData(USART3, (uint8_t) ch);

	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART3->SR & USART_FLAG_RXNE));

    return ((int)(USART3->DR & 0x1FF));
}
