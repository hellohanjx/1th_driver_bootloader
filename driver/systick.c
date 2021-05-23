//========================
//系统滴答计时器 驱动
//========================
#include "misc.h"
#include "systick.h"

static ITStatus systicksFlag;//systick 标记
SYS_TICK_CALLBACK sys_ticks_callback; 
 static volatile uint8_t tip = 0;
uint8_t sysfun(SYS_TICK_CALLBACK callBack,ITStatus rs )
{
	if(callBack == 0)
		return 2;
	if(callBack(rs))
		return TRUE;
	return  FALSE;
	
}
//========================
//滴答计时器初始化
//========================
void systick_config(void)
{
	SysTick_Config(IRQ_INTERVAL);								//1s 中断一次
	//其他过去的函数库中的被整合到下面这个函数中
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);     	//选择 HCLK为时钟源，8分频，9MHz
	
	//SysTick_ITConfig()//手上这个库没有
	
//	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;      //开启 systick 中断
//	SysTick->LOAD =  9000;    						//重载值
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;       //开启systick
}
//========================
//滴答计时器中断，设置为 1s 1次
//========================
void SysTick_Handler(void) 
{
	sysfun(sys_ticks_callback,systicksFlag);//回掉函数
	tip++;
	if(tip%3 == 0)
	systicksFlag = !systicksFlag; 
}
//========================
//滴答计时器回掉函数初始化
//========================
void sysTick_callBack_init(SYS_TICK_CALLBACK fun)
{
	sys_ticks_callback = fun;
}
