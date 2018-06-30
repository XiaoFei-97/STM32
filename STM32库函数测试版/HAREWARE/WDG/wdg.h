#ifndef _IWDG_H_
#define _IWDG_H_
#include "sys.h"

void IWDG_Init(u8 prer,u16 rlr);    //IWDG初始化，分频系数及重装载值
void IWDG_Feed(void);      //喂狗函数

void WWDG_Init(u8 tr,u8 wr,u32 fprer);    //WWDG初始化，当前计数值，上窗口值，分频时基
void WWDG_Set_Counter(u8 cnt);    //喂狗函数
void WWDG_NVIC_Init(void);      //配置分组


#endif

