#ifndef __EXFUNS_H
#define __EXFUNS_H 					   
#include <LPC177x_8x.H>
#include "ff.h"
#include "sdram.h"
extern FATFS *fs[_VOLUMES];  
extern FIL *file;	 	
extern FIL *FILE_PIC;     //ͼƬ�ļ�
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern uint32_t SIZE_UNIGBK;
extern uint8_t *UNIGBK;
extern uint8_t *UNICODE16;
extern uint8_t *UNICODE24;
extern uint8_t *UNICODE32;
extern uint8_t *fatbuf;//SD�����ݻ�����
extern uint8_t *PICTURE;//SD�����ݻ�����0xA0700000~0xA1000000

extern uint32_t SIZE_UNICODE16;
extern uint32_t SIZE_UNICODE24;


#define UNIGBK_BASEADDR       ((uint32_t)SDRAM_BASE_ADDR ) //GBK��UNICODEת���
#define UNICODE16_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x130000 )//UNICODE16�ֿ���ʼ��ַ
#define UNICODE24_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x200000)//GBK24�ֿ���ʼ��ַ
#define UNICODE32_BASEADDR	  ((uint32_t)SDRAM_BASE_ADDR + 0x400000)//GBK32�ֿ���ʼ��ַ
#define FATFS_BASEADDR	      ((uint32_t)SDRAM_BASE_ADDR + 0x700000)
#define file_BASEADDR	        ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x10000)
#define file_PIC_BASEADDR     ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x20000)
#define fatbuf_BASEADDR	      ((uint32_t)SDRAM_BASE_ADDR + 0x700000+0x30000)
#define PICTURE_BASEADDR      ((uint32_t)SDRAM_BASE_ADDR + 0x800000)

//f_typetell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.��exfuns.c���涨��
#define T_BIN		0X00	//bin�ļ�
#define T_LRC		0X10	//lrc�ļ�
#define T_NES		0X20	//nes�ļ�

#define T_TEXT		0X30	//.txt�ļ�
#define T_C			0X31	//.c�ļ�
#define T_H			0X32    //.h�ļ�

#define T_WAV		0X40	//WAV�ļ�
#define T_MP3		0X41	//MP3�ļ� 
#define T_APE		0X42	//APE�ļ�
#define T_FLAC		0X43	//FLAC�ļ�

#define T_BMP		0X50	//bmp�ļ�
#define T_JPG		0X51	//jpg�ļ�
#define T_JPEG		0X52	//jpeg�ļ�		 
#define T_GIF		0X53	//gif�ļ�   

#define T_AVI		0X60	//avi�ļ�  

 
uint8_t exfuns_init(void);							//�����ڴ�
uint8_t f_typetell(uint8_t *fname);						//ʶ���ļ�����
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free);	//�õ�������������ʣ������
uint32_t exf_fdsize(uint8_t *fdname);						//�õ��ļ��д�С			
void fs_test(void);
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);
void RES_FALT(FRESULT RES);
#endif


