//=======================
//输出控制 bsp层
//向 业务层提供接口
//=======================
#include "bsp_output.h"
#include "stm32f10x.h"
#include "ctrl_output.h"
 #include "sys.h"
//=======================
//输出初始化
//=======================
void bsp_output_init(void)
{
	output_config();
}
//=======================
//开启通道
//说明：目前有6个通道
//=======================
void  bsp_output_open(uint8_t  id)
{
	
	if(id == O_COLD)//制冷
	{
		set_PB2();
	}
	else
	if(id == O_WIND)//风机
	{
		set_PB12();
	}
	else
	if(id == O_LIGHT)//照明
	{
		set_PB13();
	}
	else
	if(id == O_UV)//紫外
	{
		set_PB14();
	}
	else
	if(id == O_PREP1)//备用1
	{
		set_PB15();
	}
	else
	if(id == O_PREP2)//备用2
	{
		set_PC6();
	}

}
//=======================
//关闭通道
//说明：目前有6个通道
//=======================
void  bsp_output_close(uint8_t id)
{
	if(id == O_COLD)//制冷
	{
		reset_PB2();
	}
	else
	if(id == O_WIND)//风机
	{
		reset_PB12();
	}
	else
	if(id == O_LIGHT)//照明
	{
		reset_PB13();
	}
	else
	if(id == O_UV)//紫外
	{
		reset_PB14();
	}
	else
	if(id == O_PREP1)//备用1
	{
		reset_PB15();
	}
	else
	if(id == O_PREP2)//备用2
	{
		reset_PC6();
	}
}
//=======================
//关闭所有通道
//说明：目前有6个通道
//=======================
void  bsp_output_reset(void)
{
	//全部关闭
	reset_PB2();
	reset_PB12();
	reset_PB13();
	reset_PB14();
	reset_PB15();
	reset_PC6();
}

/*
控制回路检测
*/
uint8_t bsp_ex_check(uint8_t id)
{
	uint8_t rs = 0;
	switch(id)
	{
		case O_COLD ://制冷
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
		break;
		
		case O_WIND ://风机
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
		break;
			
		case O_LIGHT ://照明
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
		break;
		
		case O_UV:	//紫外
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
		break;
		
		case O_PREP1 : //备用1
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
		break;
		
		case O_PREP2 : //备用2
			rs = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
		break;
	}
	rs &= 0x1;
	return rs;
}

