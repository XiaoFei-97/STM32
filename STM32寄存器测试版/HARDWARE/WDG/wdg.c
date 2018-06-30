#include "sys.h"
#include "wdg.h"
#include "led.h"

/**************�������Ź�**********************/
void IWDG_Init(u8 prer,u16 rlr)
{
    IWDG->KR = 0X5555;   //ȡ��PR��RLR��д����
	  IWDG->PR = prer;     //Ԥ��Ƶ����4
	  IWDG->RLR = rlr;       //������װ��ֵ
	  IWDG->KR = 0XAAAA;    //��װһ��
    IWDG->KR = 0XCCCC;   //���ö������Ź�
}

void IWDG_Feed(void)    //ι������
{
   IWDG->KR = 0XAAAA;    
}


/*****************���ڿ��Ź�*********************/
u8 WWDG_CNT=0x7f;
void WWDG_Init(u8 tr,u8 wr,u8 fprer)
{
	  RCC->APB1ENR|=1<<11;   //ʹ��APB1�ϵ�WWDGʱ��
	  WWDG_CNT=tr&WWDG_CNT;
	  WWDG->CFR|=fprer<<7;  //���÷�Ƶ����
	  WWDG->CFR&=0XFF80;     
	  WWDG->CFR|=wr;          //�����ϴ���ֵ
	  WWDG->CR|=WWDG_CNT;
	  WWDG->CR|=1<<7;       //ʹ�ܴ��ڿ��Ź�
	  MY_NVIC_Init(2,3,WWDG_IRQn,2);  //�ж�����
	  WWDG->SR=0X00;     //�����ǰ�жϱ�־
	  WWDG->CFR|=1<<9;    //ʹ����ǰ�жϱ�־
}

void WWDG_Set_Counter(u8 cnt)   //ι������
{
    WWDG->CR = (cnt&0x7F); 
}
void WWDG_IRQHandler(void)   //WWDG��ǰ�жϷ�����
{
    WWDG_Set_Counter(WWDG_CNT);   //ι��һ��
	  WWDG->SR = 0X00;    //�����ǰ�жϱ�־
		LED1=!LED1;   //��תLED1
}

