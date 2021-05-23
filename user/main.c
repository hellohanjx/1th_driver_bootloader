//=========================================================================
//主程序
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

#define FLASH_APP_ADDR		(0x08008000)  	//第一个应用程序起始地址(存放在FLASH),保留0X08000000~0X0800FFFF的空间为IAP使用
typedef  void (*pFunction)(void);

#define PAGE_SIZE  2048		//大容量每页2K,小容量每页1K
#define UDDATA_SIZE 1024

pFunction Jump_To_Application;
volatile uint32_t JumpAddress;

enum{ cmd_app = 1, cmd_updata, cmd_wait, cmd_null};//主机下发的命令类型

/*
系统参数
*/
typedef struct SYS_STATUS
{
	uint32_t curTime;//当前时间记录
	uint8_t cur_cmd;//当前执行命令
	uint32_t startTime;//启动间隔
	
}SYS_STATUS;
volatile SYS_STATUS sys_status;

/*
升级参数数据结构
*/
typedef struct UPDATA
{
	uint32_t addr;//升级数据写入地址
	uint32_t len;	//升级包长度
	uint8_t buf[UDDATA_SIZE+4];//升级数据缓冲
	uint8_t buf_lock;//升级缓冲锁（数据写完后才能像缓冲中写数据）
	uint32_t num;//升级包序号
}UPDATA;


typedef struct RECV_DATA
{
	uint8_t m_addr;//主板地址
	uint8_t s_addr;//驱动板地址
	uint8_t sqnr;	//流水号
	UPDATA updata;//升级参数
}RECV_DATA;

volatile RECV_DATA recvData;

uint16_t yy;
/*
跳转到用户程序
*/
void run_to_app(void)
{
	uint32_t flag = *(__IO uint16_t*)(FLASH_APP_ADDR) ;
	if(flag != 0xffff)//如果是0xffff，说明没有app程序
	{
	
		__set_PRIMASK(1);//关全局中断

		//跳转到用户程序
		JumpAddress = *(__IO uint32_t*) (FLASH_APP_ADDR+4);
		Jump_To_Application = (pFunction) JumpAddress;         

		//中断向量表转移
		__set_MSP(*(__IO uint32_t*) FLASH_APP_ADDR);          //初始化用户程序的堆栈指针

		Jump_To_Application();                                //跳转至APP执行
	}
	else
	{
		sys_status.curTime = bsp_rtcTime_get();//刷新程序，继续等待
	}
}

/*
写flash
参数：dat：待写入数据；len：待写入长度
返回值：TRUE：成功；FALSE：失败
*/
uint8_t wrt_flash(uint8_t *dat, uint16_t len)
{
	uint32_t i, count = 0; 
	unsigned long *ptr;
	
	FLASH_Status fStatus = FLASH_COMPLETE;
	recvData.updata.num = (dat[0] << 24) | (dat[1] << 16) | (dat[2] << 8) | dat[3] ;
	ptr = (unsigned long *) (&dat[4]);
	len -= 4;
	
//	__set_PRIMASK(1);//关全局中断
	FLASH_Unlock ();//解锁flash

	if((recvData.updata.addr % (PAGE_SIZE)) == 0)//擦除待写入区域
	{
		fStatus = FLASH_ErasePage(UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR);
	}
	
	if(fStatus == FLASH_COMPLETE)
	{
		//将读到的数据都写入flash ,每次写1 个字 4Byte
		for(i = 0; (i < (len / 4)) && (fStatus == FLASH_COMPLETE); i++)
		{
			uint8_t j = 0;
			//最多允许重写3次
			do{
				fStatus = FLASH_ProgramWord(count + UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR, ptr[i]);   //写到片内的FLASH(按字/4Byte写)
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
				fStatus = FLASH_ProgramWord(count + UDDATA_SIZE*(recvData.updata.num - 1) + FLASH_APP_ADDR, ptr[i]);   //写到片内的FLASH(按字/4Byte写)
			}while (fStatus != FLASH_COMPLETE && j++<3);
		}
	}
	else
	{
		return FALSE;
	}
	
	FLASH_Lock ();
//	__set_PRIMASK(0);//开全局中断

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
主循环
*/
int main(void)
{
	uint8_t *data, chk;
	uint16_t len, i;
	bsp_system_init();
	gpio_init();//2018.5.19添加
	addr_config();
	bsp_timer_init();
	bsp_led_init();
	bsp_master_init();
	sys_status.curTime = bsp_rtcTime_get();//开始计时
	sys_status.startTime = 3;
	while(1)
	{
		if(get_message())//收到数据
		{
			LED_RECV_ON;//led开
			delay_ms(10);
			LED_RECV_OFF;//led关
			
			data = get_recv_buf(&len);//获取数据
			//数据处理
			if(data[0] == 0xaa)//包头对
			{
				recvData.sqnr = data[1];//记录流水号
				if(len == (data[2] << 8 | data[3]) )//包长对
				{
					if(get_board_addr() == data[5])//驱动板地址对
					{
						recvData.m_addr = data[4];//记录主机地址
						for(i = 0, chk = 0; i < len-1; i++)//计算校验
						 chk += data[i];
						if(chk == data[len - 1])//校验对
						{
							switch(data[6])
							{
								case cmd_app://跳转到用户app
									send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
									run_to_app();
								break;
								
								case cmd_wait://等待主机命令
									send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
									sys_status.curTime = bsp_rtcTime_get();
									sys_status.startTime = 5;
								break;
								
								case cmd_updata://接受升级数据包
								{
									sys_status.curTime = bsp_rtcTime_get();
									sys_status.startTime = 5;
									if(recvData.updata.buf_lock == 0)//锁住缓存区
									{
										uint16_t wrtLen = len - 8;//待写入数据长度（提取包中的升级数据）
										recvData.updata.buf_lock = 1;
										for(i = 0; i < wrtLen; i++)
											recvData.updata.buf[i] = data[i+7];//丢到升级缓存
										/*
										开始升级
										*/
										if(wrt_flash((uint8_t*)recvData.updata.buf, wrtLen))//写flash
										{
											send_answer_master(1, recvData.sqnr, recvData.m_addr, data[6]);
										}
										else//写入falsh失败
										{
											send_answer_master(3, recvData.sqnr, recvData.m_addr, data[6]);
										}
										recvData.updata.buf_lock = 0;
									}
									else//缓存区非空
									{
											send_answer_master(4, recvData.sqnr, recvData.m_addr, data[6]);
									}
								}
								break;
								
								default://指令不对
									send_answer_master(2, recvData.sqnr, recvData.m_addr, data[6]);
								break;
							}
						}
						else
						{
							//校验不对
							send_answer_master(5, recvData.sqnr, recvData.m_addr, data[6]);
						}
					}
					else
					{
						//驱动板地址不对
						send_answer_master(6, recvData.sqnr, recvData.m_addr, data[6]);
					}
				}
				else
				{
					//包长不对
					send_answer_master(7, recvData.sqnr, recvData.m_addr, data[6]);
				}
			}
			else
			{
				//包头不对
				send_answer_master(8, recvData.sqnr, recvData.m_addr, data[6]);
			}			
		}
		delay_ms(20);//这个必须加上，否则会有问题（现象是通信时有时会卡住）
		
		if(bsp_rtcTime_get() - sys_status.curTime > sys_status.startTime) //超过3秒
		{
			run_to_app();
		}
	}
}




