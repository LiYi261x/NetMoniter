#include <LPC177x_8x.H>
#include "HZ.h"
#include "exfuns.h"
#include "lcd43.h"
#include "uart.h"
//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
uint32_t SIZE_GBK16;
uint32_t SIZE_GBK24;
uint32_t SIZE_GBK32;

//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	   
void Show_Font(uint16_t x,uint16_t y,uint8_t *font,uint8_t size,uint8_t mode)
{
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*font;
	ql=*(++font);	
	if(size!=32&&size!=16&&size!=24)return;	//��֧�ֵ�size	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*dzk++=0x00;//�������
	    return; //��������
	} 	
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		
	switch(size)
	{
		case 16:
			      dzk=(unsigned char*)GBK16_BASEADDR+foffset;	
		        break;
		case 24:
			      dzk=(unsigned char*)GBK24_BASEADDR+foffset;  			     
						break;
		case 32:
			      dzk=(unsigned char*)GBK32_BASEADDR+foffset; 
			      break;		
	}	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ��������� 	
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)   LCD_SetPixel(LCD_PANEL_UPPER,x,y,BLACK);
			else if(mode==0)LCD_SetPixel(LCD_PANEL_UPPER,x,y,BLACK); 
			temp=temp<<1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t*str,uint8_t size,uint8_t mode)
{					
	  uint16_t x0=x;
	  uint16_t y0=y;							  	  
    uint8_t bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
			if(!bHz)
			{
				if(*str>0x80)bHz=1;//���� 
				else              //�ַ�
				{      
					if(x>(x0+width-size/2))//����
			    {				   
						y+=size;
						x=x0;	   
			    }							    
					if(y>(y0+height-size))break;//Խ�緵��      
					if(*str==13)//���з���
					{         
							y+=size;
				      x=x0;
							str++; 
					}  
					else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
			    str++; 
					x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
				}
			}
			else//���� 
      {     
				bHz=0;//�к��ֿ�    
				if(x>(x0+width-size))//����
			  { 	    
					y+=size;
					x=x0;		  
			  }
	      if(y>(y0+height-size))break;//Խ�緵��  						     
	      Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	      str+=2; 
	      x+=size;//��һ������ƫ��	    
      }						 
    }   
}  			 		 
// //��ָ����ȵ��м���ʾ�ַ���
// //����ַ����ȳ�����len,����Show_Str��ʾ
// //len:ָ��Ҫ��ʾ�Ŀ��			  
// void Show_Str_Mid(uint16_t x,uint16_t y,uint8_t*str,uint8_t size,uint8_t len)
// {
// 	uint16_t strlenth=0;
//    	strlenth=strlen((const char*)str);
// 	strlenth*=size/2;
// 	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
// 	else
// 	{
// 		strlenth=(len-strlenth)/2;
// 	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
// 	}
// }   

























		  






