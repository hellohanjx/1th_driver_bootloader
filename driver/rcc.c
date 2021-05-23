//========================================
//用户时钟配置
//功能：根据用户自定义配置系统时钟等
//========================================
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"
#include "rcc.h"

void user_rcc(void) 
{
  	RCC_DeInit();					//恢复 RCC 默认值
  	RCC_HSEConfig(RCC_HSE_ON);		//设置外部高速晶振
  	while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)//检查指定 RCC 标志是否置位 (此处是HSE 就绪)
	{
		//这里可以由系统日志输出
		;
	}	
	
	//使能flash 预取指缓存
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//加速flash
	//FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Disable);	//禁能 flash 半周期访问
    FLASH_SetLatency(FLASH_Latency_2);						//设置 flash 延时时钟周期（为了flash 读写的稳定，读写需要时间）
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);	//设置 HCLK -> AHB = 系统时钟
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);		//设置高速 APB2 -> APB2 = HCLK

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);		//设置低速 APB1 -> APB1 = HCLK/2

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//设置 PLL 时钟源于倍频系数（PLL 时钟源 HSE；9倍频）
	
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);					//使能 PLL

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);	//等待 PLL 启动

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);			//设置系统时钟，选择 PLL作为系统时钟

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);				//返回系统时钟所用的时钟源
	
//	//启动所有 GPIO 引脚时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
//         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
//         | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
}
