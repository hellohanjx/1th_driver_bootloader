//======================
//������ͨѶ bsp
//��ҵ���ṩ�ӿ�
//======================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "master_protocol.h"
#include "led.h"
#include "timer2.h"

//======================
//������ͨ�ų�ʼ��
//======================
void bsp_master_init(void)
{
	usart1_config();	//uart1 ����
	uart1_dmaRx_config();	//uart1 DMA����
	uart1_dmaTx_config();	//uart1 DMA����
}

