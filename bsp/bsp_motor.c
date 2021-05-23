//=======================
//电机bsp层
//向业务层提供接口
//=======================
#include "stdint.h"
#include "moto.h"
#include "bsp_motor.h"
#include "adc.h"
#include "sys.h"
#include "user_config.h"

static volatile FlagStatus adc67Geted;//获取数据标志 
 volatile uint16_t current;
//================================
//电流检测回调函数
//================================
void  bsp_current_checkCallBack(uint16_t value)
{
	current = value;
	adc67Geted = SET;
}
//=======================
//电机系统初始化
//功能：
//初始化电机行
//初始化电机列
//初始化电机卡位
//初始化电机 adc
//初始化电流检测回调函数
//=======================
void bsp_motor_init(void)
{
	line_config();
	row_config();
	motor_position_config();
	adc1_config();
	get_adc67_init(bsp_current_checkCallBack); 
}
//=======================
//电机 IO 电平查询
//参数：电机编号
//返回值：FALSE :未在卡位；TRUE：在卡位
//=======================
uint8_t  bsp_motor_check(uint8_t type, uint8_t id)
{
	if(g_driverboardType == 1)//金码的信号与普通机型相反
	{
		return (check_position(type, id));
	}
	else//普通机型
	{
		return (!check_position(type, id));
	}
}

//=======================
//电机启动
//参数：电机编号
//=======================
void bsp_motor_start(uint8_t id)
{
	row_start(id/10, id%2);//开启通道时先开通地线 
	line_start(id%10);
}
//=======================
//电机停止
//参数：电机编号
//=======================
void bsp_motor_stop(void)
{
	line_stop();//关闭通道时后关闭地线
	row_stop(); 
}
//================================
//电机电流采集
//================================
uint16_t  bsp_current_get(void)
{
	uint8_t i;
	uint16_t data[SAMPLING_ANT],rs;
	for(i = 0;i < SAMPLING_ANT;i++)
	{
		adc1_67_start();//开始采样 
		while(adc67Geted == RESET);
		adc67Geted = RESET;
		data[i] = current;
	}
	current = 0;
//	for(i = 0,rs = 0;i < SAMPLING_ANT;i++)//这里计算去掉第一个与最后一个，去中间三个值平均数
//	rs += data[i];
	for(i = 0,rs = 0;i < (SAMPLING_ANT);i++)//这里计算去掉第一个与最后一个，去中间三个值平均数
	rs += data[i];
	rs /= (SAMPLING_ANT);
	if(rs > 100)
		rs = rs;
	return 	rs ;
}

 
