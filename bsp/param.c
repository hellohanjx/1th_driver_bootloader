/*
flash 参数操作
*/

#include "param.h"
#include "user_config.h"
#include "sys.h"
#include "timer4.h"
#include "timer3.h"


volatile uint16_t g_driverboardType			= DRIVERBOARD_TYPE;					//驱动板类型
volatile uint32_t g_temperatureType 		= TEMPERATURE_TYPE;					//温度探头类型
volatile uint16_t g_motorStartCurrent		= MOTOR_START_CURRENT;				//启动时认为多大的电流是有电机的
volatile uint16_t g_motorShortCurrent 		= MOTOR_SHORT_CURRENT;				//短路电流,启动瞬间达到200mA,这里并没有等待，最多等了几十个ns就检测，马上就这么大的电流说明短路了
volatile uint16_t g_motorOverloadCurrent 	= MOTOR_OVERLOAD_CURRENT;			//电机最大负载电流(mA)，超过可停止
/*
时间单位都需要转换为us单位
*/
volatile uint32_t g_motorOverloadTimeout	= MOTOR_OVERLOAD_TIMEOUT*(TIME_MS_US/TIME4_BASE_INTERRUPT);	//过流允许时间(s)
volatile uint32_t g_motorStartTimeout		= MOTOR_START_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);		//超时未启动时间(s)
volatile uint32_t g_motorStopTimeout		= MOTOR_STOP_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);		//超时未停止时间(s)

volatile uint32_t g_motorStopDelay 			= MOTOR_STOP_DELAY;					//电机停止延时

volatile uint32_t g_signal_saleCount 		= MOTOR_SIGNAL_SALE_COUNT;				//出货时信号检测次数
volatile uint32_t g_signal_saleInterval 	= MOTOR_SIGNAL_SALE_INTERVAL;			//出货时信号检测间隔

volatile uint32_t g_signal_checkCount 		= MOTOR_SIGNAL_CHK_COUNT;				//查询时信号检测次数
volatile uint32_t g_signal_checkInterval 	= MOTOR_SIGNAL_CHK_INTERVAL;			//查询时信号检测间隔

/*
功能：读取参数
计算了校验
*/
static void read_param(void)
{
	//驱动板类型
	if( *(__IO uint16_t*)PARAM_ADDR_DRIVERBOARD_TYPE + *(__IO uint16_t*)(PARAM_ADDR_DRIVERBOARD_TYPE+2) == 0xffff)
		g_driverboardType = *(__IO uint16_t*)PARAM_ADDR_DRIVERBOARD_TYPE;
	else
		g_driverboardType = DRIVERBOARD_TYPE;
	
	//温度探头类型
	if( *(__IO uint16_t*)PARAM_ADDR_TEMPERATURE_TYPE + *(__IO uint16_t*)(PARAM_ADDR_TEMPERATURE_TYPE+2) == 0xffff)
		g_temperatureType = (*(__IO uint16_t*)PARAM_ADDR_TEMPERATURE_TYPE);
	else
		g_temperatureType = TEMPERATURE_TYPE;
	
	//启动电流
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_START + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_START+2) == 0xffff)
		g_motorStartCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_START;
	else
		g_motorStartCurrent = MOTOR_START_CURRENT;
	
	
	//短路电流
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_SHORT + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_SHORT+2) == 0xffff)
		g_motorShortCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_SHORT;
	else
		g_motorShortCurrent = MOTOR_SHORT_CURRENT;
	
	//过载电流
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_OVERLOAD + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_OVERLOAD+2) == 0xffff)
		g_motorOverloadCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_OVERLOAD;
	else
		g_motorOverloadCurrent = MOTOR_OVERLOAD_CURRENT;
	
	//过载电流允许时间
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT+2) == 0xffff)
		g_motorOverloadTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT)*(TIME_MS_US/TIME4_BASE_INTERRUPT);
	else
		g_motorOverloadTimeout = MOTOR_OVERLOAD_TIMEOUT*(TIME_MS_US/TIME4_BASE_INTERRUPT);
	
	//超时未启动
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_START_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_START_TIMEOUT+2) == 0xffff)
		g_motorStartTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_START_TIMEOUT)*(TIME_S_US/TIME4_BASE_INTERRUPT);
	else
		g_motorStartTimeout = MOTOR_START_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);
	
	//超时未停止
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_STOP_TIMEOUT+2) == 0xffff)
		g_motorStopTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_TIMEOUT)*(TIME_S_US/TIME4_BASE_INTERRUPT);
	else
		g_motorStopTimeout = MOTOR_STOP_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);

	//电机停止延时
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT+2) == 0xffff)
		g_motorStopDelay = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT);
	else
		g_motorStopDelay = MOTOR_STOP_DELAY;

	//出货时电机位置检测次数
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_COUNT + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_SALE_COUNT+2) == 0xffff)
		g_signal_saleCount = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_COUNT);
	else
		g_signal_saleCount = MOTOR_SIGNAL_SALE_COUNT;

	//出货时电机位置检测间隔时间
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_INTERVAL + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_SALE_INTERVAL+2) == 0xffff)
		g_signal_saleInterval = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_INTERVAL);
	else
		g_signal_saleInterval = MOTOR_SIGNAL_SALE_INTERVAL;

	//查询时电机位置检测次数
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_COUNT + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_CHECK_COUNT+2) == 0xffff)
		g_signal_checkCount = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_COUNT);
	else
		g_signal_checkCount = MOTOR_SIGNAL_CHK_COUNT;

	//查询时电机位置检测间隔时间
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_INTERVAL + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_CHECK_INTERVAL+2) == 0xffff)
		g_signal_checkInterval = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_INTERVAL);
	else
		g_signal_checkInterval = MOTOR_SIGNAL_CHK_INTERVAL;
}

