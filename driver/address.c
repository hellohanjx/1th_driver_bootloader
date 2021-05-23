//===============================
//驱动板地址获取
//===============================
#include "stdint.h"
#include "stm32f10x.h"

//===============================
//引脚初始化
//addr1 -> PC3
//addr2 -> PC2
//addr3 -> PC1
//addr4 -> PC0
//===============================
void addr_config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	//使能时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//口线翻转速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
//===============================
//获取地址
//参数：addr bsp层用于获取数据
//===============================
uint8_t get_board_addr(void)
{
	uint8_t addr[4];
	addr[0] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
	addr[1] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2);
	addr[2] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
	addr[3] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
	
	return (addr[0] & 0x1) | ((addr[1] & 0x1) << 1) | ((addr[2] & 0x1) <<2 ) | ((addr[3] & 0x1) << 3);
}
