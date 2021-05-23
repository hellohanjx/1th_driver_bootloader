#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stdint.h"

void bsp_motor_init(void);
uint8_t  bsp_motor_check(uint8_t type, uint8_t id);
void bsp_motor_start(uint8_t id);
void bsp_motor_stop(void);
uint16_t  bsp_current_get(void);
#endif
