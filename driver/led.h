#ifndef __LED_H__
#define __LED_H__

#include "stm32f10x.h"


#define LED_BREATH_OFF 		GPIO_SetBits(GPIOA, GPIO_Pin_0);  
#define LED_BREATH_ON 		GPIO_ResetBits(GPIOA, GPIO_Pin_0); 
#define LED_RECV_OFF 		GPIO_SetBits(GPIOA, GPIO_Pin_1);  
#define LED_RECV_ON			GPIO_ResetBits(GPIOA, GPIO_Pin_1); 

void led_config(void);

#endif
