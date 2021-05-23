#include "misc.h"
#include "delay.h"

//==============================
//下面两个是粗糙的延时函数
//根据原子哥测算，stm32 72M时一条指令执行时间大约70ns
//注意：我测试的指令为166ns/条
//==============================
//微秒延时
void delay_us(uint16_t time) 
{         
	uint16_t i=0;      
	while(time--)    
	{        
		i=10;  //????       
		while(i--);         
	} 
}  
//毫秒延时
void delay_ms(uint16_t time) 
{         
	uint16_t i=0;      
	while(time--)    
	{        
//		i=12000;  
		i = 6000;	//经测试，这里应该使用6000
		while(i--);    
	} 
}
