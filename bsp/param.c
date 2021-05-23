/*
flash ��������
*/

#include "param.h"
#include "user_config.h"
#include "sys.h"
#include "timer4.h"
#include "timer3.h"


volatile uint16_t g_driverboardType			= DRIVERBOARD_TYPE;					//����������
volatile uint32_t g_temperatureType 		= TEMPERATURE_TYPE;					//�¶�̽ͷ����
volatile uint16_t g_motorStartCurrent		= MOTOR_START_CURRENT;				//����ʱ��Ϊ���ĵ������е����
volatile uint16_t g_motorShortCurrent 		= MOTOR_SHORT_CURRENT;				//��·����,����˲��ﵽ200mA,���ﲢû�еȴ��������˼�ʮ��ns�ͼ�⣬���Ͼ���ô��ĵ���˵����·��
volatile uint16_t g_motorOverloadCurrent 	= MOTOR_OVERLOAD_CURRENT;			//�������ص���(mA)��������ֹͣ
/*
ʱ�䵥λ����Ҫת��Ϊus��λ
*/
volatile uint32_t g_motorOverloadTimeout	= MOTOR_OVERLOAD_TIMEOUT*(TIME_MS_US/TIME4_BASE_INTERRUPT);	//��������ʱ��(s)
volatile uint32_t g_motorStartTimeout		= MOTOR_START_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);		//��ʱδ����ʱ��(s)
volatile uint32_t g_motorStopTimeout		= MOTOR_STOP_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);		//��ʱδֹͣʱ��(s)

volatile uint32_t g_motorStopDelay 			= MOTOR_STOP_DELAY;					//���ֹͣ��ʱ

volatile uint32_t g_signal_saleCount 		= MOTOR_SIGNAL_SALE_COUNT;				//����ʱ�źż�����
volatile uint32_t g_signal_saleInterval 	= MOTOR_SIGNAL_SALE_INTERVAL;			//����ʱ�źż����

volatile uint32_t g_signal_checkCount 		= MOTOR_SIGNAL_CHK_COUNT;				//��ѯʱ�źż�����
volatile uint32_t g_signal_checkInterval 	= MOTOR_SIGNAL_CHK_INTERVAL;			//��ѯʱ�źż����

/*
���ܣ���ȡ����
������У��
*/
static void read_param(void)
{
	//����������
	if( *(__IO uint16_t*)PARAM_ADDR_DRIVERBOARD_TYPE + *(__IO uint16_t*)(PARAM_ADDR_DRIVERBOARD_TYPE+2) == 0xffff)
		g_driverboardType = *(__IO uint16_t*)PARAM_ADDR_DRIVERBOARD_TYPE;
	else
		g_driverboardType = DRIVERBOARD_TYPE;
	
	//�¶�̽ͷ����
	if( *(__IO uint16_t*)PARAM_ADDR_TEMPERATURE_TYPE + *(__IO uint16_t*)(PARAM_ADDR_TEMPERATURE_TYPE+2) == 0xffff)
		g_temperatureType = (*(__IO uint16_t*)PARAM_ADDR_TEMPERATURE_TYPE);
	else
		g_temperatureType = TEMPERATURE_TYPE;
	
	//��������
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_START + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_START+2) == 0xffff)
		g_motorStartCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_START;
	else
		g_motorStartCurrent = MOTOR_START_CURRENT;
	
	
	//��·����
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_SHORT + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_SHORT+2) == 0xffff)
		g_motorShortCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_SHORT;
	else
		g_motorShortCurrent = MOTOR_SHORT_CURRENT;
	
	//���ص���
	if( *(__IO uint16_t*)PARAM_ADDR_CURRENT_OVERLOAD + *(__IO uint16_t*)(PARAM_ADDR_CURRENT_OVERLOAD+2) == 0xffff)
		g_motorOverloadCurrent = *(__IO uint16_t*)PARAM_ADDR_CURRENT_OVERLOAD;
	else
		g_motorOverloadCurrent = MOTOR_OVERLOAD_CURRENT;
	
	//���ص�������ʱ��
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT+2) == 0xffff)
		g_motorOverloadTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT)*(TIME_MS_US/TIME4_BASE_INTERRUPT);
	else
		g_motorOverloadTimeout = MOTOR_OVERLOAD_TIMEOUT*(TIME_MS_US/TIME4_BASE_INTERRUPT);
	
	//��ʱδ����
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_START_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_START_TIMEOUT+2) == 0xffff)
		g_motorStartTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_START_TIMEOUT)*(TIME_S_US/TIME4_BASE_INTERRUPT);
	else
		g_motorStartTimeout = MOTOR_START_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);
	
	//��ʱδֹͣ
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_STOP_TIMEOUT+2) == 0xffff)
		g_motorStopTimeout = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_TIMEOUT)*(TIME_S_US/TIME4_BASE_INTERRUPT);
	else
		g_motorStopTimeout = MOTOR_STOP_TIMEOUT*(TIME_S_US/TIME4_BASE_INTERRUPT);

	//���ֹͣ��ʱ
	if( *(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT + *(__IO uint16_t*)(PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT+2) == 0xffff)
		g_motorStopDelay = (*(__IO uint16_t*)PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT);
	else
		g_motorStopDelay = MOTOR_STOP_DELAY;

	//����ʱ���λ�ü�����
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_COUNT + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_SALE_COUNT+2) == 0xffff)
		g_signal_saleCount = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_COUNT);
	else
		g_signal_saleCount = MOTOR_SIGNAL_SALE_COUNT;

	//����ʱ���λ�ü����ʱ��
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_INTERVAL + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_SALE_INTERVAL+2) == 0xffff)
		g_signal_saleInterval = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_SALE_INTERVAL);
	else
		g_signal_saleInterval = MOTOR_SIGNAL_SALE_INTERVAL;

	//��ѯʱ���λ�ü�����
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_COUNT + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_CHECK_COUNT+2) == 0xffff)
		g_signal_checkCount = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_COUNT);
	else
		g_signal_checkCount = MOTOR_SIGNAL_CHK_COUNT;

	//��ѯʱ���λ�ü����ʱ��
	if( *(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_INTERVAL + *(__IO uint16_t*)(PARAM_ADDR_SIGNAL_CHECK_INTERVAL+2) == 0xffff)
		g_signal_checkInterval = (*(__IO uint16_t*)PARAM_ADDR_SIGNAL_CHECK_INTERVAL);
	else
		g_signal_checkInterval = MOTOR_SIGNAL_CHK_INTERVAL;
}

