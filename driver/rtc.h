//===================
//RTC 实时时钟头文件
//===================
#ifndef __RTC_H__
#define __RTC_H__

#include "stdint.h"

void rtc_config(void);
uint32_t get_rtc_count(void);

#endif
