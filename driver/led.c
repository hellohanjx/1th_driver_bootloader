//=========================
//led ������
//˵������bsp���ṩ�ӿ�
//=========================

#include "stm32f10x.h"
#include "led.h"

//=========================
//led ��ʼ��
//˵����ʹ���� PA0,PA1
//=========================
void led_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		//PA0,PA1����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//GPIO �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);				//Ĭ������
}

