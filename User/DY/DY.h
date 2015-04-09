#ifndef __DY_H
#define	__DY_H

#include "stm32f10x.h"
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态
/*
DBP_CL = E4;	
DBP_DA = E2;
DBP_INT = E0;
*/
/* 定义控制IO的宏 */
#define DBP_INT_TOGGLE		digitalToggle(GPIOA,GPIO_Pin_9)
#define DBP_INT_OFF		    digitalHi(GPIOA,GPIO_Pin_9)
#define DBP_INT_ON			digitalLo(GPIOA,GPIO_Pin_9)

#define DBP_DA_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_5)
#define DBP_DA_OFF	     	digitalHi(GPIOC,GPIO_Pin_5)	
#define DBP_DA_ON			digitalLo(GPIOC,GPIO_Pin_5)

#define DBP_CL_TOGGLE		digitalToggle(GPIOC,GPIO_Pin_4)
#define DBP_CL_OFF	    	digitalHi(GPIOC,GPIO_Pin_4)
#define DBP_CL_ON			digitalLo(GPIOC,GPIO_Pin_4)

void DY_GPIO_Config(void);
void OPV(void);
void Up_sendscreen_16  (const unsigned char Pic[4][128]);
void Down_sendscreen_16(const unsigned char Pic[4][128]);
void Left_sendscreen_16 (const unsigned char Pic[4][128]);
void Right_sendscreen_16(const unsigned char Pic[4][128]);
void FlappyBird_Left_Roll_Display_sendB  (unsigned char Display_State[16][16])  ;
void FlappyBird_Right_Roll_Display_sendB (unsigned char Display_State[16][16])  ;
void FlappyBird_Up_Roll_Display_sendB    (unsigned char Display_State[16][16])  ;
void FlappyBird_Down_Roll_Display_sendB  (unsigned char Display_State[16][16])  ;
void Roll_Display_Right_sendB(unsigned char Display_State[16][16],unsigned char Word_Color) ;
void Roll_Display_Left_sendB(unsigned char Display_State[16][16],unsigned char Word_Color) ;
void Roll_Display_Up_sendB(unsigned char Display_State[16][16],unsigned char Word_Color) ;
void Roll_Display_Down_sendB(unsigned char Display_State[16][16],unsigned char Word_Color) ;
void DA(char da_wei);
void DBP_sendscreen_16(void);
void delayms (unsigned int a);
void DBP_sendB(unsigned char b);
void DBP_delay(unsigned char n);
void LightChange_Roll_Display_Right_sendB(unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light) ;
void LightChange_Roll_Display_Left_sendB(unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light) ;
void LightChange_Roll_Display_Up_sendB(unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light) ;
void LightChange_Roll_Display_Down_sendB(unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light) ;
#endif /* __DY_H */
