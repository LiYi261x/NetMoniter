#include "string.h"
#include "exfuns.h"
#include "uart.h"
#include "ff.h"
#include "sd.h"
#include "lpc_types.h"
#include "lcd43.h"
// #include "HZ.h"
#define FILE_MAX_TYPE_NUM		7	//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4	//���FILE_MAX_SUBT_NUM��С��

uint8_t*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]= //�ļ������б�
{
{"BIN"},			//BIN�ļ�
{"LRC"},			//LRC�ļ�
{"NES"},			//NES�ļ�
{"TXT","C","H"},	//�ı��ļ�
{"WAV","MP3","APE","FLAC"},//֧�ֵ������ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
{"AVI"},			//��Ƶ�ļ�
};

FATFS *fs[_VOLUMES];//�߼����̹�����.	 
FIL *file;	  		//�ļ�
FIL *FILE_PIC;    //ͼƬ�ļ�
UINT br,bw;			//��д����
FILINFO fileinfo;	//�ļ���Ϣ
DIR dir;  			//Ŀ¼
uint32_t SIZE_UNIGBK;
uint8_t *UNIGBK;
uint8_t *UNICODE16;
uint8_t *UNICODE24;
uint8_t *UNICODE32;
uint8_t *fatbuf;		//SD�����ݻ�����,1MB֮��
uint8_t *PICTURE;    //SD�����ݻ�����

uint32_t SIZE_UNICODE16;
uint32_t SIZE_UNICODE24;

uint8_t exfuns_init(void)//Ϊexfuns�����ڴ�
 {
	fs[0]=(FATFS*)FATFS_BASEADDR;	//Ϊ����i�����������ڴ�	
  file=(FIL*)file_BASEADDR;		//���d�SDRAM�������ֽڵ�ַ��Ϊfile�����ڴ�
	FILE_PIC=(FIL*)file_PIC_BASEADDR;
	UNIGBK=(uint8_t*)UNIGBK_BASEADDR;
	UNICODE16=(uint8_t*)UNICODE16_BASEADDR;
	UNICODE24=(uint8_t*)UNICODE24_BASEADDR;
	UNICODE32=(uint8_t*)UNICODE32_BASEADDR;
	PICTURE=(uint8_t*)PICTURE_BASEADDR;			//ΪPICTURE�����ڴ�
 	fatbuf=(uint8_t*)fatbuf_BASEADDR;				//Ϊfatbuf�����ڴ�	
 	return 0;
 }

