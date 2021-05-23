//=========================
//led 驱动层
//说明：向bsp层提供接口
//=========================

#include "stm32f10x.h"
#include "led.h"

//=========================
//led 初始化
//说明：使用了 PA0,PA1
//=========================
void led_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		//PA0,PA1配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//GPIO 输出速率
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);				//默认拉低
}

