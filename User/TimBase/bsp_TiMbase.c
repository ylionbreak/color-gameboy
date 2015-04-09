#include "bsp_TiMbase.h" 
/// TIM7�ж����ȼ�����
void TIM7_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 10ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM7_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	/* ����TIM7CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
    //TIM_DeInit(TIM2);
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=700;	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	/* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;	
	/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);	
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);	
    TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);	
    TIM_Cmd(TIM7, ENABLE);																		 
	/*�ȹرյȴ�ʹ��*/    
}
/*********************************************END OF FILE**********************/
