#ifndef __LED_H_
#define __LED_H_
#include "sys.h"

void LED_Init(void);   //LED初始化还函数
 
#define LED0 PBout(5)  //LED0表示PB5端口
#define LED1 PEout(5)  //LED1表示PE5端口

#endif

