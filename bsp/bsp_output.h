#ifndef __BSP_OUTPUT_H__
#define __BSP_OUTPUT_H__

#include "stm32f10x.h"

typedef enum{O_COLD = 0, O_WIND, O_LIGHT, O_UV, O_PREP1, O_PREP2} OUTPUT_ID;

void  bsp_output_init(void);
void  bsp_output_open(uint8_t id);
void  bsp_output_close(uint8_t id);
void  bsp_output_reset(void);
uint8_t bsp_ex_check(uint8_t id);


#endif

