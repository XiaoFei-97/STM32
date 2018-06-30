#include "sys.h"
#include "led.h"

void LED_Init(void)
{
	  GPIO_InitTypeDef        GPIO_InitStructure;
	   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);   //ʹ��B�˿ں�E�˿�ʱ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;   //�������ģʽ
	  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;         //PB5
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  //Ƶ��Ϊ50MHZ
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
	  GPIO_SetBits(GPIOB,GPIO_Pin_5);             //��ʼ����Ϊ�ߵ�ƽ
	  
	  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;         //PE5
	  GPIO_Init(GPIOE,&GPIO_InitStructure);    
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);             //��ʼ��Ϊ�ߵ�ƽ
}

