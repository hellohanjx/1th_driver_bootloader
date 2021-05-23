#ifndef __TIMER2_H__
#define __TIMER2_H__

#include "stdint.h"

/*
�ж�ʱ�乫ʽ��T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
ע�⣺TIM_Period��Ԥװ��ֵ��TIM_Prescaler����Ƶֵ��TIM2����ʱ��ʱ��(72M)
*/

#define TIMER2_AUTO_LOAD 		1
#define TIMER2_PRE_SCALER		35999	//�ж�ʱ����1ms

typedef void  (*TIMER2_CALLBACK)( );//�ص���������



void timer2_config(void) ;
void timer2_start(uint16_t count);
void timer2_callbackFun_init(TIMER2_CALLBACK fun);

#endif

