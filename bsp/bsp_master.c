//======================
//与主机通讯 bsp
//向业务提供接口
//======================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "master_protocol.h"
#include "led.h"
#include "timer2.h"

//======================
//与主机通信初始化
//======================
void bsp_master_init(void)
{
	usart1_config();	//uart1 配置
	uart1_dmaRx_config();	//uart1 DMA配置
	uart1_dmaTx_config();	//uart1 DMA配置
}

