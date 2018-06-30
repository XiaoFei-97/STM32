#include "time.h"
#include "sys.h"
#include "led.h"

/**************定时器输出实验*********************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
	 TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitStructure;
	 NVIC_InitTypeDef  NVIC_InitStructure;
	 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     //定时器3时钟使能
	 TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //定时器初始化
	 TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数模式
	 TIM_TimeBaseInitStructure.TIM_Period=arr;   //自动重装值
	 TIM_TimeBaseInitStructure.TIM_Prescaler=psc;    //分频系数
	
	 TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);   //定时器3更新中断使能
	
	 NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;   //中断分组
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	 
	 TIM_Cmd(TIM3,ENABLE);    //使能TIM3
	
   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	 NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)    //定时器3更新中断服务函数
{
   if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)!=RESET)   //判断是否有中断
	 {
	    TIM_ClearFlag(TIM3,TIM_FLAG_Update);  //清除更新中断标志
		  LED0=!LED0;
	 }
}

/***************PWM输出实验*******************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{   
	  GPIO_InitTypeDef   GPIO_InitStructure; 
	  TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	  TIM_OCInitTypeDef   TIM_OCInitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);    //使能TIM3时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //使能端口B时钟和辅助时钟
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;    //PB5推挽输出
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;      
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //频率50MHZ
	  GPIO_Init(GPIOB,&GPIO_InitStructure); 
	  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);  //IO映射
	
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;;    //不分频
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	  TIM_TimeBaseInitStructure.TIM_Period=arr;      //设置重装载值
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;     //设置分频系数
	  TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure); 
	  
	  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;   //开启PWM2模式
	  TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;   //高极性
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //输出使能
	  TIM_OC2Init(TIM3,&TIM_OCInitStructure);
		
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);  //自动预装载
		TIM_Cmd(TIM3,ENABLE);    //使能TIM3计数器
} 

/*******************输出捕获实验**********************/

u8 TIM5CH1_CAPTURE_STA = 0; 
u16 TIM5CH1_CAPTURE_VAL;

void TIM5_Cap_Init(u16 arr,u16 psc)   //输入捕获函数初始化
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	  NVIC_InitTypeDef  NVIC_InitStructure;
	  
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能端口A时钟
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);     //使能TIM5时钟
	   
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;   //下拉输入
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_0);    //PA0下拉输入
	  
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //不分频
	  TIM_TimeBaseInitStructure.TIM_Period=arr;   //设置重装载值
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //设置分频系数
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	  TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;   //通道1
	  TIM_ICInitStructure.TIM_ICFilter=0x00;
		TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;  //上升沿
		TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;  //映射到TI
	  TIM_ICInit(TIM5,&TIM_ICInitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;     //子优先级0
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);    //开启更新中断和捕获1中断
		
		TIM_Cmd(TIM5,ENABLE);    //使能TIM3计数器
}

void TIM5_IRQHandler(void)
{
   if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)  //捕获1事件发生
	 {
	     if(TIM5CH1_CAPTURE_STA&0x40)   //捕获到高电平标志(也就是已经有了下降沿，下一次准备上升)
			 {
			     TIM5CH1_CAPTURE_STA|=0x80;
				   TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
				   TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);  //设置下一次上升沿捕获
			 }else     //没有捕获到高电平标志，意在这是第一次准备上升
			 {
			     TIM5CH1_CAPTURE_STA=0;
				   TIM5CH1_CAPTURE_VAL=0;
				   TIM_SetCounter(TIM5,0);
				   TIM5CH1_CAPTURE_STA|=0x40;  //表示这时有了上升沿
				   TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);   //设置下一次下降沿捕获
			 }
	 }
	 if((TIM5CH1_CAPTURE_STA&0x80)==0)   //还未成功捕获
	 {
	     if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)  //定时器更新发生
			 {
			     if(TIM5CH1_CAPTURE_STA&0x40)  //捕获到上升沿
					 {
					     if((TIM5CH1_CAPTURE_STA&0x3f)==0x3f)  //溢出时间超出bit5~bit0
							 {
							     TIM5CH1_CAPTURE_STA|=0x80;  //此时就让它捕获完成一次
								   TIM5CH1_CAPTURE_VAL=0xffff;
							 }else  TIM5CH1_CAPTURE_STA++;    //没有超出时间就计入bit5~bit0的定时器溢出次数
					 }
			 }
	 }
	 TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);
}
/********PWM DAC**********/
//TIM1 CH1 PWM输出设置 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	
 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //使能TIMx外设
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA外设时钟使能
	
 
  //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2模式	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 低电平有效 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1 预装载使能
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能,高级定时器必须开启这个 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx
	   										  
} 

