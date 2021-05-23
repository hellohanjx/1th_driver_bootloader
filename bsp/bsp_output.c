//=======================
//������� bsp��
//�� ҵ����ṩ�ӿ�
//=======================
#include "bsp_output.h"
#include "stm32f10x.h"
#include "ctrl_output.h"
 #include "sys.h"
//=======================
//�����ʼ��
//=======================
void bsp_output_init(void)
{
	output_config();
}
//=======================
//����ͨ��
//˵����Ŀǰ��6��ͨ��
//=======================
void  bsp_output_open(uint8_t  id)
{
	
	if(id == O_COLD)//����
	{
		set_PB2();
	}
	else
	if(id == O_WIND)//���
	{
		set_PB12();
	}
	else
	if(id == O_LIGHT)//����
	{
		set_PB13();
	}
	else
	if(id == O_UV)//����
	{
		set_PB14();
	}
	else
	if(id == O_PREP1)//����1
	{
		set_PB15();
	}
	else
	if(id == O_PREP2)//����2
	{
		set_PC6();
	}

}
//=======================
//�ر�ͨ��
//˵����Ŀǰ��6��ͨ��
//=======================
void  bsp_output_close(uint8_t id)
{
	if(id == O_COLD)//����
	{
		reset_PB2();
	}
	else
	if(id == O_WIND)//���
	{
		reset_PB12();
	}
	else
	if(id == O_LIGHT)//����
	{
		reset_PB13();
	}
	else
	if(id == O_UV)//����
	{
		reset_PB14();
	}
	else
	if(id == O_PREP1)//����1
	{
		reset_PB15();
	}
	else
	if(id == O_PREP2)//����2
	{
		reset_PC6();
	}
}
//=======================
//�ر�����ͨ��
//˵����Ŀǰ��6��ͨ��
//=======================
void  bsp_output_reset(void)
{
	//ȫ���ر�
	reset_PB2();
	reset_PB12();
	reset_PB13();
	reset_PB14();
	reset_PB15();
	reset_PC6();
}

/*
���ƻ�·���
*/
uint8_t bsp_ex_check(uint8_t id)
{
	uint8_t rs = 0;
	switch(id)
	{
		case O_COLD ://����
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
		break;
		
		case O_WIND ://���
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
		break;
			
		case O_LIGHT ://����
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
		break;
		
		case O_UV:	//����
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
		break;
		
		case O_PREP1 : //����1
			rs = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
		break;
		
		case O_PREP2 : //����2
			rs = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);
		break;
	}
	rs &= 0x1;
	return rs;
}

