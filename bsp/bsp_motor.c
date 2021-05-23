//=======================
//���bsp��
//��ҵ����ṩ�ӿ�
//=======================
#include "stdint.h"
#include "moto.h"
#include "bsp_motor.h"
#include "adc.h"
#include "sys.h"
#include "user_config.h"

static volatile FlagStatus adc67Geted;//��ȡ���ݱ�־ 
 volatile uint16_t current;
//================================
//�������ص�����
//================================
void  bsp_current_checkCallBack(uint16_t value)
{
	current = value;
	adc67Geted = SET;
}
//=======================
//���ϵͳ��ʼ��
//���ܣ�
//��ʼ�������
//��ʼ�������
//��ʼ�������λ
//��ʼ����� adc
//��ʼ���������ص�����
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
//��� IO ��ƽ��ѯ
//������������
//����ֵ��FALSE :δ�ڿ�λ��TRUE���ڿ�λ
//=======================
uint8_t  bsp_motor_check(uint8_t type, uint8_t id)
{
	if(g_driverboardType == 1)//������ź�����ͨ�����෴
	{
		return (check_position(type, id));
	}
	else//��ͨ����
	{
		return (!check_position(type, id));
	}
}

//=======================
//�������
//������������
//=======================
void bsp_motor_start(uint8_t id)
{
	row_start(id/10, id%2);//����ͨ��ʱ�ȿ�ͨ���� 
	line_start(id%10);
}
//=======================
//���ֹͣ
//������������
//=======================
void bsp_motor_stop(void)
{
	line_stop();//�ر�ͨ��ʱ��رյ���
	row_stop(); 
}
//================================
//��������ɼ�
//================================
uint16_t  bsp_current_get(void)
{
	uint8_t i;
	uint16_t data[SAMPLING_ANT],rs;
	for(i = 0;i < SAMPLING_ANT;i++)
	{
		adc1_67_start();//��ʼ���� 
		while(adc67Geted == RESET);
		adc67Geted = RESET;
		data[i] = current;
	}
	current = 0;
//	for(i = 0,rs = 0;i < SAMPLING_ANT;i++)//�������ȥ����һ�������һ����ȥ�м�����ֵƽ����
//	rs += data[i];
	for(i = 0,rs = 0;i < (SAMPLING_ANT);i++)//�������ȥ����һ�������һ����ȥ�м�����ֵƽ����
	rs += data[i];
	rs /= (SAMPLING_ANT);
	if(rs > 100)
		rs = rs;
	return 	rs ;
}

 
