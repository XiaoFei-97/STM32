#include "sys.h"
#include "wdg.h"
#include "led.h"

/********独立看门狗**********/
void IWDG_Init(u8 prer,u16 rlr)     //独立看门狗初始化，分频系数及重装载值
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //取消对RLR和PR寄存器写保护
	  IWDG_SetPrescaler(prer);     //设置分频系数
	  IWDG_SetReload(rlr);        //设置重装载值
	  IWDG_ReloadCounter();       //重装载一次
	  IWDG_Enable();             //使能独立看门狗
}
void IWDG_Feed(void)    //喂狗函数
{
    IWDG_ReloadCounter();     //重装载寄存器
}


/*************窗口看门狗**************/
u8 WWDG_CNT=0x7f;
void WWDG_Init(u8 tr,u8 wr,u32 fprer)    //窗口看门狗初始化，当前计数值，上窗口值，分频时基
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);    //开启WWDG时钟（36MHZ）
	 WWDG_CNT = tr&WWDG_CNT;       //当前计数值
   WWDG_SetPrescaler(fprer);    //设置分频系数
	 WWDG_SetWindowValue(wr);     //设置上窗口值
	 WWDG_Enable(WWDG_CNT);       //装载计数器一次
	 WWDG_ClearFlag();       //清除提前中断标志
	 WWDG_NVIC_Init();      //中断分组
	 WWDG_EnableIT();      //唤醒提前中断
} 
void WWDG_Set_Counter(u8 cnt)   //喂狗函数
{ 
   WWDG_Enable(cnt);     //装载计数器
}
void WWDG_NVIC_Init(void)
{
   NVIC_InitTypeDef  NVIC_InitStrcture;
   NVIC_InitStrcture.NVIC_IRQChannel=WWDG_IRQn;
   NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=2;   //抢占优先级2
   NVIC_InitStrcture.NVIC_IRQChannelSubPriority=3;   //子优先级3
   NVIC_Init(&NVIC_InitStrcture);	
}
void WWDG_IRQHandler(void)    //提前中断服务函数
{
  // WWDG_Set_Counter(WWDG_CNT); //（如果注释掉这段，则程序复位）
	 WWDG_ClearFlag();   //清除提前中断标志
	 LED1=!LED1;     //翻转LED1
} 

