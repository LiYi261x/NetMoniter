#include "w25qxx.h" 
#include "lpc177x_8x_ssp.h"
#include "lpc177x_8x_pinsel.h"  
#include "uart.h"	
#include "sdram.h"
#include "HZ.h"
#include "lcd43.h"
#include "exfuns.h"

//extern uint32_t SIZE_UNICODE16;
//extern uint32_t SIZE_UNICODE24;

void SPI_FLASH_Init(void)     //SPI_FLASH��ʼ������
{
	SSP_CFG_Type SSP_ConfigStruct;
  PINSEL_ConfigPin(0, 7, 2);//p0.7��SSP0_SCK
  PINSEL_ConfigPin(0, 6, 0);//p0.6: CS //SSP0_SSEL
  PINSEL_ConfigPin(0, 8, 2);//p0.8: SSP0_MISO
  PINSEL_ConfigPin(0, 9, 2);//p0.9: SSP0_MOSI
  GPIO_SetDir(0, (1<<6), 1); 	/* ���ô���Ƭѡ�ܽ�p0.6Ϊ��� */
  GPIO_SetValue(0, (1<<6));

  SSP_ConfigStruct.ClockRate =2000000; 	/* ����SPI�������20M */
  SSP_ConfigStruct.CPHA = SSP_CPHA_FIRST;
  SSP_ConfigStruct.CPOL = SSP_CPOL_HI; 
	SSP_ConfigStruct.Databit = SSP_DATABIT_8;
	SSP_ConfigStruct.Mode = SSP_MASTER_MODE;
	SSP_ConfigStruct.FrameFormat = SSP_FRAME_SPI;
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);//��ʼ��SSP1

	LPC_SSP1->CR1 |= SSP_CR1_SSP_EN;//SSP_Cmd(LPC_SSP1, ENABLE);	/* ʹ��SPI ���� 1 */
}
/* ������(Function):	SPI_FLASH_SendByte
 * ����(Param):				byte Ҫ���͵�1�ֽ����� 							
 * ����ֵ(Return):		tmp  ���յ���1�ֽ�����
 * ����(Description):	SPI�ֽڷ��ͽ��պ���*/
uint8_t SPI_FLASH_SendByte(uint8_t byte)//
{
	while ( (LPC_SSP1->SR & (SSP_SR_TNF|SSP_SR_BSY)) != SSP_SR_TNF );  /* �ȴ�æ��־λΪ��ͷ��ͻ���Ϊδ�� */
	LPC_SSP1->DR = byte;  /* ����1�ֽ����ݵ�SPI1 */
 	while ( (LPC_SSP1->SR & (SSP_SR_BSY|SSP_SR_RNE)) != SSP_SR_RNE );  /* æ��־λΪ��ͽ��ջ������ǿ� */
	return (LPC_SSP1->DR);
}

uint16_t W25QXX_TYPE=W25Q64;	//Ĭ����W25Q64

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q64
//����Ϊ8M�ֽ�,����128��Block,4096��Sector 
void W25QXX_Init(void)
{ 
	SPI_FLASH_Init();		   			//��ʼ��SPI
 	SPI_FLASH_CS_HIGH();	
	W25QXX_TYPE=W25QXX_ReadID();	//��ȡFLASH ID.
}  
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;	
  SPI_FLASH_CS_LOW();	
	SPI_FLASH_SendByte(0x90);//���Ͷ�ȡID����	  
	SPI_FLASH_SendByte(0x00); 	    
	SPI_FLASH_SendByte(0x00); 	    
	SPI_FLASH_SendByte(0x00); 	 			   
	Temp|=SPI_FLASH_SendByte(0xFF)<<8;
	Temp|=SPI_FLASH_SendByte(0xFF);	 
  printf("Flash ID :%x\r\n",Temp);	
	SPI_FLASH_CS_HIGH();
	return Temp;
} 
//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
uint8_t W25QXX_ReadSR(void)   
{  
	uint8_t byte=0;   
	SPI_FLASH_CS_HIGH();    
	SPI_FLASH_CS_LOW();                            //ʹ������   
	SPI_FLASH_SendByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI_FLASH_SendByte(0Xff);             //��ȡһ���ֽ�  
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     
	return byte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25QXX_Write_SR(uint8_t sr)   
{   
  SPI_FLASH_CS_HIGH(); 	
	SPI_FLASH_CS_LOW();                            //ʹ������   
	SPI_FLASH_SendByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI_FLASH_SendByte(sr);               //д��һ���ֽ�  
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void W25QXX_Write_Enable(void)   
{
	SPI_FLASH_CS_HIGH(); 
	SPI_FLASH_CS_LOW();                            //ʹ������   
  SPI_FLASH_SendByte(W25X_WriteEnable);      //����дʹ��  
 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25QXX_Write_Disable(void)   
{  
	SPI_FLASH_CS_HIGH(); 
	SPI_FLASH_CS_LOW();                            //ʹ������   
  SPI_FLASH_SendByte(W25X_WriteDisable);     //����д��ָֹ��    
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
} 		
  		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;   	
    SPI_FLASH_CS_HIGH(); 	
	  SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_ReadData);         //���Ͷ�ȡ����   
    SPI_FLASH_SendByte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
    SPI_FLASH_SendByte((uint8_t)((ReadAddr)>>8));   
    SPI_FLASH_SendByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI_FLASH_SendByte(0XFF);   //ѭ������  
    }
 	SPI_FLASH_CS_HIGH();  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    W25QXX_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_PageProgram);      //����дҳ����   
    SPI_FLASH_SendByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
    SPI_FLASH_SendByte((uint8_t)((WriteAddr)>>8));   
    SPI_FLASH_SendByte((uint8_t)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI_FLASH_SendByte(pBuffer[i]);//ѭ��д��  
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ 
	W25QXX_Wait_Busy();					   //�ȴ�д�����
	SPI_FLASH_CS_HIGH(); 
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
uint8_t W25QXX_BUFFER[4096];		 
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		
		W25QXX_Write_Enable();
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}
		else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
		SPI_FLASH_CS_HIGH(); 
	};	 
}
	  
