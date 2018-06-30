#ifndef _TPAD_H_
#define _TPAD_H_
#include "sys.h"

extern u16 tpad_default_val; 

void TPAD_Reset(void);
void TIM5_CH2_Cap_Init(u16 arr,u16 psc);
u16 TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8 TPAD_Init(u8 psc);
u8 TPAD_Scan(u8 mode);

#endif

