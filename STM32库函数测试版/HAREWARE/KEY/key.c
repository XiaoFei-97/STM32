#include "sys.h"
#include "delay.h"
#include "key.h"

void KEY_Init(void)
{
	
	  GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);   //ʹ�ܶ˿�Aʱ�ӺͶ˿�Eʱ��
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;       //��������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;   //PE2��3��4
	  GPIO_Init(GPIOE,&GPIO_InitStructure);   //(������Ҫע������ģʽ�²���ҪƵ��)
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;       //��������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;           //PA0
	  GPIO_Init(GPIOA,&GPIO_InitStructure);   //(������Ҫע������ģʽ�²���ҪƵ��)
}

u8 KEY_Scan(u8 mode)     //mode=0��֧��������mode=1֧������
{
    static u8 key_up = 1;      //��̬����key_up=1
	  if(mode)  key_up = 1;      //mode=1ʱִ�д˾�
	  if(key_up && (KEY0==0||KEY1==0||KEY2==0||WK_UP==1)) //�ж��Ƿ��а�������
		{
		   delay_ms(10);
			 key_up=0;
			 if(KEY0==0)   return 1;  //KEY0�Ƿ���
			 if(KEY1==0)   return 2;  //KEY1�Ƿ���
			 if(KEY2==0)   return 3;  //KEY2�Ƿ���
			 if(WK_UP==1)  return 4;  //WK_UP�Ƿ���
		}else if
		(KEY0==1 && KEY1==1 && KEY2==1 && WK_UP==0)  key_up=1;  //û�а�������
		return 0;		
}

