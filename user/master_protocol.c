//=========================
//������ͨѶʹ�õ�Э��
//=========================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "master_protocol.h" 
#include "stm32f10x.h"
#include "address.h" 

/*
���ܣ���Ӧ����
������rs�����->1.ִ�гɹ���2.��֧�ֵ�ָ�3.д��flashʧ�ܣ�4.���ݻ������ǿ�
5.У���
sqnr:��ˮ�ţ�m_addr:������ַ��cmd����Ӧ��Ӧ������
*/
void send_answer_master(uint8_t rs, uint8_t sqnr, uint8_t m_addr, uint8_t cmd)
{
	uint8_t str[50],i,j,chk;
	
	i = 0;
	str[i++] = 0xbb;			//��ͷ
	str[i++] = sqnr;//��ˮ��
	str[i++] = ' ';				//�������ֽ�
	str[i++] = ' ';				//�������ֽ�
	str[i++] = m_addr;		//������ַ
	str[i++] = get_board_addr();//�������ַ
	str[i++] = cmd;
	str[i++] = rs;
	str[2] = (i + 1) <<8;
	str[3] = (i + 1) & 0xff;
	
	for(j = 0, chk = 0; j < i; j++)//����У���
	{
		chk += str[j];
	}
	str[i++] = chk;
	delay_ms(50);
	usart1_send(str,i);
}
