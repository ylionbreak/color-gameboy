#include "music.h"   
extern unsigned int time;
void Music_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		/*开启LED的外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 
		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6;	
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOE, &GPIO_InitStructure);	
		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOE, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6);
}
/*6 播放 4 下一首 2 上一首 0 暂停*/
void Next_Wait(void)
{
	time=0;
	while(time<300);
	time=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_4);/*下一首*/
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_4);
	while(time<300);
	time=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);/*暂停*/
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}
void Before_Wait(void)
{
	while(time<300);
	time=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_2);/*上一首*/
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_2);
	while(time<300);
	time=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);/*暂停*/
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);	

}
void PlayMusic(void)
{

	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	while(time<400);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}
void StopMusic(void)
{

	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}
/*********************************************END OF FILE**********************/
