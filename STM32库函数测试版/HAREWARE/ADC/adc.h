#ifndef _ADC_H_
#define _ADC_H_
#include "sys.h"

void Adc_Init(void);
u16 Get_Adc_Average(u8 ch,u8 times);
u16 Get_Adc(u8 ch) ;	

#endif

