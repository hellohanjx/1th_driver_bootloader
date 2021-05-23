/*
@说明：发现有的cpu引脚初始状态非高阻；所以会出现电机上电后开始转，发现85货道会上电后开始转
	所以在此添加对电机的GPIO初始化，认为设置一下；进制电机加电
*/
#include "stm32f10x_gpio.h"
#include "gpio_init.h"

void gpio_init(void)
{
	/*电机列信号初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);	

	//奇数列引脚 PA12 PA11 PA8 ,237的地址控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 |  GPIO_Pin_8;	//PA12->PO1,PA11->PO2，PA8->PA3 配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	//偶数列引脚 PC9 PC8 PC7, 237的地址控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9;	//PC7->PE3, PC8->PE2，PC9->PE1 配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线翻转速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);

	/*电机行信号初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//PB0->D6,PB1->D5 配置为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//PC4->DT2,PC5->D7 配置为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 输出全部拉高
	
	/*输出回路初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;					//PC6配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线翻转速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	//默认关闭通道
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2;		//PB2,PB12~PB15配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线翻转速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOB,GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2);

}

