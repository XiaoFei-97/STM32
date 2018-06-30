#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"

void EXTIx_Init(void)
{   
 	  EXTI_InitTypeDef   EXTI_InitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	
    KEY_Init();   //对按键端口初始化
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //使能端口复用中断
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);  //配置中断线PE2的映射关系
	  EXTI_InitStructure.EXTI_Line=EXTI_Line2;    //中断线2
	  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;   //中断模式
	  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;   //下降沿触发
	  EXTI_Init(&EXTI_InitStructure);
	  
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);   //配置中断线PE3的映射关系
	  EXTI_InitStructure.EXTI_Line=EXTI_Line3;              //中断线3
  	EXTI_Init(&EXTI_InitStructure);
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);   //配置中断线PE4的映射关系
	  EXTI_InitStructure.EXTI_Line=EXTI_Line4;             //中断线4
  	EXTI_Init(&EXTI_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);   //配置中短线PA0的映射关系
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;              //中断线0
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;     //配置中断线2，使能通道及优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
		NVIC_Init(&NVIC_InitStructure);	
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;   //配置中断线3，使能通道及优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
		NVIC_Init(&NVIC_InitStructure);	
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;    //配置中断线4，使能通道及优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	  NVIC_Init(&NVIC_InitStructure);	
		 
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;   //配置中断线0，使能通道及优先级
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	  NVIC_Init(&NVIC_InitStructure);	
}

void EXTI2_IRQHandler(void)      //2号中断的服务函数
{
    delay_ms(10);
	  if(KEY2==0)  
		{
		    BEEP=~BEEP;
		}	  
		EXTI_ClearITPendingBit(EXTI_Line2);   //清除2号中断标志位
		
} 
void EXTI3_IRQHandler(void)    //3号中断的服务函数
{
    delay_ms(10);
	  if(KEY1==0)  
		{
		    LED0=~LED0; 
		}
		EXTI_ClearITPendingBit(EXTI_Line3);    //清除3号中断标志位
}
void EXTI4_IRQHandler(void)  //4号中断的服务函数
{
    delay_ms(10);
	  if(KEY0==0)  
		{
		    LED0=~LED0;	  
		}
		EXTI_ClearITPendingBit(EXTI_Line4);      //清除4号中断标志位
}
void EXTI0_IRQHandler(void)   //0号中断的服务函数
{
    delay_ms(10);
	  if(WK_UP==1)  
		{
		    LED0=~LED0;LED1=~LED1;  
		}
		EXTI_ClearITPendingBit(EXTI_Line0);   //清除0号中断标志位
}

