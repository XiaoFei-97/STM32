#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "tpad.h"

u16 tpad_default_val = 0;    //��ʼ���ĳ��ʱ��ƽ��ֵ
#define TPAD_GATE_VAL 100   //����ֵ
#define TPAD_ARR_MAX_VAL 0XFFFF  //���װ��ֵ

u8 TPAD_Init(u8 psc)
{
    u16 buf[10];     //���񷵻ص�ʮ������
	  u16 temp;
	  u8 i,j;
	  TIM5_CH2_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);  //��1MHZ��Ƶ�ʼ���
	  for(i=0;i<10;i++)
	 {
	     buf[i]=TPAD_Get_Val();    //���񵽵�ֵ����buf������  
	     delay_ms(10);   
	 }
	 for(i=0;i<9;i++)     //����ʮ��ֵ������������
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
	 temp = 0;    //�����Ѿ�ʹ������temp,�������¹���
	 for(i=2;i<8;i++)  temp+=buf[i];  //���м��6�����ݽ����ۼ�
	 tpad_default_val = temp/6;  //��ʱ�Ѿ��ó���6�����ݵ�ƽ��ֵ
	 printf("tpad_default_val:%d\r\n",tpad_default_val);   //��ӡ��������صĵ��ݳ��ƽ��ʱ��
	 if(tpad_default_val>TPAD_ARR_MAX_VAL/2)   return 1;   //!!Ϊʲô��ʼ����������1
	 return 0;
}
void TPAD_Reset(void)
{
	  GPIO_InitTypeDef   GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //�����˿�Aʱ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;   //PA1�������
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_1);   //���0�ͷŵ��ݵ���
	  delay_ms(5);
	  TIM_SetCounter(TIM5,0);    //����
	  TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC2);
	  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //�ŵ����Ϊ��������
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
u16 TPAD_Get_Val(void)
{
    TPAD_Reset();   //tpad�ȸ�λ
	  while((TIM_GetFlagStatus(TIM5,TIM_IT_CC2))==RESET)  //�ȴ�������򷵻ز���ֵ
		{
		    if(TIM_GetCounter(TIM5)>TPAD_ARR_MAX_VAL - 500)     //!!ΪʲôҪ��500��
				return TIM_GetCounter(TIM5);  //�����ʱ�ͷ��ؼ�������ֵ
		}
		return TIM_GetCapture2(TIM5);
}

u16 TPAD_Get_MaxVal(u8 n)    //n�����ݣ�ȡ�����ֵ
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

u8 TPAD_Scan(u8 mode)  //mode=0��֧������������mode=1��֧��
{
    static u8 keyen = 0;  //0���Կ�ʼ��⣬>0���ܼ��
	  u8 res =0;   
		u8 sample =3;    //Ĭ�ϲ�������Ϊ3��
	  u16 rval;    //���������ֵ
	  if(mode)   
		{   
		   sample = 6;   //֧��������ʱ�򣬲���Ϊ6��
			 keyen = 0;   //֧������
		}
		rval = TPAD_Get_MaxVal(sample); 
		if(rval>(tpad_default_val+TPAD_GATE_VAL))   //������ʱ����ڳ�ʼƽ��ֵ+����ֵ���򴥷�����
		{
		   if(keyen==0)  res = 1;   //keyen=0,��Ч
			 printf("r:%d\r\n",rval);
			 keyen = 3;   //����Ҫ��3�ΰ�������Ч���˴�����ı����˰�������������Ч��
		} 
		if(keyen)  keyen--;
		return res;
}


void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{  
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitStructure;
	  TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //�����˿�Aʱ�Ӻ�TIM5ʱ��
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;  //��Ϊ��������
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	  
	  TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //tds=tck_tim
	  TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
	  TIM_TimeBaseInitStructure.TIM_Period=arr;   //��װ��ֵ
	  TIM_TimeBaseInitStructure.TIM_Prescaler=psc;   //��Ƶϵ��
	  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	  
	  TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;    //ѡ�������IC2ӳ�䵽TI5��
	  TIM_ICInitStructure.TIM_ICFilter=0x03;    //8����ʱ��ʱ���˲�
	  TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPSC_DIV1;    //���������Ƶ������Ƶ
	  TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPolarity_Rising;    //�����ش�������
	  TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;   //
	  TIM_ICInit(TIM5,&TIM_ICInitStructure);
		
		TIM_Cmd(TIM5,ENABLE);    //����������			
}

