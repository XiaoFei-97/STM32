#ifndef _TIME_H_
#define _TIME_H_
#include "sys.h"


void TIM3_Int_Init(u16 arr,u16 psc);   //TIM3定时器的初始化，设置重装载值和分频系数

void TIM3_Pwm_Init(u16 arr,u16 psc);   //TIM3PWM的初始化，设置重装载值和分频系数

extern u8 TIM5CH1_CAPTURE_STA;
extern u16 TIM5CH1_CAPTURE_VAL;
void TIM5_Cap_Init(u16 arr,u16 psc);   //捕获初始化，设置重装载值和分频系数

void TIM1_PWM_Init(u16 arr,u16 psc);

#endif 

