//====================================
//�ӿ� 485
//�����ذ�ͨ��
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
static volatile uint8_t message;//1��ʾ�յ����ݣ�0��ʾû������

/*
������Ϣ����ѭ��
*/
static void send_message(void)
{
	message = 1;
}
/*
��ȡ��Ϣ
*/
uint8_t get_message(void)
{
	uint8_t rs = message;
	message = 0;//�����Ϣ
	return rs;
}

//====================================
//ͨѶ���ڳ�ʼ��
//���� UART1 ����
//====================================
void usart1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//gpio ����
	USART_InitTypeDef USART_InitStructure;	//uasrt ����
	NVIC_InitTypeDef NVIC_InitStructure;	//�ж�����

	//ʹ�� GPIOʱ�ӣ����Ź��ܸ���ʱ�ӣ�
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA  ,  ENABLE );

	//TX ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RX ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��������
	USART_InitStructure.USART_BaudRate            = 57600  ;				//������
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;           	//8������λ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;              	//1��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_No ;              	//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����Ӳ��������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //ʹ�ܷ���/����
	USART_Init(USART1, &USART_InitStructure);
	
	
	//�����ж�����
//	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);          //ʹ�ܽ����ж�
	USART_ITConfig(USART1,USART_IT_ERR,ENABLE);				//ʹ�ܴ����ж�
	
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE );
//	USART_ITConfig(USART1, USART_IT_TC, ENABLE);                                  
//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

//	USART_ClearFlag(USART1,USART_FLAG_RXNE); 
	USART_ClearFlag(USART1,USART_FLAG_TC); 

	USART_Cmd(USART1, ENABLE);     	//ʹ�� usart1 ����
	
	//=============================
	//�ж�ʹ�ܣ���Ӧ���ж�������
	//=============================
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 		                       	//USART1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                        //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
}
//====================================
//uart1 ���� DMA����
//====================================
void uart1_dmaRx_config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;				//�ж�����
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMA1 ʱ��ʹ��
	
	DMA_DeInit(DMA1_Channel5); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);		//dr�Ĵ�����ַ (0x40013804)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx_buffer;				//���ջ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//������Ϊ������Դ
	DMA_InitStructure.DMA_BufferSize = RX_LENGTH;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//dma ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//�ڴ�����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//dma���ݿ�� �ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�ڴ����ݿ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//���ȼ��ߣ����ʹ�ò�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		//�ص��ڴ浽�ڴ�
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
		
	DMA_ClearFlag(DMA1_FLAG_GL1);       //���DMAȫ����־

//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);//��������ж�
//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);//��������ж�
	/* Enable USART1 DMA TX request */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);	//ʹ��DMA����
	DMA_Cmd(DMA1_Channel5, ENABLE);					//��DMA1-5����
	
		
	//=============================
	//�ж�ʹ�ܣ���Ӧ���ж�������
	//=============================
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn; 		                //DMA1-5�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;                       //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                        //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

/*
DMA1-4
����1����DMA
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





//DMA1_Channel5�жϷ�����	UART1�������
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
	DMA_Cmd(DMA1_Channel5, DISABLE);//�ر�DMA,��ֹ�������������
	DMA1_Channel5->CNDTR = RX_LENGTH;//��װ��
	DMA_Cmd(DMA1_Channel5, ENABLE);//������,�ؿ�DMA
}

/*
DMA1-4
�����ж�
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
DMA��������
*/
void usart1_send(uint8_t *dat, uint16_t size)  
{
	memcpy((uint8_t*)tx_buffer, dat, size);//�������ݵ�������
    DMA_Cmd(DMA1_Channel4, DISABLE);//??DMA??
    DMA1_Channel4->CNDTR = size; //����
    DMA_Cmd(DMA1_Channel4, ENABLE);//??DMA??
}


//====================================
//����
//====================================
//void usart1_send(uint8_t data)
//{
//	//TXE ���ֿ�
//	//TC ǹ�ſ�
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET );//ò�Ƶ������
////	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET );//
//	USART_SendData(USART1,data);
////	   while (!(USART1->SR & USART_FLAG_TXE));
////   USART1->DR = (data & (uint16_t)0x01FF);	 

//}


//====================================
//����1�ж�
//====================================
void USART1_IRQHandler(void)
{
	uint32_t  sr;
	sr = sr;//Ϊ�˲�����
	
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)//�����ж�
	{
		sr = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		//��ȡDMA�ӽ������ݳ���
		rx_len = RX_LENGTH - DMA_GetCurrDataCounter(DMA1_Channel5);//����������ֲ�ĺ�����ͬ��
		DMA_Cmd(DMA1_Channel5, DISABLE);//�ر�DMA,��ֹ�������������
		if(rx_len != 0)
		{
			send_message();
		}
		if(rx_len != 0x40c)
		{
			rx_len = rx_len;
		}
		DMA1_Channel5->CNDTR = RX_LENGTH;//��װ��
		DMA_Cmd(DMA1_Channel5, ENABLE);//������,�ؿ�DMA
	}
	
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)//���Ϳ��ж�
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)//��������ж�
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);                                  
	}
	
	//==================================
	//��������
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
		sr = USART1->SR;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		rx_buffer[rx_len++] = USART1->DR;
		
		
//		rx_len %= 100;
//		rx_buffer[rx_len++]=USART_ReceiveData(USART1);//��ȡ���յ�������USART1->DR,�Զ������־λ
	}
	
	if(USART_GetITStatus(USART1, USART_IT_ORE) != RESET)//��������ж�
	{//RDR ������δ����������λ�Ĵ�������������Ҫ�͵�RDR ��
		sr = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_ORE);//�����ʵ���Բ�Ҫ������Ϊ�ǿ��������������ģ�
	}
	

	if(USART_GetITStatus(USART1 ,USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//����
	{//���ò��û�п��Ǹ��ж�ʹ�ܣ���ⲻ��
		sr = USART_ReceiveData(USART1);
		//USART_ClearITPendingBit(USART1, USART_IT_PE | USART_IT_FE | USART_IT_NE);
	}
}

/*
��ȡ���ݳ��������ݵĽӿ�
*/
uint8_t* get_recv_buf(uint16_t*len)
{
	 *len = rx_len;
	return (uint8_t*)rx_buffer;
}
