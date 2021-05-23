#ifndef __BSP_MASTER_H__
#define __BSP_MASTER_H__

#include "stdint.h"

void bsp_master_init(void);
void bsp_master_sendString(uint8_t *data,uint16_t len);
void bsp_master_sendByte(uint8_t data); 


#endif

