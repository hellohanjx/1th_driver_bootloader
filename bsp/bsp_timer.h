#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stdint.h"

void bsp_timer_init(void);

uint32_t bsp_rtcTime_get(void);

void bsp_midTimer_start(uint16_t value);
void bsp_midTimer_stop(void);
uint16_t bsp_midTimer_ticks(void);
	
void bsp_roughTimer_start(uint16_t value);
void bsp_roughTimer_stop(void);
uint32_t bsp_roughTimer_ticks(void);

#endif

