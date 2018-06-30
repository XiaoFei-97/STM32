#ifndef _TIME_H_
#define _TIME_H_
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);  //��ʱ��3�ĳ�ʼ����������װ��ֵ�ͷ�Ƶϵ��

#define LED0_PWM_VAL TIM3->CCR2   //ͨ���ı�CCR2��ֵ���ı�ռ�ձ�
void TIM3_Pwm_Init(u16 arr,u16 psc);  //TIM3��PWM��ʼ����������װ��ֵ�ͷ�Ƶϵ��


extern u8 TIM5_CAPTURE_STA;
extern u16 TIM5CH1_CAPTURE_VAL;
void TIM5_Cap_Init(u16 arr,u16 psc);  //TIM5�����ʼ����������װ��ֵ�ͷ�Ƶϵ��

void TIM1_PWM_Init(u16 arr,u16 psc);



#endif