/*
�ָ�Ĭ������
ÿ����������������
*/
static uint8_t reset_default_param(void)
{
	FLASH_Status fStatus;
	uint8_t i;
	
	//����������
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
	
	//�¶�̽ͷ����
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
	
	//�����������
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
	
	//��·����
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
	
	//���ص���
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


	//��������ʱ��
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
	//��ʱδ����ʱ��
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
	//��ʱδֹͣʱ��
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
	
	//���ֹͣ��ʱ
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

	//����ʱ���λ�ü�����
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

	//����ʱ���λ�ü����
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

	
	//��ѯʱ���λ�ü�����
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

	//��ѯʱ���λ�ü����
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
��ʼ��flash
���ܣ��ָ�flash��־λ���ָ����������Ĭ��ֵ
ע�⣺flash ����ΪС��ģʽ,�͵�ַ�洢���ֽ�
*/
void init_flash(void)
{
	FLASH_Status fStatus;
	uint8_t i = 0;

	if((*(__IO uint16_t*)PARAM_ADDR_BASE ) != PARAM_FLAG)//��־λ����
	{
//		delay_ms(5000);
		FLASH_Unlock ();
		FLASH_ErasePage(PARAM_ADDR_BASE);
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_BASE, PARAM_FLAG);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
	
		reset_default_param();//�ָ�Ĭ�ϲ���
		read_param();
		FLASH_Lock ();
	}
	else//��ȡ��������
	{
		read_param();
	}
}

/*
���ܣ����ò���
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
		FLASH_Unlock ();//����flash���
		FLASH_ErasePage(PARAM_ADDR_BASE);//����Ҫд��
		do
		{
			fStatus = FLASH_ProgramHalfWord(PARAM_ADDR_BASE, PARAM_FLAG);
		}while(fStatus != FLASH_COMPLETE && i++ < 3);
		
	//����������
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
	
	//�¶�̽ͷ����
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
	
	//�����������
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
	
	//��·����
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
	
	//���ص���
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


	//��������ʱ��
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
	
	//��ʱδ����ʱ��
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
	
	//��ʱδֹͣʱ��
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
	
	//���ֹͣ��ʱ
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

	//����ʱ���λ�ü�����
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

	//����ʱ���λ�ü����
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

		
	//��ѯʱ���λ�ü�����
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

	//��ѯʱ���λ�ü����
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
		
		
		
		
		//�������óɹ��������ȫ�ֱ���
		g_driverboardType 		= param_get()[0];		//����������
		g_temperatureType 		= param_get()[1];		//�¶�̽ͷ����
		g_motorStartCurrent 	= param_get()[2];		//����ʱ��Ϊ���ĵ������е����(mA)
		g_motorShortCurrent 	= param_get()[3];		//��·����(mA)
		g_motorOverloadCurrent 	= param_get()[4];		//�������ص���(mA)��������ֹͣ
		g_motorOverloadTimeout 	= param_get()[5]*(TIME_MS_US/TIME4_BASE_INTERRUPT);	//��������ʱ��(ms)
		g_motorStartTimeout 	= param_get()[6]*(TIME_S_US/TIME4_BASE_INTERRUPT);	//��ʱδ����ʱ��(s)
		g_motorStopTimeout		= param_get()[7]*(TIME_S_US/TIME4_BASE_INTERRUPT);	//��ʱδֹͣʱ��(s)
		g_motorStopDelay		= param_get()[8];		//���ֹͣ��ʱ(ms)
		g_signal_saleCount 		= param_get()[9];		//�źż�����
		g_signal_saleInterval 	= param_get()[10];		//�źż����(us)
		g_signal_checkCount 	= param_get()[11];		//��ѯʱ�źż��
		g_signal_checkInterval 	= param_get()[12];		//��ѯʱ�źż����
		
		FLASH_Lock ();
		return TRUE;
	}
	return FALSE;	
}

/*
���ܣ��ظ�����Ĭ��ֵ
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

	rs = reset_default_param();//�ָ�Ĭ�ϲ���
	read_param();
	FLASH_Lock ();
	return rs;
}

