#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_gpdma.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include <ucos_ii.h>
uint8_t Buffer0[23]  = "uart0 init success\n\r";
uint8_t Buffer2[23]  = "uart2 init success!\n\r"; 

// __IO uint8_t UART_RX[128];//DMA_UART�Ĵ��ڽ��ջ�����
//  OS_EVENT *QSem;//������Ϣ����ָ��
//  OS_MEM   *PartitionPt;
//extern void *MsgQeueTb[10];//������Ϣָ�����飬���г���Ϊ10
//extern uint8_t Partition[10][128];//[SIZE];
void lpc1788_Uart_Init(int port)//LPC1788_Uart_Init
{
	UART_CFG_Type UARTConfigStruct;	/* UART���� */
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	/* UART FIFO���� */
	switch(port)
	{
		case 0:
					PINSEL_ConfigPin(0,2,1);
					PINSEL_ConfigPin(0,3,1);break;
		case 2:
					PINSEL_ConfigPin(0,10,1);
					PINSEL_ConfigPin(0,11,1);break;
		default:break;
	}
	UART_ConfigStructInit(&UARTConfigStruct);/* ��ʼ��UARTConfigStruct�ṹ��Ϊ��������Ϊ115200��8λ���ݣ�����żУ�飬1λֹͣλ */
	UARTConfigStruct.Baud_rate = 115200;
	UART_Init((UART_ID_Type)port, &UARTConfigStruct);	/* ��UARTConfigStruct�����Ĳ�����ʼ��UART���� */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;//ΪUARTʹ��DMA
	UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);// ΪUART0�����ʼ��FIFO

	UART_TxCmd((UART_ID_Type)port, ENABLE);/* ʹ��UART���� */
	
	UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
	NVIC_DisableIRQ(UART0_IRQn);
		
}
static void UART_Config(unsigned char channel)
{
	UART_CFG_Type UARTConfigStruct1;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct1;
	PINSEL_ConfigPin(0,10,1);
	PINSEL_ConfigPin(0,11,1);
	UART_ConfigStructInit(&UARTConfigStruct1);
	UARTConfigStruct1.Baud_rate = 38400;
	UART_Init((UART_ID_Type)channel, &UARTConfigStruct1);
	
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct1);
	UARTFIFOConfigStruct1.FIFO_DMAMode = ENABLE;
	UART_FIFOConfig((UART_ID_Type)channel, &UARTFIFOConfigStruct1);
	UART_TxCmd((UART_ID_Type)channel, ENABLE);
}

// static void DMA_Config(unsigned char channel)
// {
// 	GPDMA_Channel_CFG_Type GPDMACfg;
// // 	GPDMA_Init();
// 	NVIC_DisableIRQ (DMA_IRQn);
// 	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
// 	GPDMACfg.ChannelNum = 1;
// 	GPDMACfg.SrcMemAddr = 0;// Source memory - don't care
// 	GPDMACfg.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
// 	GPDMACfg.TransferSize = sizeof(UART2_RX);// Transfer size
// 	GPDMACfg.TransferWidth = 0;// Transfer width - don't care
// 	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
// 	GPDMACfg.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
// 	GPDMACfg.DstConn = 0;// Destination connection - don't care
// 	GPDMACfg.DMALLI = 0;// Linker List Item - unused
// 	GPDMA_Setup(&GPDMACfg);
// 	NVIC_EnableIRQ (DMA_IRQn);
// 	GPDMA_ChannelCmd(1, ENABLE);
// }

void USER_Init(void)
{	
	GPDMA_Init();
	lpc1788_Uart_Init(0);/* ��ʼ������Uart0 ʹ��DMAͨ��0*/	
	lpc1788_DMA_Init();  
//	printf("uart0 init success!\n\r\r");	
	UART_Send((UART_ID_Type)0, Buffer0,sizeof(Buffer0),BLOCKING);
	
	UART_Config(2);	
	DMA_Config(1);      
	UART_Send((UART_ID_Type)2,Buffer2,sizeof(Buffer2),BLOCKING);///* ��ӡһ���ַ�������Uart2 */
////	lpc1788_TIMER_Init();
}
// void TIMER2_IRQHandler(void)
// {		
// 	static unsigned int length_last=sizeof(UART_RX) &0x0fff;
// 	unsigned int length = LPC_GPDMACH1->CControl & 0x0fff;	
//   uint8_t *pt,*pt0,index,err;		
// 	if (TIM_GetIntStatus(LPC_TIM2, TIM_MR0_INT))	/* �ж��Ƿ���ͨ��0ƥ���ж� */	
// 	{	
// 		TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);/* ���ͨ��0ƥ���жϱ�־ */	
// 		if((length==length_last && length_last!=(sizeof(UART_RX) &0x0fff)))//˵�����ڴ��ݽ���
// 		{
// 			uint8_t uiIIRtemp = sizeof(UART_RX)-length;
// 			GPDMA_ChannelCmd(1, DISABLE);
// 			LPC_GPDMACH1->CControl = (LPC_GPDMACH1->CControl & 0xfffff000)|(sizeof(UART_RX) &0x0fff);
// 			LPC_GPDMACH1->CDestAddr = (uint32_t) &UART_RX;//������ʼ��ַ
// 			{//�ж��û�����
// 				pt=OSMemGet(PartitionPt,&err);
// 				pt0=pt;
//  				for(index=0;index<uiIIRtemp;index++)
//  				{
//  					*pt=UART_RX[index];
// 					pt++;
//         }
// 			  OSQPost(QSem,(void *)pt0);
// 			}
// 			length_last=sizeof(UART_RX) &0x0fff;
// 			GPDMA_ChannelCmd(1, ENABLE);
// 		}
// 	else if((sizeof(UART_RX)-length)>150)//�����쳣
// 	{
// 		uint8_t uiIIRtemp = sizeof(UART_RX)-length;
// 		GPDMA_ChannelCmd(1, DISABLE);
// 		LPC_GPDMACH1->CControl = (LPC_GPDMACH1->CControl & 0xfffff000)|(sizeof(UART_RX) &0x0fff);
// 		LPC_GPDMACH1->CDestAddr = (uint32_t) &UART_RX;//������ʼ��ַ
// 		{//�ж��û�����
// 			unsigned char index;
// 			for(index=0;index<uiIIRtemp;index++)
// 				UART_RX[index]=0;
// 		}
// 		length_last=sizeof(UART_RX) &0x0fff;
// 		GPDMA_ChannelCmd(1, ENABLE);
// 	}
// 	else
// 		length_last = length;
// 	}
// }

PUTCHAR_PROTOTYPE//�ض���C printf������Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* ����һ���ַ���UART */
	while (UART_CheckBusy((UART_ID_Type)UART_0) == SET);/* ���ڷ�����ɣ�THR����Ϊ��*/
  return ch;
}

/************************************* End *************************************/
