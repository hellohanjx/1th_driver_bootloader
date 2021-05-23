//===================
//RTC ����
//˵����RTC �������� BACKUP ���Ĵ洢��������ֻҪ�����磬RTC �������ǲ�����
//RTC ������루S����Ϊ������λ����Լ�ɴ洢��ʱ���� 136�꣨����32λ���Ȼ��㣩
//ע�⣺������û��32k��������ֻ�����ڲ����ⲿ���پ����ⲿ����ʹ��8M��
//===================
#include "misc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include "rtc.h"

//===================
//rtc ��ʼ��
//===================
void rtc_config(void)
{
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );	//ʹ�ܵ�Դ�뱸����ʱ��
	PWR_BackupAccessCmd(ENABLE);                           //�������BKP��������RTC 
	BKP_DeInit();	                                       //��λ������
	RCC_HSEConfig(RCC_HSE_ON);	                           //�����ⲿ HSE
	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);	   //�ȴ�HSE����

	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);	       //ѡ�� HSE/128 ΪRTCʱ��Դ
	RCC_RTCCLKCmd(ENABLE);	                               //RTCʱ��ʹ��
	RTC_WaitForSynchro();	                               //�ȴ�RTC �� APBʱ��ͬ��(�����Ǹ�APB1����ʱ�Ӱ�)
	RTC_WaitForLastTask();	                               //�ȴ����һ�ζ�RTC�Ĵ�����д�������

//	RTC_ITConfig(RTC_IT_SEC, ENABLE);	                   //RTC ���ж�ʹ��
//	RTC_WaitForLastTask();	                               //�ȴ�����RTC�Ĵ�����д�������
	
	RTC_EnterConfigMode();			//��������RTC
	RTC_SetPrescaler(62499); 		//����RTCʱ�ӷ�Ƶֵ 62499�������Ƶ��= (8M/128)/(62499+1)=1Hz(1s)
	RTC_WaitForLastTask();	  		//�ȴ�����RTC�Ĵ�����д�������
	RTC_ExitConfigMode();			//�˳�����ģʽ    
}
//===================
//rtc �Ĵ��ȡ
//���ܣ���ȡ�˴ο���/����RTC������RTC�ļ���
//===================
uint32_t get_rtc_count(void)
{
	return RTC_GetCounter();
}

