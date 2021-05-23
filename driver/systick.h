//========================
//系统滴答计时器头文件
//========================

#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"

#define IRQ_INTERVAL	9000000 //中断间隔 1s

typedef uint8_t (*SYS_TICK_CALLBACK)(uint8_t state);//定义一个回调函数类型


void sysTick_callBack_init(SYS_TICK_CALLBACK fun);
void systick_config(void);
ITStatus get_sysTick_state(void);

#endif
