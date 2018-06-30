#include "sys.h"
#include "wdg.h"
#include "led.h"

/**************独立看门狗**********************/
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG->KR = 0X5555;   //取消PR和RLR的写保护
	  IWDG->PR = prer;     //预分频因子4
	  IWDG->RLR = rlr;       //设置重装载值
	  IWDG->KR = 0XAAAA;    //重装一次
    IWDG->KR = 0XCCCC;   //启用独立看门狗
}

void IWDG_Feed(void)    //喂狗函数
{
   IWDG->KR = 0XAAAA;    
}


/*****************窗口看门狗*********************/
u8 WWDG_CNT=0x7f;
void WWDG_Init(u8 tr,u8 wr,u8 fprer)
{
	  RCC->APB1ENR|=1<<11;   //使能APB1上的WWDG时钟
	  WWDG_CNT=tr&WWDG_CNT;
	  WWDG->CFR|=fprer<<7;  //设置分频基数
	  WWDG->CFR&=0XFF80;     
	  WWDG->CFR|=wr;          //设置上窗口值
	  WWDG->CR|=WWDG_CNT;
	  WWDG->CR|=1<<7;       //使能窗口看门狗
	  MY_NVIC_Init(2,3,WWDG_IRQn,2);  //中断配置
	  WWDG->SR=0X00;     //清除提前中断标志
	  WWDG->CFR|=1<<9;    //使能提前中断标志
}

void WWDG_Set_Counter(u8 cnt)   //喂狗函数
{
    WWDG->CR = (cnt&0x7F); 
}
void WWDG_IRQHandler(void)   //WWDG提前中断服务函数
{
    WWDG_Set_Counter(WWDG_CNT);   //喂狗一次
	  WWDG->SR = 0X00;    //清除提前中断标志
		LED1=!LED1;   //翻转LED1
}

