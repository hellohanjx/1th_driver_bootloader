//========================
//ϵͳ�δ��ʱ��ͷ�ļ�
//========================

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"

#define IRQ_INTERVAL	9000000 //�жϼ�� 1s

typedef uint8_t (*SYS_TICK_CALLBACK)(uint8_t state);//����һ���ص���������


void sysTick_callBack_init(SYS_TICK_CALLBACK fun);
void systick_config(void);
ITStatus get_sysTick_state(void);

#endif
