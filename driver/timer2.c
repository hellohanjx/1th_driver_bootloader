//============================
//��ʱ�������ļ�
//stm32 ��ʱ��Ϊ16λ
//stm32 ��ʱ��1 ����
//ע�⣺stm32f103RBT6	ֻ��3��ͨ�ü�ʱ����1���߼���ʱ��
//============================
#include "stm32f10x.h"
#include "stdint.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "timer2.h"

static TIMER2_CALLBACK	timer_callBack = 0;

//�жϴ�����
static void  timr2_it_fun(TIMER2_CALLBACK callBack)
{
	if(callBack == 0)
		return;
	callBack();
}
//�ص�������ʼ��
void timer2_callbackFun_init(TIMER2_CALLBACK fun)
{
	timer_callBack	= fun;
}
//============================
//����1 ��ʱ��
//˵����ʹ�ö�ʱ�� 2
//���ܣ���ʼ������1��ʱ��
//============================
void timer2_config(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //����ʱ��2ʱ�ӣ�APB1���36 M��

	TIM_TimeBaseStructure.TIM_Period = 0;      					//�Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER2_PRE_SCALER;   	//Ԥ��Ƶ����������ʱ�ӷ��䣩2kHZ 0.5ms
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ����� ��
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; 	//ʱ�ӷָ� 4��Ƶ�����ֻ����Ϊ��������ã�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //������ʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //��ʼ�� time2 ����������
	
	//���з��� TIM_TimeBaseInit ���棬���⿪���ͽ����ж�
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                   //��������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//ʹ��ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);                            	//��ʼ���ж�
	TIM_Cmd(TIM2, DISABLE);                                      //timer2 ʹ��
}
//============================
//����1 ��ʱ��
//˵�����˶�ʱ����λ(ms)
//���ܣ����������ڽ����ã��������ٸ��ֽڵ�ʱ��δ���յ����ݣ���Ϊ�������
//����: count ��ʱʱ��
//============================
void timer2_start(uint16_t count)
{
	TIM_SetAutoreload(TIM2, count);//�ı���װֵ
	TIM_SetCounter(TIM2, 0);		//���������ֵ
	TIM_Cmd(TIM2, ENABLE);
}
//===============================
//TIM2 �ж�
//===============================
void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    { 
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIM_Cmd(TIM2, DISABLE);
		//�ص�����
		timr2_it_fun(timer_callBack);
    } 
} 

