//===============================
//�������ַ��ȡ
//===============================
#include "stdint.h"
#include "stm32f10x.h"

//===============================
//���ų�ʼ��
//addr1 -> PC3
//addr2 -> PC2
//addr3 -> PC1
//addr4 -> PC0
//===============================
void addr_config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	//ʹ��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
//===============================
//��ȡ��ַ
//������addr bsp�����ڻ�ȡ����
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
