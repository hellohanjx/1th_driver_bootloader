//=========================================================================
//������
//=========================================================================
#include "stdint.h"
#include "bsp_others.h"
#include "bsp_led.h"
#include "bsp_master.h"
#include "delay.h"
#include "bsp_timer.h"
#include "master_protocol.h"
#include "stm32f10x_flash.h"
#include "address.h"

#include "uart1.h"
#include "led.h"
#include "gpio_init.h"

#define FLASH_APP_ADDR		(0x08008000)  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH),����0X08000000~0X0800FFFF�Ŀռ�ΪIAPʹ��
typedef  void (*pFunction)(void);

#define PAGE_SIZE  2048		//������ÿҳ2K,С����ÿҳ1K
#define UDDATA_SIZE 1024

pFunction Jump_To_Application;
volatile uint32_t JumpAddress;

enum{ cmd_app = 1, cmd_updata, cmd_wait, cmd_null};//�����·�����������

/*
ϵͳ����
*/
typedef struct SYS_STATUS
{
	uint32_t curTime;//��ǰʱ���¼
	uint8_t cur_cmd;//��ǰִ������
	uint32_t startTime;//�������
	
}SYS_STATUS;
volatile SYS_STATUS sys_status;

/*
�����������ݽṹ
*/
typedef struct UPDATA
{
	uint32_t addr;//��������д���ַ
	uint32_t len;	//����������
	uint8_t buf[UDDATA_SIZE+4];//�������ݻ���
	uint8_t buf_lock;//����������������д�������񻺳���д���ݣ�
	uint32_t num;//���������
}UPDATA;


typedef struct RECV_DATA
{
	uint8_t m_addr;//�����ַ
	uint8_t s_addr;//�������ַ
	uint8_t sqnr;	//��ˮ��
	UPDATA updata;//��������
}RECV_DATA;

volatile RECV_DATA recvData;

uint16_t yy;
/*
��ת���û�����
*/
void run_to_app(void)
{
	uint32_t flag = *(__IO uint16_t*)(FLASH_APP_ADDR) ;
	if(flag != 0xffff)//�����0xffff��˵��û��app����
	{
	
		__set_PRIMASK(1);//��ȫ���ж�

		//��ת���û�����
		JumpAddress = *(__IO uint32_t*) (FLASH_APP_ADDR+4);
		Jump_To_Application = (pFunction) JumpAddress;         

		//�ж�������ת��
		__set_MSP(*(__IO uint32_t*) FLASH_APP_ADDR);          //��ʼ���û�����Ķ�ջָ��

		Jump_To_Application();                                //��ת��APPִ��
	}
	else
	{
		sys_status.curTime = bsp_rtcTime_get();//ˢ�³��򣬼����ȴ�
	}
}

