//========================================
//�����ʱ��3��4���õ�ʱ���ǹرյģ����������cpu�ٶ�
//rtc ʱ��û�����жϣ�Ҳ��Ϊ�˼ӿ�cpu�ٶ�

//========================================

#include "rtc.h"
#include "timer2.h"
#include "stdint.h"
#include "bsp_timer.h"

void bsp_timer_init(void)//systickʱ����led����������������Է���led��ʼ����
{
	rtc_config();			//RTC ʱ������
	timer2_config();
}

//========================================
//��ȡ��ǰrtcʱ����ģ���λ (s)
//========================================
uint32_t bsp_rtcTime_get(void)
{
	return get_rtc_count();
}

