//============================
//定时器驱动文件
//stm32 定时器为16位
//stm32 定时器1 特殊
//注意：stm32f103RBT6	只有3个通用计时器，1个高级定时器
//============================
#include "stm32f10x.h"
#include "stdint.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "timer2.h"

static TIMER2_CALLBACK	timer_callBack = 0;

//中断处理函数
static void  timr2_it_fun(TIMER2_CALLBACK callBack)
{
	if(callBack == 0)
		return;
	callBack();
}
//回调函数初始化
void timer2_callbackFun_init(TIMER2_CALLBACK fun)
{
	timer_callBack	= fun;
}
//============================
//串口1 定时器
//说明：使用定时器 2
//功能：初始化串口1定时器
//============================
void timer2_config(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //开定时器2时钟（APB1最高36 M）

	TIM_TimeBaseStructure.TIM_Period = 0;      					//自动重装载器的值
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER2_PRE_SCALER;   	//预分频（将计数器时钟分配）2kHZ 0.5ms
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数 关
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; 	//时钟分割 4分频（这个只有作为输出才有用）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数方式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //初始化 time2 计数器配置
	
	//这行放在 TIM_TimeBaseInit 后面，避免开机就进入中断
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                   //允许更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//使能通道中断
	NVIC_Init(&NVIC_InitStructure);                            	//初始化中断
	TIM_Cmd(TIM2, DISABLE);                                      //timer2 使能
}
//============================
//串口1 定时器
//说明：此定时器单位(ms)
//功能：用来做串口接收用，超过多少个字节的时间未接收到数据，认为接收完毕
//参数: count 定时时长
//============================
void timer2_start(uint16_t count)
{
	TIM_SetAutoreload(TIM2, count);//改变重装值
	TIM_SetCounter(TIM2, 0);		//清除计数器值
	TIM_Cmd(TIM2, ENABLE);
}
//===============================
//TIM2 中断
//===============================
void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    { 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, DISABLE);
		//回调函数
		timr2_it_fun(timer_callBack);
    } 
} 