/*
дflash
������dat����д�����ݣ�len����д�볤��
����ֵ��TRUE���ɹ���FALSE��ʧ��
*/
uint8_t wrt_flash(uint8_t *dat, uint16_t len)
{
	uint32_t i, count = 0; 
	unsigned long *ptr;
	
	FLASH_Status fStatus = FLASH_COMPLETE;
	recvData.updata.num = (dat[0] << 24) | (dat[1] << 16) | (dat[2] << 8) | dat[3] ;
	ptr = (unsigned long *) (&dat[4]);
	len -= 4;
	
//	__set_PRIMASK(1);//��ȫ���ж�
	FLASH_Unlock ();//����flash

	if((recvData.updata.addr % (PAGE_SIZE)) == 0)//������д������
	{
		fStatus = FLASH_ErasePage(UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR);
	}
	
	if(fStatus == FLASH_COMPLETE)
	{
		//�����������ݶ�д��flash ,ÿ��д1 ���� 4Byte
		for(i = 0; (i < (len / 4)) && (fStatus == FLASH_COMPLETE); i++)
		{
			uint8_t j = 0;
			//���������д3��
			do{
				fStatus = FLASH_ProgramWord(count + UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR, ptr[i]);   //д��Ƭ�ڵ�FLASH(����/4Byteд)
				if(fStatus == FLASH_COMPLETE)
				{
					count += 4;
				}
				else
				{
					count = count;
				}
			}while (fStatus != FLASH_COMPLETE && j++<3);
		}
		if(len < 4)
		{
			uint8_t j = 0;
			do{
				fStatus = FLASH_ProgramWord(count + UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR, ptr[i]);   //д��Ƭ�ڵ�FLASH(����/4Byteд)
			}while (fStatus != FLASH_COMPLETE && j++<3);
		}
	}
	else
	{
		return FALSE;
	}
	
	FLASH_Lock ();
//	__set_PRIMASK(0);//��ȫ���ж�

	if(fStatus == FLASH_COMPLETE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
��ѭ��
*/
int main(void)
{
	uint8_t *data, chk;
	uint16_t len, i;
	bsp_system_init();
	gpio_init();//2018.5.19���
	addr_config();
	bsp_timer_init();
	bsp_led_init();
	bsp_master_init();
	sys_status.curTime = bsp_rtcTime_get();//��ʼ��ʱ
	sys_status.startTime = 3;
	while(1)
	{
		if(get_message())//�յ�����
		{
			LED_RECV_ON;//led��
			delay_ms(10);
			LED_RECV_OFF;//led��
			
			data = get_recv_buf(&len);//��ȡ����
			//���ݴ���
			if(data[0] == 0xaa)//��ͷ��
			{
				recvData.sqnr = data[1];//��¼��ˮ��
				if(len == (data[2] << 8 | data[3]) )//������
				{
					if(get_board_addr() == data[5])//�������ַ��
					{
						recvData.m_addr = data[4];//��¼������ַ
						for(i = 0, chk = 0; i < len-1; i++)//����У��
						 chk += data[i];
						if(chk == data[len - 1])//У���
						{
							switch(data[6])
							{
								case cmd_app://��ת���û�app
									send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
									run_to_app();
								break;
								
								case cmd_wait://�ȴ���������
									send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
									sys_status.curTime = bsp_rtcTime_get();
									sys_status.startTime = 5;
								break;
								
								case cmd_updata://�����������ݰ�
								{
									sys_status.curTime = bsp_rtcTime_get();
									sys_status.startTime = 5;
									if(recvData.updata.buf_lock == 0)//��ס������
									{
										uint16_t wrtLen = len - 8;//��д�����ݳ��ȣ���ȡ���е��������ݣ�
										recvData.updata.buf_lock = 1;
										for(i = 0; i < wrtLen; i++)
											recvData.updata.buf[i] = data[i+7];//������������
										/*
										��ʼ����
										*/
										if(wrt_flash((uint8_t*)recvData.updata.buf, wrtLen))//дflash
										{
											send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
										}
										else//д��falshʧ��
										{
											send_answer_master(3, recvData.sqnr, recvData.m_addr, data[6]);
										}
										recvData.updata.buf_lock = 0;
									}
									else//�������ǿ�
									{
											send_answer_master(4, recvData.sqnr, recvData.m_addr, data[6]);
									}
								}
								break;
								
								default://ָ���
									send_answer_master(2, recvData.sqnr, recvData.m_addr, data[6]);
								break;
							}
						}
						else
						{
							//У�鲻��
							send_answer_master(5, recvData.sqnr, recvData.m_addr, data[6]);
						}
					}
					else
					{
						//�������ַ����
						send_answer_master(6, recvData.sqnr, recvData.m_addr, data[6]);
					}
				}
				else
				{
					//��������
					send_answer_master(7, recvData.sqnr, recvData.m_addr, data[6]);
				}
			}
			else
			{
				//��ͷ����
				send_answer_master(8, recvData.sqnr, recvData.m_addr, data[6]);
			}			
		}
		delay_ms(20);//���������ϣ�����������⣨������ͨ��ʱ��ʱ�Ῠס��
		
		if(bsp_rtcTime_get() - sys_status.curTime > sys_status.startTime) //����3��
		{
			run_to_app();
		}
	}
}




