//==============
//led 灯bsp层，向业务层提供接口
//led 呼吸灯
//led 数据接收灯
//==============
#include "bsp_led.h"
#include "led.h"
#include "systick.h"


 //=========================
//led 呼吸灯闪烁回调函数
//=========================
uint8_t  bsp_ledBreath_flickerCallBack(uint8_t state)
{
	if(state)
	{
		LED_BREATH_ON;
	}
	else
	{
		LED_BREATH_OFF; 
	}
	return TRUE;
}

//=========================
//led 初始化
//初始化 led 引脚
//初始化 systick 时钟，使用 systick实现 呼吸灯闪烁
//初始化 通用定时器4，用来处 led 接收灯
//=========================
void bsp_led_init(void)
{
	systick_config();//systick 初始化
	led_config();	//led 引脚初始化
	sysTick_callBack_init(bsp_ledBreath_flickerCallBack);//systick 回调函数初始化
}

