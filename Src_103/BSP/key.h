#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"	


#define NO_KEY_PRESS	0		//�ް������µļ�ֵ
/* -----���²��������ʵ������޸�----- */
#define	KEY_STAT_noKeyPress		0
#define KEY_STAT_filter			  1
#define KEY_STAT_clicking			2
#define KEY_STAT_longKeyPress	3
#define KEY_STAT_Release			4


void Key_Init(void);
uint32_t Key_1Line_Scan(void);
uint32_t Key_2Line_Scan(void);
uint32_t Key_3Line_Scan(void);
uint32_t Key_4Line_Scan(void);
uint32_t Key_5Line_Scan(void);
uint32_t Get_KeyValue(void);
#endif /* __LED_H */

