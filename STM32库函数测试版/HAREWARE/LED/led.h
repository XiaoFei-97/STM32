#ifndef __LED_H_
#define __LED_H_
#include "sys.h"

void LED_Init(void);   //LED��ʼ��������
 
#define LED0 PBout(5)  //LED0��ʾPB5�˿�
#define LED1 PEout(5)  //LED1��ʾPE5�˿�

#endif

