#include "misc.h"
#include "delay.h"

//==============================
//���������Ǵֲڵ���ʱ����
//����ԭ�Ӹ���㣬stm32 72Mʱһ��ָ��ִ��ʱ���Լ70ns
//ע�⣺�Ҳ��Ե�ָ��Ϊ166ns/��
//==============================
//΢����ʱ
void delay_us(uint16_t time) 
{         
	uint16_t i=0;      
	while(time--)    
	{        
		i=10;  //????       
		while(i--);         
	} 
}  
//������ʱ
void delay_ms(uint16_t time) 
{         
	uint16_t i=0;      
	while(time--)    
	{        
//		i=12000;  
		i = 6000;	//�����ԣ�����Ӧ��ʹ��6000
		while(i--);    
	} 
}
