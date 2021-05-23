//===================
//RTC 驱动
//说明：RTC 是利用了 BACKUP 区的存储器，所以只要不掉电，RTC 计数器是不会变的
//RTC 如果用秒（S）作为计数单位，大约可存储的时间是 136年（即是32位长度换算）
//注意：驱动板没有32k晶振，所以只能用内部或外部高速晶振（外部晶振使用8M）
//===================
#include "misc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include "rtc.h"

//===================
//rtc 初始化
//===================
void rtc_config(void)
{
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );	//使能电源与备份区时钟
	PWR_BackupAccessCmd(ENABLE);                           //允许访问BKP备份域与RTC 
	BKP_DeInit();	                                       //复位备份域
	RCC_HSEConfig(RCC_HSE_ON);	                           //开启外部 HSE
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);	   //等待HSE起振

	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);	       //选择 HSE/128 为RTC时钟源
	RCC_RTCCLKCmd(ENABLE);	                               //RTC时钟使能
	RTC_WaitForSynchro();	                               //等待RTC 与 APB时钟同步(就是那个APB1总线时钟吧)
	RTC_WaitForLastTask();	                               //等待最进一次对RTC寄存器的写操作完成

//	RTC_ITConfig(RTC_IT_SEC, ENABLE);	                   //RTC 秒中断使能
//	RTC_WaitForLastTask();	                               //等待最后对RTC寄存器的写操作完成
	
	RTC_EnterConfigMode();			//允许配置RTC
	RTC_SetPrescaler(62499); 		//设置RTC时钟分频值 62499，则计数频率= (8M/128)/(62499+1)=1Hz(1s)
	RTC_WaitForLastTask();	  		//等待最后对RTC寄存器的写操作完成
	RTC_ExitConfigMode();			//退出配置模式    
}
//===================
//rtc 寄存读取
//功能：读取此次开机/设置RTC以来，RTC的计数
//===================
uint32_t get_rtc_count(void)
{
	return RTC_GetCounter();
}