void fs_test(void)
{
	FRESULT res;
	FRESULT ceshi;
	int i;
	char folder[255] = ""; 
	res = (FRESULT)SD_Init();
	if(res == FALSE){printf("sd init error.\n\r");}
	res = f_mount(fs[0],"0:", 1);
	if(res != FR_OK){printf("f_mount error = %d\n\r",res);}

//  LCD_ShowString(200,100,400,32,32,"Loading UNIGBK.BIN .....");
	ceshi=f_open (file, "UNIGBK.BIN", FA_READ);  RES_FALT(ceshi);
	SIZE_UNIGBK=(*file).fsize;
	printf("\n UNIGBK.BIN �ļ���ռ��С��%d�ֽ� \n",(*file).fsize);
	ceshi=f_read(file, UNIGBK, SIZE_UNIGBK, &br);	RES_FALT(ceshi);
	printf("\nbr ��ֵ��%d\n",br);printf("\n");
	//for (i = 0; i < br; i++){printf("%c",UNIGBK[i]);}	
	f_close(file);br=0;//////////////////////////////////////////////�ر��ļ�

  scan_files(folder);	//����SD���ļ�

//  LCD_ShowString(200,132,400,32,32,"Loading text.txt .....");	
	ceshi=f_open (file, "text.txt", FA_READ);  RES_FALT(ceshi);
	printf("\n text.txt �ļ���ռ��С��%d�ֽ� \n",(*file).fsize);
	ceshi=f_read(file, fatbuf, (*file).fsize, &br);	RES_FALT(ceshi);
	printf("\nbr ��ֵ��%d\n",br);printf("\n");
	for (i = 0; i < br; i++){printf("%c",fatbuf[i]);}	
	f_close(file);br=0;//////////////////////////////////////////////�ر��ļ�
	
//	LCD_ShowString(200,164,400,32,32,"Loading HZ16.sif .....");
 	ceshi=f_open(file,"HZ16.sif", FA_READ);
	SIZE_UNICODE16=(*file).fsize;
	printf("\n UNICODE16��ռ��С��%d�ֽ� \n",SIZE_UNICODE16);
	ceshi=f_read(file, UNICODE16,SIZE_UNICODE16, &br);//��HZ16.sif�ļ�����0xA0030000��ʼ��SDRAM
	printf("\n br ��ֵ��%d \n",br);
 // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
	f_close(file);br=0;//////////////////////////////////////////////�ر��ļ�
	
// 	ceshi=f_open(file,"GBK24.FON", FA_READ);
// 	SIZE_GBK24=(*file).fsize;
// 	printf("\n SIZE_GBK24��ռ��С��%d�ֽ� \n",SIZE_GBK24);
// 	ceshi=f_read(file, GBK24,SIZE_GBK24, &br);//��GBK16�ļ�����0xA0100000��ʼ��SDRAM
// 	printf("\n br ��ֵ��%d \n",br);
//  // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
// 	f_close(file);br=0;//////////////////////////////////////////////�ر��ļ�
 	
// 	ceshi=f_open(file,"GBK32.FON", FA_READ);
// 	SIZE_GBK32=(*file).fsize;
// 	printf("\n SIZE_GBK32��ռ��С��%d�ֽ� \n",SIZE_GBK32);
// 	ceshi=f_read(file, GBK32,SIZE_GBK32, &br);//��GBK16�ļ�����0xA0300000��ʼ��SDRAM
// 	printf("\n br ��ֵ��%d \n",br);
//  // for (i = 0; i < br; i++){printf("%x",GBK16[i]);}
// 	f_close(file);br=0;//////////////////////////////////////////////�ر��ļ�
	

//	LCD_ShowString(200,196,400,32,32,"ALL FILE LOADING SUCCED");
}
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fileinfo;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fileinfo.lfname = lfn;
    fileinfo.lfsize = sizeof lfn;
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fileinfo);                   /* Read a directory item */
            if (res != FR_OK || fileinfo.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fileinfo.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
            fn = fileinfo.fname;
#endif
            if (fileinfo.fattrib & AM_DIR) {                    /* It is a directory */
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s \n\r", path, fn);
            }

        }
    }

    return res;
}
//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
uint8_t char_upper(uint8_t c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	      
//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//		 ����,����λ��ʾ��������,����λ��ʾ����С��.
uint8_t f_typetell(uint8_t *fname)
{
	uint8_t tbuf[5];
	uint8_t *attr='\0';//��׺��
	uint8_t i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//����Ա�
	{
		for(j=0;j<FILE_MAX_SUBT_NUM;j++)//����Ա�
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//û�ҵ�		 			   
}	 

//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
uint8_t exf_getfree(uint8_t *drv,uint32_t *total,uint32_t *free)
{
	FATFS *fs1;
	uint8_t res;
    uint32_t fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res =(uint32_t)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
 	}
	return res;
}	
void RES_FALT(FRESULT RES)
{
	if(RES) 
	{
		switch (RES)
		{
			case FR_NO_FILE :        printf("\n can not found this file \n");break;
			case FR_NO_PATH :        printf("\n can not found this PATH \n");break;
			case FR_INVALID_NAME :printf("\n �ļ�����Ч\n");break;
			case FR_INVALID_DRIVE   :printf("\n ����������Ч \n");break;
			case FR_EXIST :          printf("\n ���ļ��Ѵ��� \n");break;
			case FR_DENIED :         printf("\n ���ʱ��ܾ� \n");break;
			case FR_NOT_READY :      printf("\n ���ڡ��������������޷����� \n");break;
			case FR_WRITE_PROTECTED: printf("\n SD����д���� \n");break;
			case FR_DISK_ERR :       printf("\n �ײ�IO��ڴ��� \n");break;
			case FR_INT_ERR:         printf("\n FAT�ṹ���� \n");break;
			case FR_NOT_ENABLED :    printf("\n �߼�������û�й����� \n");break;
			case FR_NO_FILESYSTEM :  printf("\n ����û����Ч��FAT�� \n");break;
		}
  }	
}