/*
恢复默认数据
每个参数正反存两个
*/
static uint8_t reset_default_param(void)
{
	FLASH_Status fStatus;
	uint8_t i;
	
	//驱动板类型
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_DRIVERBOARD_TYPE, DRIVERBOARD_TYPE);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_DRIVERBOARD_TYPE + 2, (~DRIVERBOARD_TYPE));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	
	//温度探头类型
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_TEMPERATURE_TYPE, TEMPERATURE_TYPE);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_TEMPERATURE_TYPE + 2, (~TEMPERATURE_TYPE));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	
	//电机启动电流
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_START, MOTOR_START_CURRENT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_START + 2, (~MOTOR_START_CURRENT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	
	//短路电流
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_SHORT, MOTOR_SHORT_CURRENT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_SHORT + 2, (~MOTOR_SHORT_CURRENT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	
	//过载电流
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_OVERLOAD, MOTOR_OVERLOAD_CURRENT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_OVERLOAD + 2, (~MOTOR_OVERLOAD_CURRENT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;


	//过流允许时间
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT, MOTOR_OVERLOAD_TIMEOUT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT + 2, (~MOTOR_OVERLOAD_TIMEOUT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	//超时未启动时间
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_START_TIMEOUT, MOTOR_START_TIMEOUT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_START_TIMEOUT + 2, (~MOTOR_START_TIMEOUT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	//超时未停止时间
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_TIMEOUT, MOTOR_STOP_TIMEOUT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_TIMEOUT + 2, (~MOTOR_STOP_TIMEOUT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	
	//电机停止延时
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT, MOTOR_STOP_DELAY);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT + 2, (~MOTOR_STOP_DELAY));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;

	//出货时电机位置检测次数
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_COUNT, MOTOR_SIGNAL_SALE_COUNT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_COUNT + 2, (~MOTOR_SIGNAL_SALE_COUNT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;

	//出货时电机位置检测间隔
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_INTERVAL, MOTOR_SIGNAL_SALE_INTERVAL);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_INTERVAL + 2, (~MOTOR_SIGNAL_SALE_INTERVAL));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;

	
	//查询时电机位置检测次数
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_COUNT, MOTOR_SIGNAL_CHK_COUNT);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_COUNT + 2, (~MOTOR_SIGNAL_CHK_COUNT));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;

	//查询时电机位置检测间隔
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_INTERVAL, MOTOR_SIGNAL_CHK_INTERVAL);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;
	i = 0;
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_INTERVAL + 2, (~MOTOR_SIGNAL_CHK_INTERVAL));
	}while(fStatus != FLASH_COMPLETE && i++ < 3);
	if(fStatus != FLASH_COMPLETE)
		return FALSE;

	
	return TRUE;
}


/*
初始化flash
功能：恢复flash标志位，恢复驱动板参数默认值
注意：flash 保存为小端模式,低地址存储低字节
*/
void init_flash(void)
{
	FLASH_Status fStatus;
	uint8_t i = 0;

	if((*(__IO uint16_t*)PARAM_ADDR_BASE ) != PARAM_FLAG)//标志位不符
	{
//		delay_ms(5000);
		FLASH_Unlock ();
		FLASH_ErasePage(PARAM_ADDR_BASE);
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_BASE, PARAM_FLAG);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
	
		reset_default_param();//恢复默认参数
		read_param();
		FLASH_Lock ();
	}
	else//读取设置数据
	{
		read_param();
	}
}

