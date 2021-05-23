//========================
//ϵͳ�δ��ʱ�� ����
//========================
#include "misc.h"
#include "systick.h"

static ITStatus systicksFlag;//systick ���
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
//�δ��ʱ����ʼ��
//========================
void systick_config(void)
{
	SysTick_Config(IRQ_INTERVAL);								//1s �ж�һ��
	//������ȥ�ĺ������еı����ϵ��������������
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);     	//ѡ�� HCLKΪʱ��Դ��8��Ƶ��9MHz
	
	//SysTick_ITConfig()//���������û��
	
//	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;      //���� systick �ж�
//	SysTick->LOAD =  9000;    						//����ֵ
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;       //����systick
}
//========================
//�δ��ʱ���жϣ�����Ϊ 1s 1��
//========================
void SysTick_Handler(void) 
{
	sysfun(sys_ticks_callback,systicksFlag);//�ص�����
	tip++;
	if(tip%3 == 0)
	systicksFlag = !systicksFlag; 
}
//========================
//�δ��ʱ���ص�������ʼ��
//========================
void sysTick_callBack_init(SYS_TICK_CALLBACK fun)
{
	sys_ticks_callback = fun;
}
