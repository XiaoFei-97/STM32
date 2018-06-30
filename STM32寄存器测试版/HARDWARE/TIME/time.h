#ifndef _TIME_H_
#define _TIME_H_
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);  //定时器3的初始化，设置重装载值和分频系数

#define LED0_PWM_VAL TIM3->CCR2   //通过改变CCR2的值来改变占空比
void TIM3_Pwm_Init(u16 arr,u16 psc);  //TIM3的PWM初始化，设置重装载值和分频系数


extern u8 TIM5_CAPTURE_STA;
extern u16 TIM5CH1_CAPTURE_VAL;
void TIM5_Cap_Init(u16 arr,u16 psc);  //TIM5捕获初始化，设置重装载值和分频系数

void TIM1_PWM_Init(u16 arr,u16 psc);



#endif