/*
功能：设置参数
*/
uint8_t set_flash(void)
{
	FLASH_Status fStatus;
	uint8_t i;
	
	if(g_driverboardType != param_get()[0] 
	|| g_temperatureType != param_get()[1]
	|| g_motorStartCurrent != param_get()[2]
	|| g_motorShortCurrent != param_get()[3]
	|| g_motorOverloadCurrent != param_get()[4]
	|| g_motorOverloadTimeout != param_get()[5]*(TIME_MS_US/TIME4_BASE_INTERRUPT)
	|| g_motorStartTimeout != param_get()[6]*(TIME_S_US/TIME4_BASE_INTERRUPT)
	|| g_motorStopTimeout != param_get()[7]*(TIME_S_US/TIME4_BASE_INTERRUPT)
	|| g_motorStopDelay		!= param_get()[8]
	|| g_signal_saleCount 	!= param_get()[9]
	|| g_signal_saleInterval 	!= param_get()[10]
	|| g_signal_checkCount != param_get()[11]
	|| g_signal_checkInterval != param_get()[12]
	)
	{
		FLASH_Unlock ();//解锁flash编程
		FLASH_ErasePage(PARAM_ADDR_BASE);//擦除要写区
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_BASE, PARAM_FLAG);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		
	//驱动板类型
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_DRIVERBOARD_TYPE, (param_get()[0]));
		}while(fStatus != FLASH_COMPLETE && i++ < 10);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_DRIVERBOARD_TYPE + 2, (~param_get()[0]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//温度探头类型
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_TEMPERATURE_TYPE, param_get()[1]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_TEMPERATURE_TYPE + 2, (~param_get()[1]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//电机启动电流
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_START, param_get()[2]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_START + 2, (~param_get()[2]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//短路电流
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_SHORT, param_get()[3]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_SHORT + 2, (~param_get()[3]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//过载电流
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_OVERLOAD, param_get()[4]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_CURRENT_OVERLOAD + 2, (~param_get()[4]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;


	//过流允许时间
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT, param_get()[5]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT + 2, (~param_get()[5]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//超时未启动时间
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_START_TIMEOUT, param_get()[6]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_START_TIMEOUT + 2, (~param_get()[6]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//超时未停止时间
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_TIMEOUT, param_get()[7]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_TIMEOUT + 2, (~param_get()[7]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
	
	//电机停止延时
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT, param_get()[8]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT + 2, (~param_get()[8]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;

	//出货时电机位置检测次数
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_COUNT, param_get()[9]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_COUNT + 2, (~param_get()[9]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;

	//出货时电机位置检测间隔
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_INTERVAL, param_get()[10]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_SALE_INTERVAL + 2, (~param_get()[10]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;

		
	//查询时电机位置检测次数
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_COUNT, param_get()[11]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_COUNT + 2, (~param_get()[11]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;

	//查询时电机位置检测间隔
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_INTERVAL, param_get()[12]);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		i = 0;
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_SIGNAL_CHECK_INTERVAL + 2, (~param_get()[12]));
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		if(fStatus != FLASH_COMPLETE)
			return FALSE;
		
		
		
		
		//到此设置成功，则更新全局变量
		g_driverboardType 		= param_get()[0];		//驱动板类型
		g_temperatureType 		= param_get()[1];		//温度探头类型
		g_motorStartCurrent 	= param_get()[2];		//启动时认为多大的电流是有电机的(mA)
		g_motorShortCurrent 	= param_get()[3];		//短路电流(mA)
		g_motorOverloadCurrent 	= param_get()[4];		//电机最大负载电流(mA)，超过可停止
		g_motorOverloadTimeout 	= param_get()[5]*(TIME_MS_US/TIME4_BASE_INTERRUPT);	//过流允许时间(ms)
		g_motorStartTimeout 	= param_get()[6]*(TIME_S_US/TIME4_BASE_INTERRUPT);	//超时未启动时间(s)
		g_motorStopTimeout		= param_get()[7]*(TIME_S_US/TIME4_BASE_INTERRUPT);	//超时未停止时间(s)
		g_motorStopDelay		= param_get()[8];		//电机停止延时(ms)
		g_signal_saleCount 		= param_get()[9];		//信号检测次数
		g_signal_saleInterval 	= param_get()[10];		//信号检测间隔(us)
		g_signal_checkCount 	= param_get()[11];		//查询时信号检测
		g_signal_checkInterval 	= param_get()[12];		//查询时信号检测间隔
		
		FLASH_Lock ();
		return TRUE;
	}
	return FALSE;	
}

/*
功能：回复参数默认值
*/
uint8_t reset_flash(void)
{
	uint8_t rs = FALSE, i = 0;
	FLASH_Status fStatus;
	
	FLASH_Unlock ();
	FLASH_ErasePage(PARAM_ADDR_BASE);
	do
	{
		fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_BASE, PARAM_FLAG);
	}while(fStatus != FLASH_COMPLETE && i++ < 3);

	rs = reset_default_param();//恢复默认参数
	read_param();
	FLASH_Lock ();
	return rs;
}

