/*
@˵���������е�cpu���ų�ʼ״̬�Ǹ��裻���Ի���ֵ���ϵ��ʼת������85�������ϵ��ʼת
	�����ڴ���ӶԵ����GPIO��ʼ������Ϊ����һ�£����Ƶ���ӵ�
*/
#include "stm32f10x_gpio.h"
#include "gpio_init.h"

void gpio_init(void)
{
	/*������źų�ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);	

	//���������� PA12 PA11 PA8 ,237�ĵ�ַ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 |  GPIO_Pin_8;	//PA12->PO1,PA11->PO2��PA8->PA3 ����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	//ż�������� PC9 PC8 PC7, 237�ĵ�ַ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9;	//PC7->PE3, PC8->PE2��PC9->PE1 ����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

	/*������źų�ʼ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//PB0->D6,PB1->D5 ����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//PC4->DT2,PC5->D7 ����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 ���ȫ������
	
	/*�����·��ʼ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;					//PC6����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	//Ĭ�Ϲر�ͨ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2;		//PB2,PB12~PB15����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOB,GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2);

}

