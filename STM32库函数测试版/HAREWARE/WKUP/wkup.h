#ifndef _WKUP_H_
#define _WKUP_H_
#include "sys.h"

#define WKUP_KD PAin(0)


void Sys_Enter_Standby(void);  //系统进入待机模式
u8 Check_WKUP(void);       //检测WKUP引脚的信号
void WKUP_Init(void);   //PA0 WKUP唤醒初始化
#endif

