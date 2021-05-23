#ifndef __BSP_OTHERS_H__
#define __BSP_OTHERS_H__

#include "stdint.h"

void bsp_system_init(void);
uint8_t bsp_addr_get(void);
uint32_t bsp_rtcTime_get(void);
void  bsp_temp_check(void);
uint16_t bsp_temp_get(uint8_t id);
void bsp_addr_check(void);

#endif
