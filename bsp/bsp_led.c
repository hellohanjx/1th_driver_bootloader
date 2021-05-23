//==============
//led ��bsp�㣬��ҵ����ṩ�ӿ�
//led ������
//led ���ݽ��յ�
//==============
#include "bsp_led.h"
#include "led.h"
#include "systick.h"


 //=========================
//led ��������˸�ص�����
//=========================
uint8_t  bsp_ledBreath_flickerCallBack(uint8_t state)
{
	if(state)
	{
		LED_BREATH_ON;
	}
	else
	{
		LED_BREATH_OFF; 
	}
	return TRUE;
}

//=========================
//led ��ʼ��
//��ʼ�� led ����
//��ʼ�� systick ʱ�ӣ�ʹ�� systickʵ�� ��������˸
//��ʼ�� ͨ�ö�ʱ��4�������� led ���յ�
//=========================
void bsp_led_init(void)
{
	systick_config();//systick ��ʼ��
	led_config();	//led ���ų�ʼ��
	sysTick_callBack_init(bsp_ledBreath_flickerCallBack);//systick �ص�������ʼ��
}

