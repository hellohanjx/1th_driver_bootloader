//====================================
//接口 485
//与主控板通信
//====================================
#ifndef __COM_TO_MASTER__
#define __COM_TO_MASTER__

#include "stdint.h"

#define MASTER_COMBAUD		115200		//与主机通讯波特率
//====================================
//包头包尾
//====================================
#define M_STX		0xaa	//主机端包头
#define S_STX		0xbb	//驱动板包头
//====================================





uint8_t get_message(void);
uint8_t* get_recv_buf(uint16_t*len);


typedef uint8_t (*MASTER_RECV_DEALWITH)(volatile  uint8_t*, volatile uint16_t);//定义一个回调函数类型

void usart1_config(void);
void uart1_dmaRx_config(void);
void uart1_dmaTx_config(void);

void recv_fun_init(MASTER_RECV_DEALWITH fun);
void usart1_send(uint8_t *dat, uint16_t size)  ;

#endif
