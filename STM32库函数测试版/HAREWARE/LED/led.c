#include "sys.h"
#include "led.h"

void LED_Init(void)
{
	  GPIO_InitTypeDef        GPIO_InitStructure;
	   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);   //使能B端口和E端口时钟
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;   //推挽输出模式
	  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;         //PB5
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  //频率为50MHZ
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
	  GPIO_SetBits(GPIOB,GPIO_Pin_5);             //初始化设为高电平
	  
	  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;         //PE5
	  GPIO_Init(GPIOE,&GPIO_InitStructure);    
	  GPIO_SetBits(GPIOE,GPIO_Pin_5);             //初始化为高电平
}

