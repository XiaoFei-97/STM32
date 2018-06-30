#include "sys.h"
#include "delay.h"
#include "key.h"

void KEY_Init(void)
{
	
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);   //使能端口A时钟和端口E时钟
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;       //上拉输入
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;   //PE2，3，4
	  GPIO_Init(GPIOE,&GPIO_InitStructure);   //(！！需要注意输入模式下不需要频率)
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;       //下拉输入
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;           //PA0
	  GPIO_Init(GPIOA,&GPIO_InitStructure);   //(！！需要注意输入模式下不需要频率)
}

u8 KEY_Scan(u8 mode)     //mode=0不支持连按，mode=1支持连按
{
    static u8 key_up = 1;      //静态变量key_up=1
	  if(mode)  key_up = 1;      //mode=1时执行此句
	  if(key_up && (KEY0==0||KEY1==0||KEY2==0||WK_UP==1)) //判断是否有按键按下
		{
		   delay_ms(10);
			 key_up=0;
			 if(KEY0==0)   return 1;  //KEY0是否按下
			 if(KEY1==0)   return 2;  //KEY1是否按下
			 if(KEY2==0)   return 3;  //KEY2是否按下
			 if(WK_UP==1)  return 4;  //WK_UP是否按下
		}else if
		(KEY0==1 && KEY1==1 && KEY2==1 && WK_UP==0)  key_up=1;  //没有按键按下
		return 0;		
}

