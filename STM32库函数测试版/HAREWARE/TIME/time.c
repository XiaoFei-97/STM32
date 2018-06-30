#include "time.h"
#include "sys.h"
#include "led.h"

/**************��ʱ�����ʵ��*********************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
	 TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitStructure;
	 NVIC_InitTypeDef  NVIC_InitStructure;
	 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //��ʱ��3ʱ��ʹ��
	 TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //��ʱ����ʼ��
	 TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���ģʽ
	 TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ���װֵ
	 TIM_TimeBaseInitStructure.TIM_Prescaler=psc;    //��Ƶϵ��
	
	 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);   //��ʱ��3�����ж�ʹ��
	
	 NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;   //�жϷ���
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	 
	 TIM_Cmd(TIM3,ENABLE);    //ʹ��TIM3
	
   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	 NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)    //��ʱ��3�����жϷ�����
{
   if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)!=RESET)   //�ж��Ƿ����ж�
	 {
	    TIM_ClearFlag(TIM3,TIM_FLAG_Update);  //��������жϱ�־
		  LED0=!LED0;
	 }
}

/***************PWM���ʵ��*******************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{   
	  GPIO_InitTypeDef   GPIO_InitStructure; 
	  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	  TIM_OCInitTypeDef   TIM_OCInitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);    //ʹ��TIM3ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //ʹ�ܶ˿�Bʱ�Ӻ͸���ʱ��
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //PB5�������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;      
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //Ƶ��50MHZ
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
	  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //IOӳ��
	
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;;    //����Ƶ
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	  TIM_TimeBaseInitStructure.TIM_Period=arr;      //������װ��ֵ
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;     //���÷�Ƶϵ��
	  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure); 
	  
	  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;   //����PWM2ģʽ
	  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;   //�߼���
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //���ʹ��
	  TIM_OC2Init(TIM3,&TIM_OCInitStructure);
		
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);  //�Զ�Ԥװ��
		TIM_Cmd(TIM3,ENABLE);    //ʹ��TIM3������
} 

/*******************�������ʵ��**********************/

u8 TIM5CH1_CAPTURE_STA = 0; 
u16 TIM5CH1_CAPTURE_VAL;

void TIM5_Cap_Init(u16 arr,u16 psc)   //���벶������ʼ��
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ�ܶ˿�Aʱ��
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);     //ʹ��TIM5ʱ��
	   
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;   //��������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_0);    //PA0��������
	  
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //����Ƶ
	  TIM_TimeBaseInitStructure.TIM_Period=arr;   //������װ��ֵ
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //���÷�Ƶϵ��
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	  TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;   //ͨ��1
	  TIM_ICInitStructure.TIM_ICFilter=0x00;
		TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //������
		TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;  //ӳ�䵽TI
	  TIM_ICInit(TIM5,&TIM_ICInitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;     //�����ȼ�0
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);    //���������жϺͲ���1�ж�
		
		TIM_Cmd(TIM5,ENABLE);    //ʹ��TIM3������
}

void TIM5_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)  //����1�¼�����
	 {
	     if(TIM5CH1_CAPTURE_STA&0x40)   //���񵽸ߵ�ƽ��־(Ҳ�����Ѿ������½��أ���һ��׼������)
			 {
			     TIM5CH1_CAPTURE_STA|=0x80;
				   TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
				   TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //������һ�������ز���
			 }else     //û�в��񵽸ߵ�ƽ��־���������ǵ�һ��׼������
			 {
			     TIM5CH1_CAPTURE_STA=0;
				   TIM5CH1_CAPTURE_VAL=0;
				   TIM_SetCounter(TIM5,0);
				   TIM5CH1_CAPTURE_STA|=0x40;  //��ʾ��ʱ����������
				   TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //������һ���½��ز���
			 }
	 }
	 if((TIM5CH1_CAPTURE_STA&0x80)==0)   //��δ�ɹ�����
	 {
	     if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)  //��ʱ�����·���
			 {
			     if(TIM5CH1_CAPTURE_STA&0x40)  //����������
					 {
					     if((TIM5CH1_CAPTURE_STA&0x3f)==0x3f)  //���ʱ�䳬��bit5~bit0
							 {
							     TIM5CH1_CAPTURE_STA|=0x80;  //��ʱ�������������һ��
								   TIM5CH1_CAPTURE_VAL=0xffff;
							 }else  TIM5CH1_CAPTURE_STA++;    //û�г���ʱ��ͼ���bit5~bit0�Ķ�ʱ���������
					 }
			 }
	 }
	 TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);
}
/********PWM DAC**********/
//TIM1 CH1 PWM������� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	
 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʹ��TIMx����
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOA����ʱ��ʹ��
	
 
  //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1 Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��,�߼���ʱ�����뿪����� 
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx
	   										  
} 

