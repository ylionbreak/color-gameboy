#include "led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*����LED������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE); 
		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		/*ѡ��Ҫ���Ƶ�����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		/* �ر�����led��	*/
		GPIO_SetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_3);	 
}
/*********************************************END OF FILE**********************/