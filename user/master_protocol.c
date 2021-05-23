//=========================
//用主机通讯使用的协议
//=========================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "master_protocol.h" 
#include "stm32f10x.h"
#include "address.h" 

/*
功能：回应主机
参数：rs：结果->1.执行成功；2.不支持的指令；3.写入flash失败；4.数据缓冲区非空
5.校验错；
sqnr:流水号；m_addr:主机地址；cmd：回应对应的命令
*/
void send_answer_master(uint8_t rs, uint8_t sqnr, uint8_t m_addr, uint8_t cmd)
{
	uint8_t str[50],i,j,chk;
	
	i = 0;
	str[i++] = 0xbb;			//包头
	str[i++] = sqnr;//流水号
	str[i++] = ' ';				//包长高字节
	str[i++] = ' ';				//包长低字节
	str[i++] = m_addr;		//主机地址
	str[i++] = get_board_addr();//驱动板地址
	str[i++] = cmd;
	str[i++] = rs;
	str[2] = (i + 1) <<8;
	str[3] = (i + 1) & 0xff;
	
	for(j = 0, chk = 0; j < i; j++)//计算校验和
	{
		chk += str[j];
	}
	str[i++] = chk;
	delay_ms(50);
	usart1_send(str,i);
}
