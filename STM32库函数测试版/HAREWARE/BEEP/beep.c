#include "beep.h"
#include "stm32f10x.h"

void BEEP_Init(void)
{
	 GPIO_InitTypeDef   GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ�ܶ˿�Bʱ��
	
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;    //�������ģʽ
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;           //PB8
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;   //Ƶ��Ϊ50MHZ
   GPIO_Init(GPIOB,&GPIO_InitStructure);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_8);                 //��ʼ��Ϊ�͵�ƽ
}

