#ifndef __BEEP_H_
#define __BEEP_H_
#include "sys.h"

void BEEP_Init(void);  //蜂鸣器初始化函数

#define BEEP PBout(8)  //BEEP表示PB8

#endif

