#include "sys.h"
#include "led.h"
#include "time.h"

/*******************定时器中断**********************/
void TIM3_Int_Init(u16 arr,u16 psc)  //定时器3的初始化
{
    RCC->APB1ENR |= 1<<1;  //定时器3时钟使能
	  TIM3->ARR = arr;      //设置重装载值
	  TIM3->PSC = psc;     //设置分频系数
	  TIM3->DIER |= 1<<0;    //开启定时器更新中断
	  MY_NVIC_Init(1,3,TIM3_IRQn,2);  //抢占0，子优先3，分组2
	  TIM3->CR1 |= 1<<0;     //使能定时器3
}

void TIM3_IRQHandler(void)   //定时器3的更新中断服务函数
{
    if(TIM3->SR&0X0001)    //最好不要用SR==1<<0来判断，会影响其他位
		{
		    LED1=!LED1;    //LED1翻转
		}
		TIM3->SR &= ~(1<<0);    //清除更新中断标志
}

/*********************PWM输出实验**********************/
void TIM3_Pwm_Init(u16 arr,u16 psc)
{ 
    RCC->APB1ENR |= 1<<1;   //开启TIM3时钟
    RCC->APB2ENR |= 1<<3;   //开启端口B时钟
	  GPIOB->CRL &= 0XFF0FFFFF;   
	  GPIOB->CRL |= 0X00B00000;   //复用推挽输出，50MHZ
	
	  RCC->APB2ENR |= 1<<0;   //开启辅助时钟
	  AFIO->MAPR |= 1<<11;   //部分映像CH2从PA7到PB5
	  
	  TIM3->ARR = arr;     //重装载值
	  TIM3->PSC = psc;     //分频系数
	  
	  TIM3->CCMR1 |= 7<<12;   //输出比较2模式
	  TIM3->CCMR1 |= 1<<11;   //CH2装载使能
	  
	  TIM3->CCER |= 1<<4;    //0C2输出使能
	  TIM3->CR1 |= 0X0080;  //重装载使能
	  TIM3->CR1 |= 0X01;    //启动计数器
}

/****************输入捕获实验********************/
u8 TIM5_CAPTURE_STA = 0;
u16 TIM5CH1_CAPTURE_VAL;
void TIM5_Cap_Init(u16 arr,u16 psc)
{
    RCC->APB2ENR |= 1<<2;      //使能端口A时钟
	  RCC->APB1ENR |= 1<<3;      //使能TIM5时钟
	
	  GPIOA->CRL &= 0XFFFFFFF0;   //PA0下拉输入
	  GPIOA->CRL |= 0X00000008;
	  GPIOA->ODR |= 0<<0;      
	  
	  TIM5->ARR = arr;    //重装载值
	  TIM5->PSC = psc;    //分频系数
	
	  TIM5->CCMR1 |= 1<<0;   //IC1映射在TI1上
	  TIM5->CCMR1 |= 0<<4;    //设置无滤波
	  TIM5->CCMR1 |= 0<<10;     //配置输入分频，不分频
	  
	  TIM5->CCER |= 0<<1;   //上升沿捕获
	  TIM5->CCER |= 1<<0;    //允许捕获到寄存器
	  TIM5->DIER |= 1<<0;   //开启更新中断
	  TIM5->DIER |= 1<<1;    //开启捕获中断
	  TIM5->CR1 |= 0X01;    //开启计数器
	
	  MY_NVIC_Init(2,0,TIM5_IRQn,2);   //中断分组，抢占2子优先0，分组2
}

void TIM5_IRQHandler(void)     //定时器5中断服务函数
{
    u16 tsr;
	  tsr = TIM5->SR;
	  if((TIM5_CAPTURE_STA&0X80)==0)    //如果还未成功捕获
		{
		    if(tsr&0x01)    //如果产生更新中断（即此时有了溢出）
				{
				     if(TIM5_CAPTURE_STA&0X40)   //已经捕获到高电平
						 {
						    if((TIM5_CAPTURE_STA&0X3F)==0X3F) //高电平时间太长
								{
								     TIM5_CAPTURE_STA |= 0X80;   //就算成功捕获一次
									   TIM5CH1_CAPTURE_VAL = 0XFFFF;
								}
								else  TIM5_CAPTURE_STA++;
						 }
				}
				if(tsr&0x02)    //如果产生捕获中断
				{
				    if(TIM5_CAPTURE_STA&0X40)    //已经捕获到下降沿
						{
						   TIM5_CAPTURE_STA |= 0X80; //标记成功捕获到脉宽
							 TIM5CH1_CAPTURE_VAL  = TIM5->CCR1;  //获取当前捕获值
							 TIM5->CCER &= ~(1<<1);    //设为上升沿捕获
						}
						else
						{
						   TIM5_CAPTURE_STA = 0;   //清空
							 TIM5CH1_CAPTURE_VAL = 0;    
							 TIM5_CAPTURE_STA |= 0x40;   //标记捕获到了上升沿
							 TIM5->CNT = 0;         //计数器清空
							 TIM5->CCER |= 1<<1;    //设为下降沿捕获
						}
				}
		}
		TIM5->SR = 0;    //清除中断标志位
}
/***********PWM DAC************/
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置  
	RCC->APB2ENR|=1<<2;   	//使能PORTA时钟	 
	RCC->APB2ENR|=1<<11;   	//使能TIM1时钟	
 	  	
	GPIOA->CRH&=0XFFFFFFF0;	//PA8输出
	GPIOA->CRH|=0X0000000B;	//复用功能输出 	  
 
	TIM1->ARR=arr;			//设定计数器自动重装值 
	TIM1->PSC=psc;			//预分频器分频设置
	
	TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式		 
	TIM1->CCMR1|=1<<3; 		//CH1 预装载使能	   

	TIM1->CCER|=1<<1;   	//OC1 低电平有效 
	TIM1->CCER|=1<<0;   	//OC1 输出使能	
	TIM1->BDTR|=1<<15;   	//MOE 主输出使能,高级定时器必须开启这个  

	TIM1->CR1=0x0080;   	//ARPE使能 
	TIM1->CR1|=0x01;    	//使能定时器1											  
} 


