#ifndef __Music_H
#define	__Music_H

#include "stm32f10x.h"

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define BEEP_TOGGLE		digitalToggle(GPIOA,GPIO_Pin_1)
#define BEEP_OFF			digitalHi(GPIOA,GPIO_Pin_1)
#define BEEP_ON		digitalLo(GPIOA,GPIO_Pin_1)

void Music_GPIO_Config(void);
void Next_Wait(void);
void Before_Wait(void);
void PlayMusic(void);
void StopMusic(void);

#endif /* __Music_H */
