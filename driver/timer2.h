#ifndef __TIMER2_H__
#define __TIMER2_H__

#include "stdint.h"

/*
中断时间公式：T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
注意：TIM_Period：预装载值；TIM_Prescaler：分频值；TIM2：定时器时钟(72M)
*/

#define TIMER2_AUTO_LOAD 		1
#define TIMER2_PRE_SCALER		35999	//中断时间间隔1ms

typedef void  (*TIMER2_CALLBACK)( );//回调函数类型



void timer2_config(void) ;
void timer2_start(uint16_t count);
void timer2_callbackFun_init(TIMER2_CALLBACK fun);

#endif

