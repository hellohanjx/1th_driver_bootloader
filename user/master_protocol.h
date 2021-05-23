#ifndef __MASTER_PROTOCOL__
#define __MASTER_PROTOCOL__

#include "stdint.h"



void send_answer_master(uint8_t rs, uint8_t sqnr, uint8_t m_addr, uint8_t cmd);


#define RESTART() ((void (*)())(0x0)) ()	//÷ÿ∆Ùcpu

#endif
