#include "sys.h"
#include "wdg.h"
#include "led.h"

/********�������Ź�**********/
void IWDG_Init(u8 prer,u16 rlr)     //�������Ź���ʼ������Ƶϵ������װ��ֵ
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   //ȡ����RLR��PR�Ĵ���д����
	  IWDG_SetPrescaler(prer);     //���÷�Ƶϵ��
	  IWDG_SetReload(rlr);        //������װ��ֵ
	  IWDG_ReloadCounter();       //��װ��һ��
	  IWDG_Enable();             //ʹ�ܶ������Ź�
}
void IWDG_Feed(void)    //ι������
{
    IWDG_ReloadCounter();     //��װ�ؼĴ���
}


/*************���ڿ��Ź�**************/
u8 WWDG_CNT=0x7f;
void WWDG_Init(u8 tr,u8 wr,u32 fprer)    //���ڿ��Ź���ʼ������ǰ����ֵ���ϴ���ֵ����Ƶʱ��
{
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);    //����WWDGʱ�ӣ�36MHZ��
	 WWDG_CNT = tr&WWDG_CNT;       //��ǰ����ֵ
   WWDG_SetPrescaler(fprer);    //���÷�Ƶϵ��
	 WWDG_SetWindowValue(wr);     //�����ϴ���ֵ
	 WWDG_Enable(WWDG_CNT);       //װ�ؼ�����һ��
	 WWDG_ClearFlag();       //�����ǰ�жϱ�־
	 WWDG_NVIC_Init();      //�жϷ���
	 WWDG_EnableIT();      //������ǰ�ж�
} 
void WWDG_Set_Counter(u8 cnt)   //ι������
{ 
   WWDG_Enable(cnt);     //װ�ؼ�����
}
void WWDG_NVIC_Init(void)
{
   NVIC_InitTypeDef  NVIC_InitStrcture;
   NVIC_InitStrcture.NVIC_IRQChannel=WWDG_IRQn;
   NVIC_InitStrcture.NVIC_IRQChannelCmd=ENABLE;
   NVIC_InitStrcture.NVIC_IRQChannelPreemptionPriority=2;   //��ռ���ȼ�2
   NVIC_InitStrcture.NVIC_IRQChannelSubPriority=3;   //�����ȼ�3
   NVIC_Init(&NVIC_InitStrcture);	
}
void WWDG_IRQHandler(void)    //��ǰ�жϷ�����
{
  // WWDG_Set_Counter(WWDG_CNT); //�����ע�͵���Σ������λ��
	 WWDG_ClearFlag();   //�����ǰ�жϱ�־
	 LED1=!LED1;     //��תLED1
} 

