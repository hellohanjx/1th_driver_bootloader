//========================================
//这里的时钟3，4不用的时候是关闭的，有利于提高cpu速度
//rtc 时钟没有用中断，也是为了加快cpu速度

//========================================

#include "rtc.h"
#include "timer2.h"
#include "stdint.h"
#include "bsp_timer.h"

void bsp_timer_init(void)//systick时钟与led呼吸灯相关联，所以放在led初始化中
{
	rtc_config();			//RTC 时钟配置
	timer2_config();
}

//========================================
//获取当前rtc时间节拍，单位 (s)
//========================================
uint32_t bsp_rtcTime_get(void)
{
	return get_rtc_count();
}

