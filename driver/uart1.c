//====================================
//接口 485
//与主控板通信
//====================================
#include "stdint.h"
#include "stm32f10x.h"
#include "misc.h"
#include "uart1.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "string.h"

#define RX_LENGTH 1100
#define TX_LENGTH 100
static volatile uint8_t rx_buffer[RX_LENGTH];
static volatile uint16_t rx_len;
static volatile uint8_t tx_buffer[TX_LENGTH];
static volatile uint8_t message;//1表示收到数据，0表示没有数据

/*
发送消息到主循环
*/
static void send_message(void)
{
	message = 1;
}
/*
获取消息
*/
uint8_t get_message(void)
{
	uint8_t rs = message;
	message = 0;//清除消息
	return rs;
}

//====================================
//通讯串口初始化
//串口 UART1 配置
//====================================
void usart1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//gpio 类型
	USART_InitTypeDef USART_InitStructure;	//uasrt 类型
	NVIC_InitTypeDef NVIC_InitStructure;	//中断类型

	//使能 GPIO时钟，引脚功能复用时钟，
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA  ,  ENABLE );

	//TX 引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RX 引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//串口配置
	USART_InitStructure.USART_BaudRate            = 57600  ;				//波特率
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;           	//8个数据位
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;              	//1个停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;              	//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//禁用硬件流控制
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //使能发送/接收
	USART_Init(USART1, &USART_InitStructure);
	
	
	//串口中断配置
//	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);          //使能接收中断
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);				//使能错误中断
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE );
//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);                                  
//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

//	USART_ClearFlag(USART1,USART_FLAG_RXNE); 
	USART_ClearFlag(USART1,USART_FLAG_TC); 

	USART_Cmd(USART1, ENABLE);     	//使能 usart1 外设
	
	//=============================
	//中断使能，对应到中断向量表
	//=============================
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		                       	//USART1接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                        //从优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//中断通道使能
	NVIC_Init(&NVIC_InitStructure);
}
//====================================
//uart1 串口 DMA配置
//====================================
void uart1_dmaRx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;				//中断类型
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMA1 时钟使能
	
	DMA_DeInit(DMA1_Channel5); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);		//dr寄存器地址 (0x40013804)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx_buffer;				//接收缓冲地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//外设作为数据来源
	DMA_InitStructure.DMA_BufferSize = RX_LENGTH;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//dma 不自增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//内存自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//dma数据宽度 字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//循环缓冲
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//优先级高，多个使用才有用
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		//关掉内存到内存
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		
	DMA_ClearFlag(DMA1_FLAG_GL1);       //清除DMA全部标志

//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);//传输完成中断
//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);//传输错误中断
	/* Enable USART1 DMA TX request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);	//使能DMA接收
	DMA_Cmd(DMA1_Channel5, ENABLE);					//打开DMA1-5接收
	
		
	//=============================
	//中断使能，对应到中断向量表
	//=============================
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; 		                //DMA1-5接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //先占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                        //从优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//中断通道使能
	NVIC_Init(&NVIC_InitStructure);
}

/*
DMA1-4
串口1发送DMA
*/
void uart1_dmaTx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;				
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMA1 ????

	DMA_DeInit(DMA1_Channel4); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)tx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;				//?????????
	DMA_InitStructure.DMA_BufferSize = TX_LENGTH;							//DMA????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//DMA???????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//DMA??????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//8bit ????,dma
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//8bit ????,??
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;					//??????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;				//????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					//???????
	
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);					//?????
	
	DMA_ClearFlag(DMA1_FLAG_GL1);       // ??DMA????   

	
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);					//?????
//	DMA_ITConfig(DMA1_Channel4, DMA_IT_TE, ENABLE);					//??????
	
	/* Enable USART1 DMA TX request */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel4, DISABLE);				//??DMA1-4??
	
	//=============================
	//DMA????
	//=============================
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn; 		                //????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                       //?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		                        //????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//????
	NVIC_Init(&NVIC_InitStructure);

}





//DMA1_Channel5中断服务函数	UART1接收完成
uint8_t DMA_fl = 0;
void DMA1_Channel5_IRQHandler(void)
{
	DMA_fl = !DMA_fl;
	if(DMA_GetITStatus(DMA1_IT_TC5) == SET)    
	{
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}
	if(DMA_GetITStatus(DMA1_IT_TE5) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TE5);
	}
	DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA,防止处理其间有数据
	DMA1_Channel5->CNDTR = RX_LENGTH;//重装填
	DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
}

/*
DMA1-4
发送中断
*/
void DMA1_Channel4_IRQHandler(void)
{
//	DMA_fl = !DMA_fl;
	if(DMA_GetITStatus(DMA1_IT_TC4) == SET)    
	{
		DMA_ClearITPendingBit(DMA1_IT_TC4);
	}
	if(DMA_GetITStatus(DMA1_IT_TE4) == SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TE4);
	}
	DMA_Cmd(DMA1_Channel4, DISABLE);//?dma
}

/*
DMA发送数据
*/
void usart1_send(uint8_t *dat, uint16_t size)  
{
	memcpy((uint8_t*)tx_buffer, dat, size);//拷贝数据到发送区
    DMA_Cmd(DMA1_Channel4, DISABLE);//??DMA??
    DMA1_Channel4->CNDTR = size; //重载
    DMA_Cmd(DMA1_Channel4, ENABLE);//??DMA??
}


//====================================
//发送
//====================================
//void usart1_send(uint8_t data)
//{
//	//TXE 弹仓空
//	//TC 枪膛空
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET );//貌似得用这个
////	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );//
//	USART_SendData(USART1,data);
////	   while (!(USART1->SR & USART_FLAG_TXE));
////   USART1->DR = (data & (uint16_t)0x01FF);	 

//}


//====================================
//串口1中断
//====================================
void USART1_IRQHandler(void)
{
	uint32_t  sr;
	sr = sr;//为了不报错
	
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//空闲中断
	{
		sr = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		//获取DMA接接收数据长度
		rx_len = RX_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel5);//这个和手上手册的函数不同了
		DMA_Cmd(DMA1_Channel5, DISABLE);//关闭DMA,防止处理其间有数据
		if(rx_len != 0)
		{
			send_message();
		}
		if(rx_len != 0x40c)
		{
			rx_len = rx_len;
		}
		DMA1_Channel5->CNDTR = RX_LENGTH;//重装填
		DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA
	}
	
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)//发送空中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)//发送完成中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);                                  
	}
	
	//==================================
	//接收数据
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		sr = USART1->SR;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		rx_buffer[rx_len++] = USART1->DR;
		
		
//		rx_len %= 100;
//		rx_buffer[rx_len++]=USART_ReceiveData(USART1);//读取接收到的数据USART1->DR,自动清除标志位
	}
	
	if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)//溢出错误中断
	{//RDR 中数据未读，但是移位寄存器中有新数据要送到RDR 中
		sr = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_ORE);//这个其实可以不要。（因为是靠软件序列来清除的）
	}
	

	if(USART_GetITStatus(USART1 ,USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//出错
	{//这个貌似没有开那个中断使能，检测不到
		sr = USART_ReceiveData(USART1);
		//USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
}

/*
获取数据长度与数据的接口
*/
uint8_t* get_recv_buf(uint16_t*len)
{
	 *len = rx_len;
	return (uint8_t*)rx_buffer;
}
