#ifndef _IWDG_H_
#define _IWDG_H_
#include "sys.h"

void IWDG_Init(u8 prer,u16 rlr);    //IWDG��ʼ������Ƶϵ������װ��ֵ
void IWDG_Feed(void);      //ι������

void WWDG_Init(u8 tr,u8 wr,u32 fprer);    //WWDG��ʼ������ǰ����ֵ���ϴ���ֵ����Ƶʱ��
void WWDG_Set_Counter(u8 cnt);    //ι������
void WWDG_NVIC_Init(void);      //���÷���


#endif

