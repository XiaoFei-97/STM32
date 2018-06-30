#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tpad.h"

u16 tpad_default_val = 0;    //初始化的充电时间平均值
#define TPAD_GATE_VAL 100   //门限值
#define TPAD_ARR_MAX_VAL 0XFFFF  //最大装载值

u8 TPAD_Init(u8 psc)
{
    u16 buf[10];     //捕获返回的十个数据
	  u16 temp;
	  u8 i,j;
	  TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);  //以1MHZ的频率计数
	  for(i=0;i<10;i++)
	 {
	     buf[i]=TPAD_Get_Val();    //捕获到的值存入buf数组中  
	     delay_ms(10);   
	 }
	 for(i=0;i<9;i++)     //给这十个值进行升序排列
	 {
	     for(j=i+1;j<10;j++)
		   {
					if(buf[i]>buf[j])
					{
					     temp = buf[i];
						   buf[i]=buf[j];
						   buf[j]=temp;
					}
			 }
	 }
	 temp = 0;    //升序已经使用完了temp,这里重新归零
	 for(i=2;i<8;i++)  temp+=buf[i];  //对中间的6个数据进行累加
	 tpad_default_val = temp/6;  //此时已经得出这6个数据的平均值
	 printf("tpad_default_val:%d\r\n",tpad_default_val);   //打印出这个空载的电容充电平均时间
	 if(tpad_default_val>TPAD_ARR_MAX_VAL/2)   return 1;   //!!为什么初始化不正常是1
	 return 0;
}
void TPAD_Reset(void)
{
	  GPIO_InitTypeDef   GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //开启端口A时钟
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;   //PA1推挽输出
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_1);   //输出0释放电容电量
	  delay_ms(5);
	  TIM_SetCounter(TIM5,0);    //归零
	  TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC2);
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //放电后，设为浮空输入
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
u16 TPAD_Get_Val(void)
{
    TPAD_Reset();   //tpad先复位
	  while((TIM_GetFlagStatus(TIM5,TIM_IT_CC2))==RESET)  //等待溢出，则返回捕获值
		{
		    if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL - 500)     //!!为什么要减500？
				return TIM_GetCounter(TIM5);  //如果超时就返回计数器的值
		}
		return TIM_GetCapture2(TIM5);
}

u16 TPAD_Get_MaxVal(u8 n)    //n个数据，取出最大值
{
    u16 temp = 0;
		u16 res = 0;
	  while(n--)
		{
		    temp = TPAD_Get_Val();
				if(temp>res)  res = temp;
		}
		return res;
}

u8 TPAD_Scan(u8 mode)  //mode=0不支持连续触发，mode=1则支持
{
    static u8 keyen = 0;  //0可以开始检测，>0不能检测
	  u8 res =0;   
		u8 sample =3;    //默认采样次数为3次
	  u16 rval;    //采样的最大值
	  if(mode)   
		{   
		   sample = 6;   //支持连按的时候，采样为6次
			 keyen = 0;   //支持连按
		}
		rval = TPAD_Get_MaxVal(sample); 
		if(rval>(tpad_default_val+TPAD_GATE_VAL))   //如果充电时间大于初始平均值+门限值，则触发输入
		{
		   if(keyen==0)  res = 1;   //keyen=0,有效
			 printf("r:%d\r\n",rval);
			 keyen = 3;   //至少要过3次按键才有效，此处巧妙的避免了按键连续触发的效果
		} 
		if(keyen)  keyen--;
		return res;
}


void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{  
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //开启端口A时钟和TIM5时钟
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //设为浮空输入
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	  
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //tds=tck_tim
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;  //向上计数
	  TIM_TimeBaseInitStructure.TIM_Period=arr;   //重装载值
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //分频系数
	  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	  
	  TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;    //选择输入端IC2映射到TI5上
	  TIM_ICInitStructure.TIM_ICFilter=0x03;    //8个定时器时钟滤波
	  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPSC_DIV1;    //配置输入分频，不分频
	  TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPolarity_Rising;    //上升沿触发捕获
	  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //
	  TIM_ICInit(TIM5,&TIM_ICInitStructure);
		
		TIM_Cmd(TIM5,ENABLE);    //开启计数器			
}

