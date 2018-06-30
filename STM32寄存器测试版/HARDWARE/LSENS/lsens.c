#include "lsens.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//LSENS(光敏传感器)驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
 
//初始化光敏传感器
void Lsens_Init(void)
{
 	RCC->APB2ENR|=1<<7;    //使能PORTF口时钟  
	GPIOF->CRH&=0XFFFFFFF0;//PF8 anolog输入
	RCC->APB2ENR|=1<<15;   //ADC3时钟使能	  
	RCC->APB2RSTR|=1<<15;   //ADC复位
	RCC->APB2RSTR&=~(1<<15);//复位结束	    
	RCC->CFGR&=~(3<<14);   //分频因子清零	 
	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR|=2<<14;      	 
	ADC3->CR1&=0XF0FFFF;   //工作模式清零
	ADC3->CR1|=0<<16;      //独立工作模式  
	ADC3->CR1&=~(1<<8);    //非扫描模式	  
	ADC3->CR2&=~(1<<1);    //单次转换模式
	ADC3->CR2&=~(7<<17);	   
	ADC3->CR2|=7<<17;	   //软件控制转换  
	ADC3->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC3->CR2&=~(1<<11);   //右对齐	   
	ADC3->SQR1&=~(0XF<<20);
	ADC3->SQR1|=0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 			   
	//设置通道1的采样时间
	ADC3->SMPR2&=~(7<<(3*6));//通道6采样时间清空	  
 	ADC3->SMPR2|=7<<(3*6); //通道6  239.5个周期,提高采样时间可以提高精确度	

	ADC3->CR2|=1<<0;	   //开启AD转换器	 
	ADC3->CR2|=1<<3;       //使能复位校准  
	while(ADC1->CR2&1<<3); //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC3->CR2|=1<<2;        //开启AD校准	   
	while(ADC3->CR2&1<<2);  //等待校准结束
	//该位由软件设置以开始校准，并在校准结束时由硬件清除  
}
u16 Get_Adc3(u8 ch)   
{
	//设置转换序列	  		 
	ADC3->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC3->SQR3|=ch;		  			    
	ADC3->CR2|=1<<22;       //启动规则转换通道 
	while(!(ADC3->SR&1<<1));//等待转换结束	 	   
	return ADC3->DR;		//返回adc值	
} 
//读取Light Sens的值
//0~100:0,最暗;100,最亮 
u8 Lsens_Get_Val(void)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<LSENS_READ_TIMES;t++)
	{
		temp_val+=Get_Adc3(LSENS_ADC_CHX);	//读取ADC值
		delay_ms(5);
	}
	temp_val/=LSENS_READ_TIMES;//得到平均值 
	if(temp_val>4000)temp_val=4000;
	return (u8)(100-(temp_val/40));
}














