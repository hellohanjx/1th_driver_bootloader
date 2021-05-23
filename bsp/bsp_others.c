//=====================
//一些没有接口函数底层驱动封装
//主要是运行在后台
//=====================
#include "bsp_others.h"
#include "misc.h"
#include "rcc.h"
#include "address.h"
#include "delay.h"


//========================================
//中断组配置
//说明：使用中断组1
//1位先占优先级，3位次优先级
//========================================
static void NVIC_group_config(void)
{
	//中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//2个先占优先级0，1；8个
}

//=======================================
//系统时钟初始化，库文件本身自带
//用户时钟初始化，用户可以自己修改
//中断分组初始化，选择系统所用的中断分组
//rtc时钟初始化
//地址配置初始化
//=======================================
void bsp_system_init(void)
{
	SystemInit();			//库文件定义
	user_rcc();				//用户自定义时钟
	NVIC_group_config();	//中断优先级分组
}

