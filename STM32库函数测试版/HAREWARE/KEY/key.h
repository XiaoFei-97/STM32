#ifndef __KEY_H_
#define __KEY_H_
#include "sys.h"

#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)    //PE2������KEY2��ʾ
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)    //PE3������KEY1��ʾ
#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)    //PE2������KEY0��ʾ
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)   //PA0������WK_UP��ʾ

void KEY_Init(void);    //������ʼ������
u8 KEY_Scan(u8 mode);   //��ֵ���뺯��

#endif

