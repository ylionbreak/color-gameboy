#include"shu.h"
unsigned int NumListGe[10]  = {0x0022,0x00e7,0x000b,0x0043,0x00c6,0x0052,0x00012,0x00e3,0x0002,0x0042};
unsigned int NumListShi[10] = {0x0600,0xb700,0x0b00,0x8300,0xb200,0xc200,0x4200,0xa700,0x0200,0x8200};

void shu_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��������ܵ�����ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 
	/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��GPIOB0*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_Write(GPIOD, 0xffff);
}
void DisplayScore(unsigned int number)
{
	unsigned int Num_Data;
	Num_Data=NumListShi[number/10]+NumListGe[number%10];
	GPIO_Write(GPIOD ,Num_Data);		 
}
void DisplayScoreClear(void)
{
	GPIO_Write(GPIOD ,0xffff);		 
}
