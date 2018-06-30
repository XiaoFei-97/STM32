#include "sys.h"
#include "led.h"
#include "time.h"

/*******************��ʱ���ж�**********************/
void TIM3_Int_Init(u16 arr,u16 psc)  //��ʱ��3�ĳ�ʼ��
{
    RCC->APB1ENR |= 1<<1;  //��ʱ��3ʱ��ʹ��
	  TIM3->ARR = arr;      //������װ��ֵ
	  TIM3->PSC = psc;     //���÷�Ƶϵ��
	  TIM3->DIER |= 1<<0;    //������ʱ�������ж�
	  MY_NVIC_Init(1,3,TIM3_IRQn,2);  //��ռ0��������3������2
	  TIM3->CR1 |= 1<<0;     //ʹ�ܶ�ʱ��3
}

void TIM3_IRQHandler(void)   //��ʱ��3�ĸ����жϷ�����
{
    if(TIM3->SR&0X0001)    //��ò�Ҫ��SR==1<<0���жϣ���Ӱ������λ
		{
		    LED1=!LED1;    //LED1��ת
		}
		TIM3->SR &= ~(1<<0);    //��������жϱ�־
}

/*********************PWM���ʵ��**********************/
void TIM3_Pwm_Init(u16 arr,u16 psc)
{ 
    RCC->APB1ENR |= 1<<1;   //����TIM3ʱ��
    RCC->APB2ENR |= 1<<3;   //�����˿�Bʱ��
	  GPIOB->CRL &= 0XFF0FFFFF;   
	  GPIOB->CRL |= 0X00B00000;   //�������������50MHZ
	
	  RCC->APB2ENR |= 1<<0;   //��������ʱ��
	  AFIO->MAPR |= 1<<11;   //����ӳ��CH2��PA7��PB5
	  
	  TIM3->ARR = arr;     //��װ��ֵ
	  TIM3->PSC = psc;     //��Ƶϵ��
	  
	  TIM3->CCMR1 |= 7<<12;   //����Ƚ�2ģʽ
	  TIM3->CCMR1 |= 1<<11;   //CH2װ��ʹ��
	  
	  TIM3->CCER |= 1<<4;    //0C2���ʹ��
	  TIM3->CR1 |= 0X0080;  //��װ��ʹ��
	  TIM3->CR1 |= 0X01;    //����������
}

/****************���벶��ʵ��********************/
u8 TIM5_CAPTURE_STA = 0;
u16 TIM5CH1_CAPTURE_VAL;
void TIM5_Cap_Init(u16 arr,u16 psc)
{
    RCC->APB2ENR |= 1<<2;      //ʹ�ܶ˿�Aʱ��
	  RCC->APB1ENR |= 1<<3;      //ʹ��TIM5ʱ��
	
	  GPIOA->CRL &= 0XFFFFFFF0;   //PA0��������
	  GPIOA->CRL |= 0X00000008;
	  GPIOA->ODR |= 0<<0;      
	  
	  TIM5->ARR = arr;    //��װ��ֵ
	  TIM5->PSC = psc;    //��Ƶϵ��
	
	  TIM5->CCMR1 |= 1<<0;   //IC1ӳ����TI1��
	  TIM5->CCMR1 |= 0<<4;    //�������˲�
	  TIM5->CCMR1 |= 0<<10;     //���������Ƶ������Ƶ
	  
	  TIM5->CCER |= 0<<1;   //�����ز���
	  TIM5->CCER |= 1<<0;    //�����񵽼Ĵ���
	  TIM5->DIER |= 1<<0;   //���������ж�
	  TIM5->DIER |= 1<<1;    //���������ж�
	  TIM5->CR1 |= 0X01;    //����������
	
	  MY_NVIC_Init(2,0,TIM5_IRQn,2);   //�жϷ��飬��ռ2������0������2
}

void TIM5_IRQHandler(void)     //��ʱ��5�жϷ�����
{
    u16 tsr;
	  tsr = TIM5->SR;
	  if((TIM5_CAPTURE_STA&0X80)==0)    //�����δ�ɹ�����
		{
		    if(tsr&0x01)    //������������жϣ�����ʱ���������
				{
				     if(TIM5_CAPTURE_STA&0X40)   //�Ѿ����񵽸ߵ�ƽ
						 {
						    if((TIM5_CAPTURE_STA&0X3F)==0X3F) //�ߵ�ƽʱ��̫��
								{
								     TIM5_CAPTURE_STA |= 0X80;   //����ɹ�����һ��
									   TIM5CH1_CAPTURE_VAL = 0XFFFF;
								}
								else  TIM5_CAPTURE_STA++;
						 }
				}
				if(tsr&0x02)    //������������ж�
				{
				    if(TIM5_CAPTURE_STA&0X40)    //�Ѿ������½���
						{
						   TIM5_CAPTURE_STA |= 0X80; //��ǳɹ���������
							 TIM5CH1_CAPTURE_VAL  = TIM5->CCR1;  //��ȡ��ǰ����ֵ
							 TIM5->CCER &= ~(1<<1);    //��Ϊ�����ز���
						}
						else
						{
						   TIM5_CAPTURE_STA = 0;   //���
							 TIM5CH1_CAPTURE_VAL = 0;    
							 TIM5_CAPTURE_STA |= 0x40;   //��ǲ�����������
							 TIM5->CNT = 0;         //���������
							 TIM5->CCER |= 1<<1;    //��Ϊ�½��ز���
						}
				}
		}
		TIM5->SR = 0;    //����жϱ�־λ
}
/***********PWM DAC************/
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������  
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTAʱ��	 
	RCC->APB2ENR|=1<<11;   	//ʹ��TIM1ʱ��	
 	  	
	GPIOA->CRH&=0XFFFFFFF0;	//PA8���
	GPIOA->CRH|=0X0000000B;	//���ù������ 	  
 
	TIM1->ARR=arr;			//�趨�������Զ���װֵ 
	TIM1->PSC=psc;			//Ԥ��Ƶ����Ƶ����
	
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
	TIM1->CCMR1|=1<<3; 		//CH1 Ԥװ��ʹ��	   

	TIM1->CCER|=1<<1;   	//OC1 �͵�ƽ��Ч 
	TIM1->CCER|=1<<0;   	//OC1 ���ʹ��	
	TIM1->BDTR|=1<<15;   	//MOE �����ʹ��,�߼���ʱ�����뿪�����  

	TIM1->CR1=0x0080;   	//ARPEʹ�� 
	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1											  
} 


