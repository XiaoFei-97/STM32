#ifndef __KEY_H_
#define __KEY_H_
#include "sys.h"

#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)    //PE2输入用KEY2表示
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)    //PE3输入用KEY1表示
#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)    //PE2输入用KEY0表示
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)   //PA0输入用WK_UP表示

void KEY_Init(void);    //按键初始化函数
u8 KEY_Scan(u8 mode);   //键值输入函数

#endif

