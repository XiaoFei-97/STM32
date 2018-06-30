#include "exti.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"

void EXTIx_Init(void)
{   
 	  EXTI_InitTypeDef   EXTI_InitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	
    KEY_Init();   //�԰����˿ڳ�ʼ��
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //ʹ�ܶ˿ڸ����ж�
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);  //�����ж���PE2��ӳ���ϵ
	  EXTI_InitStructure.EXTI_Line=EXTI_Line2;    //�ж���2
	  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;   //�ж�ģʽ
	  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;   //�½��ش���
	  EXTI_Init(&EXTI_InitStructure);
	  
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);   //�����ж���PE3��ӳ���ϵ
	  EXTI_InitStructure.EXTI_Line=EXTI_Line3;              //�ж���3
  	EXTI_Init(&EXTI_InitStructure);
	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);   //�����ж���PE4��ӳ���ϵ
	  EXTI_InitStructure.EXTI_Line=EXTI_Line4;             //�ж���4
  	EXTI_Init(&EXTI_InitStructure);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);   //�����ж���PA0��ӳ���ϵ
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;              //�ж���0
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;     //�����ж���2��ʹ��ͨ�������ȼ�
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
		NVIC_Init(&NVIC_InitStructure);	
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;   //�����ж���3��ʹ��ͨ�������ȼ�
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
		NVIC_Init(&NVIC_InitStructure);	
	
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;    //�����ж���4��ʹ��ͨ�������ȼ�
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	  NVIC_Init(&NVIC_InitStructure);	
		 
	  NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;   //�����ж���0��ʹ��ͨ�������ȼ�
	  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	  NVIC_Init(&NVIC_InitStructure);	
}

void EXTI2_IRQHandler(void)      //2���жϵķ�����
{
    delay_ms(10);
	  if(KEY2==0)  
		{
		    BEEP=~BEEP;
		}	  
		EXTI_ClearITPendingBit(EXTI_Line2);   //���2���жϱ�־λ
		
} 
void EXTI3_IRQHandler(void)    //3���жϵķ�����
{
    delay_ms(10);
	  if(KEY1==0)  
		{
		    LED0=~LED0; 
		}
		EXTI_ClearITPendingBit(EXTI_Line3);    //���3���жϱ�־λ
}
void EXTI4_IRQHandler(void)  //4���жϵķ�����
{
    delay_ms(10);
	  if(KEY0==0)  
		{
		    LED0=~LED0;	  
		}
		EXTI_ClearITPendingBit(EXTI_Line4);      //���4���жϱ�־λ
}
void EXTI0_IRQHandler(void)   //0���жϵķ�����
{
    delay_ms(10);
	  if(WK_UP==1)  
		{
		    LED0=~LED0;LED1=~LED1;  
		}
		EXTI_ClearITPendingBit(EXTI_Line0);   //���0���жϱ�־λ
}

