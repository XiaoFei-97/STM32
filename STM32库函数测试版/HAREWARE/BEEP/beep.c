#include "beep.h"
#include "stm32f10x.h"

void BEEP_Init(void)
{
	 GPIO_InitTypeDef   GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能端口B时钟
	
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //推挽输出模式
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;           //PB8
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;   //频率为50MHZ
   GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_8);                 //初始设为低电平
}