//�ȴ�ʱ�䳬��...
void W25QXX_Erase_Chip(void) //��������оƬ	  
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_ChipErase);        //����Ƭ��������  
	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
	W25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   

//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25QXX_Erase_Sector(uint32_t Dst_Addr) //����һ������  
{  
	//����falsh�������,������  
	SPI_FLASH_CS_HIGH(); 	
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();   
  	SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_SectorErase);      //������������ָ�� 
    SPI_FLASH_SendByte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI_FLASH_SendByte((uint8_t)((Dst_Addr)>>8));   
    SPI_FLASH_SendByte((uint8_t)Dst_Addr);  
// 	 SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   //�ȴ��������
	SPI_FLASH_CS_HIGH(); 
}  
//�ȴ�����
void W25QXX_Wait_Busy(void)   
{   
	SPI_FLASH_CS_HIGH(); 
	while((W25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void W25QXX_PowerDown(void)   
{ 
	  SPI_FLASH_CS_HIGH(); 
  	SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_PowerDown);        //���͵�������  
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
    //delay_us(3);                               //�ȴ�TPD  
}   
//����
void W25QXX_WAKEUP(void)   
{  	SPI_FLASH_CS_HIGH();
  	SPI_FLASH_CS_LOW();                            //ʹ������   
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
// 	SPI_FLASH_CS_HIGH();                            //ȡ��Ƭѡ     	      
   // delay_us(3);                               //�ȴ�TRES1
}   

/*void updata_font(void)
{
	   uint16_t l=0;	uint16_t m=0;		 

		 l=308;//l=SIZE_UNICODE16/4096+1;
	   printf("\n\rupdata_font l:%d\n\r",l);
	   LCD_ShowString(30,150,200,16,16,"LOAD FONT TO Flash:");		
 		 LCD_FillRectangle(LCD_PANEL_UPPER,166,380,150,166, skyblue);
 	   for(m=0;m<l;m++)
	   {
		  W25QXX_Write(UNICODE16+m*4096,4096*m,4096);		//��0��ʼд��4096������
      LCD_FillRectangle(LCD_PANEL_UPPER,166,166+(310/l*m),150,166, NavyBlue);			 
		 }  
}*/

void load_font(void)
{
	   uint16_t l=0;	uint16_t m=0; 
	   l=308;//l=SIZE_UNICODE16/4096+1;
	   LCD_FillRectangle(LCD_PANEL_UPPER,166,380,150,166, skyblue);
     printf("\n\rload_font l:%d\n\r",l);
	   //LCD_ShowString(30,150,200,16,16,"LOAD FONT FORM FLASH:");
		 for(m=0;m<l;m++)
	   {
 			 W25QXX_Read(UNICODE16+m*4096,6*1024*1024+4096*m,4096);
       LCD_FillRectangle(LCD_PANEL_UPPER,166,166+(310/l*m),150,166, NavyBlue);			 
		 }	 
	   //LCD_ShowString(30,166,200,16,16,"16*16FONT LOAD SUCCEED");	 
}
























