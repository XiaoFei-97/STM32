#ifndef _WKUP_H_
#define _WKUP_H_
#include "sys.h"

#define WKUP_KD PAin(0)


void Sys_Enter_Standby(void);  //ϵͳ�������ģʽ
u8 Check_WKUP(void);       //���WKUP���ŵ��ź�
void WKUP_Init(void);   //PA0 WKUP���ѳ�ʼ��
#endif

