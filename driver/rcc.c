//========================================
//�û�ʱ������
//���ܣ������û��Զ�������ϵͳʱ�ӵ�
//========================================
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "rcc.h"

void user_rcc(void) 
{
  	RCC_DeInit();					//�ָ� RCC Ĭ��ֵ
  	RCC_HSEConfig(RCC_HSE_ON);		//�����ⲿ���پ���
  	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)//���ָ�� RCC ��־�Ƿ���λ (�˴���HSE ����)
	{
		//���������ϵͳ��־���
		;
	}	
	
	//ʹ��flash Ԥȡָ����
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//����flash
	//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable);	//���� flash �����ڷ���
    FLASH_SetLatency(FLASH_Latency_2);						//���� flash ��ʱʱ�����ڣ�Ϊ��flash ��д���ȶ�����д��Ҫʱ�䣩
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	//���� HCLK -> AHB = ϵͳʱ��
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);		//���ø��� APB2 -> APB2 = HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);		//���õ��� APB1 -> APB1 = HCLK/2

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//���� PLL ʱ��Դ�ڱ�Ƶϵ����PLL ʱ��Դ HSE��9��Ƶ��
	
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);					//ʹ�� PLL

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	//�ȴ� PLL ����

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);			//����ϵͳʱ�ӣ�ѡ�� PLL��Ϊϵͳʱ��

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);				//����ϵͳʱ�����õ�ʱ��Դ
	
//	//�������� GPIO ����ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
//         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
//         | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
}
