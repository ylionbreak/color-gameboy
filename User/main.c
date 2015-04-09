#include "stm32f10x.h"
#include "bsp_spi_flash.h"
#include "bsp_SysTick.h"
#include "bsp_TiMbase.h"
#include "DY.h"
#include "key.h"
#include "led.h"
#include "shu.h"
#include "music.h"
#include "words.h"
#include "game.c"
extern void save(unsigned char savestate[]);
extern void FlappyBird(char aFlappyBird_State);
extern void dead(unsigned int score,unsigned char SelectScore);
extern void Remember(void);
/*状态定义*/
unsigned char Select_Array[16][16]= {0};
char xsta,ysta;
unsigned char random1,random2,random3,random4,random5,random6,random7;/*random1为tim7值更新，random2为按键上时间，random3为按键下时间，random4为按键左时间，random5为按键右时间,random6为按键确认时间*/
unsigned char high_score;
unsigned int time;

unsigned char Save_State[10]= {0};		 /* 保存状态数组 x1 y1 x2 y2 bird_position score time timeorder Bird_Color Gtime*/
unsigned char dir;
int main(void)
{
	int x,y;
	unsigned char LightOrDark;/*0为变亮1为变暗*/
	unsigned char Roll_Color;
	char lightlevel;
	unsigned char DisplayTimes;
	int RollDisplayTimes;
	DY_GPIO_Config();
	KEY_GPIO_Config();										/* 设置按键GPIO端口 */
	shu_GPIO_Config();									    /* 设置数码管GPIO端口 */
	SPI_FLASH_Init();										/* 设置FlashGPIO端口 */
	SysTick_Init();											/* SysTick 定时配置 1ms  中断一次  */
	TIM7_Configuration();									/* TIM7    定时配置 0.7ms中断一次  */
	TIM7_NVIC_Configuration();
	Music_GPIO_Config();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
	BEEP_OFF;
	random1=0;
	random2=0;
	random3=0;
	DisplayScoreClear();
	RollDisplayTimes=0;
	time=0;
	dir = 1;

	for(x=0; x<16; x++)
		for(y=0; y<16; y++)
			Select_Array[y][x]=	QianFang[y][x];
	Roll_Color=(random1+random2+random3+random7)%7+1;
	while(RollDisplayTimes<120-16)
	{
		if(time>=30)/*变亮*/
		{
			Roll_Color=(random1+random2+random3+random7)%7+1;
			time=0;
			RollDisplayTimes++;
			for(x=0; x<16; x++)
				for(y=RollDisplayTimes; y<16+RollDisplayTimes; y++)
					Select_Array[x][y-RollDisplayTimes]=QianFang[x][y];
		}
		/*滚动显示文字*/
		{
			dir=FourDirection();
			if(dir==4)
			{
				Roll_Display_Right_sendB(Select_Array,Roll_Color);
			}
			else if(dir==2)
			{
				Roll_Display_Down_sendB(Select_Array,Roll_Color);
			}
			else if(dir==3)
			{
				Roll_Display_Left_sendB(Select_Array,Roll_Color);
			}
			else if(dir==1)
			{
				Roll_Display_Up_sendB(Select_Array,Roll_Color);
			}
		}
	}
	OPV();
	/*初始化状态参数*/
	xsta=0;
	ysta=0;
	dir = 1;
	/*初始化滚动显示以便进入时可以循环显示*/
	Roll_Display_Left_sendB(clearscreen,7);
	DisplayTimes=0;
	RollDisplayTimes=0;
	time=0;
	for(x=0; x<16; x++)
		for(y=0; y<16; y++)
			Select_Array[y][x]=GunDongXianShi[y][x];
	Roll_Color=(random1+random2+random3+random7)%7+1;
	DisplayScoreClear();
	while(random1<150);
	/*一级菜单大循环*/
	while(1)
	{
		/*上下选择：滚动显示，FlappyBird  并且更新显示内容*/
		if(Keyscan_Down()==1&&ysta<5)
		{
			ysta++;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y]=0;
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1+random2+random3+random7)%7+1;
			time=0;
			if(ysta==0)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=GunDongXianShi[x][y];
			else if(ysta==1)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=FlappyBird_Word[x][y];
			else if(ysta==2)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=JiYiLiDaTiaoZhan[x][y];
			else if(ysta==3)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=ABDaRen[x][y];
			else if(ysta==4)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=MeiTuHuaHua[x][y];
			else if(ysta==5)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=TanChiShe[x][y];
		}
		else if(Keyscan_Up()==1&&ysta>0)
		{
			ysta--;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y]=0;
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1+random2+random3+random7)%7+1;
			time=0;
			if(ysta==0)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=GunDongXianShi[x][y];
			else if(ysta==1)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=FlappyBird_Word[x][y];
			else if(ysta==2)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=JiYiLiDaTiaoZhan[x][y];
			else if(ysta==3)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=ABDaRen[x][y];
			else if(ysta==4)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=MeiTuHuaHua[x][y];
			else if(ysta==5)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=TanChiShe[x][y];
		}
		/*进入确认二级菜单初始化显示*/
		else if(Keyscan_Enter()==1)
		{
			random7++;
			/*二级菜单初始化*/
			{
				if(ysta==0)
				{
					/*滚动显示队员名字学院初始化*/
					Roll_Display_Left_sendB(clearscreen,7);
					RollDisplayTimes=0;
					Roll_Color=(random1+random2+random3+random7)%7+1;
					lightlevel=0;
					LightOrDark=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=Name[x][y];
				}
				else if(ysta==1)
				{
					xsta=0;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random3)%7+1;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=KaiShiYouXi[x][y];
				}
				else if(ysta==4)
				{
					xsta=0;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random3)%7+1;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=new[x][y];
				}
				else if(ysta==5)
				{
					xsta=0;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random3)%7+1;
					//Before_Wait();
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=Lo[x][y];
				}
				else if(ysta==3)
				{
					LightOrDark=0;
					xsta=0;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random3)%7+1;
					Next_Wait();
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=HuaShuiMu[x][y];
				}
			}
			while(ysta==0)   /*滚动显示队员名字学院*/
			{
				/*屏幕移动一次*/
				if(time>=20+lightlevel*3&&LightOrDark==0)/*变亮*/
				{
					time=0;
					lightlevel++;
					RollDisplayTimes++;
					for(x=0; x<16; x++)
						for(y=RollDisplayTimes; y<16+RollDisplayTimes; y++)
							Select_Array[x][y-RollDisplayTimes]=Name[x][y];
				}
				else if(time>=20+lightlevel*3&&LightOrDark==1)/*变暗*/
				{
					time=0;
					lightlevel--;
					RollDisplayTimes++;
					for(x=0; x<16; x++)
						for(y=RollDisplayTimes; y<16+RollDisplayTimes; y++)
							Select_Array[x][y-RollDisplayTimes]=Name[x][y];
				}
				if(lightlevel>31)
				{
					lightlevel=31;
					LightOrDark=1;
				}
				else if(lightlevel<0)
				{
					lightlevel=0;
					LightOrDark=0;
				}
				/*播放结束初始化继续播放*/
				if(RollDisplayTimes==329-16)
				{
					Roll_Display_Left_sendB(clearscreen,7);
					RollDisplayTimes=0;
					Roll_Color=(random1+random3+random7)%7+1;
					time=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=Name[x][y];
				}
				/*显示文字*/
				{
					dir=FourDirection();
					if(dir==4)
					{
						LightChange_Roll_Display_Right_sendB(Select_Array,Roll_Color,31-lightlevel);
					}
					else if(dir==2)
					{
						LightChange_Roll_Display_Down_sendB(Select_Array,Roll_Color,31-lightlevel);
					}
					else if(dir==3)
					{
						LightChange_Roll_Display_Left_sendB(Select_Array,Roll_Color,31-lightlevel);
					}
					else if(dir==1)
					{
						LightChange_Roll_Display_Up_sendB(Select_Array,Roll_Color,31-lightlevel);
					}
				}
				/*强制退出*/
				if(Keyscan_Esc()==1)
				{
					ysta=0;
					DisplayTimes=0;
					Roll_Display_Left_sendB(clearscreen,7);
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=GunDongXianShi[x][y];
					break;
				}
			}
			while(ysta==1)   /*FlappyBird*/
			{
				DisplayScoreClear();
				if(Keyscan_Right()==1&&xsta<3)   /*二级菜单向右选择*/
				{
					xsta++;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random5+random7)%7+1;
					time=0;
					if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=KaiShiYouXi[x][y];
					else if(xsta==2)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=JiXuYouXi[x][y];
					else if(xsta==3)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=ZuiGaoFen[x][y];
				}
				else if(Keyscan_Left()==1&&xsta>0)     /*二级菜单向右选择*/
				{
					xsta--;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2*random4+random7)%7+1;
					time=0;
					if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=KaiShiYouXi[x][y];
					else if(xsta==2)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=JiXuYouXi[x][y];
					else if(xsta==3)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=ZuiGaoFen[x][y];
				}
				else if(Keyscan_Enter()==1)
				{
					random7++;
					SPI_FLASH_BufferRead(&high_score,0x000000,1);/*首先读取游戏最高分*/
					while(xsta==0)   /*开始游戏*/
					{
						FlappyBird(1);
						break;
					}
					while(xsta==1)/*继续游戏*/
					{
						FlappyBird(2);
						break;
					}
					while(xsta==2)   /*最高分*/
					{
						DisplayScore(high_score);
						DisplayTimes=0;
						Roll_Display_Left_sendB(clearscreen,7);
						Roll_Color=(random1+random7+random2)%7+1;
						time=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=ZuiGaoFen[x][y];
						while(1)
						{
							if(time>=50)
							{
								time=0;
								DisplayTimes++;
								for(x=0; x<16; x++)
									for(y=DisplayTimes; y<16+DisplayTimes; y++)
										Select_Array[x][y-DisplayTimes]=ZuiGaoFen[x][y];
							}
							if(DisplayTimes==53)   /*播放结束返回上级*/
							{

								break;
							}
							/*滚动显示文字*/
							{
								dir=FourDirection();
								if(dir==4)
								{
									Roll_Display_Right_sendB(Select_Array,Roll_Color);
								}
								else if(dir==2)
								{
									Roll_Display_Down_sendB(Select_Array,Roll_Color);
								}
								else if(dir==3)
								{
									Roll_Display_Left_sendB(Select_Array,Roll_Color);
								}
								else if(dir==1)
								{
									Roll_Display_Up_sendB(Select_Array,Roll_Color);
								}
							}
						}
						DisplayTimes=0;
						time=0;
						DisplayScoreClear();
						xsta=1;
						ysta=1;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=KaiShiYouXi[x][y];
						break;
					}
				}
				else if(Keyscan_Esc()==1)
				{
					xsta=0;
					ysta=1;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					time=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=FlappyBird_Word[x][y];
					break;
				}
				/*滚动一次时间到移动一次*/
				if(time>=50)
				{
					DisplayTimes++;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=KaiShiYouXi[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=JiXuYouXi[x][y];
					else if(xsta==2)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=ZuiGaoFen[x][y];
				}
				/*播放结束初始化继续播放*/
				if(DisplayTimes==66&&xsta==0)   /*开始游戏的字显示*/
				{
					DisplayTimes=0;
					Roll_Color=(random1+random2*random4)%7+1;
					time=0;
					for(y=0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=KaiShiYouXi[x][y];
				}
				else if(DisplayTimes==66&&xsta==1)     /*继续游戏的字显示*/
				{
					DisplayTimes=0;
					Roll_Color=(random1*random2+random4)%7+1;
					time=0;
					for(y= 0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=JiXuYouXi[x][y];
				}
				else if(DisplayTimes==53&&xsta==2)     /*最高分的字的显示*/
				{
					DisplayTimes=0;
					Roll_Color=(random1+random2+random4)%7+1;
					time=0;
					for(y= 0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=ZuiGaoFen[x][y];
				}
				/*滚动显示文字*/
				{
					dir=FourDirection();
					if(dir==4)
					{
						Roll_Display_Right_sendB(Select_Array,Roll_Color);
					}
					else if(dir==2)
					{
						Roll_Display_Down_sendB(Select_Array,Roll_Color);
					}
					else if(dir==3)
					{
						Roll_Display_Left_sendB(Select_Array,Roll_Color);
					}
					else if(dir==1)
					{
						Roll_Display_Up_sendB(Select_Array,Roll_Color);
					}
				}
			}
			while(ysta==2)   /*Remeber*/
			{
				SPI_FLASH_BufferRead(&high_score,0x002000,1);
				Remember();
			}
			while(ysta==3)	/*AB达人*/
			{
				if(Keyscan_Right()==1&&xsta<1)   /*二级菜单向右选择*/
				{
					xsta++;
					Next_Wait();
					/*按键更新初始化显示内容*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					LightOrDark=0;
					Roll_Color=(random1+random2+random5+random7)%7+1;
					time=0;
					if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=DOUBLE[x][y];
				}
				else if(Keyscan_Left()==1&&xsta>0)     /*二级菜单向右选择*/
				{
					xsta--;
					Before_Wait();
					LightOrDark=0;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2*random4+random7)%7+1;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=HuaShuiMu[x][y];
				}
				else if(Keyscan_Esc()==1)
				{
					xsta=0;
					ysta=3;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					time=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=ABDaRen[x][y];
					break;
				}
				else if(Keyscan_Enter()==1)
				{
					random7++;
					while(xsta==0)   /*花水木*/
					{
						MusicGame(1);
						xsta=0;
						DisplayTimes=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=HuaShuiMu[x][y];
						ysta=3;
						break;
					}
					while(xsta==1)	 /*DOUBLE*/
					{
						MusicGame(2);
						xsta=1;
						DisplayTimes=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=HuaShuiMu[x][y];
						ysta=3;
						break;
					}
				}
				/*滚动一次时间到移动一次*/
				if(time>=50&&LightOrDark==0)
				{
					DisplayTimes++;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=HuaShuiMu[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=DOUBLE[x][y];
				}
				else if(time>=50&&LightOrDark==1)
				{
					DisplayTimes--;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=HuaShuiMu[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=DOUBLE[x][y];
				}
				/*播放结束初始化继续播放*/
				if(DisplayTimes==30&&xsta==0)   /*花水木的字显示*/
				{
					DisplayTimes=29;
					LightOrDark=1;
					Roll_Color=(random1+random2*random4)%7+1;
					time=0;
					for(x=0; x<16; x++)
						for(y=DisplayTimes; y<16+DisplayTimes; y++)
							Select_Array[x][y-DisplayTimes]=HuaShuiMu[x][y];
				}
				else if(DisplayTimes==0&&xsta==0)   /*DOUBLE的字显示*/
				{
					DisplayTimes=1;
					LightOrDark=0;
					Roll_Color=(random1+random2*random4)%7+1;
					time=0;
					for(y=0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=HuaShuiMu[x][y];
				}
				if(DisplayTimes==34&&xsta==1)     /*花水木的字显示*/
				{
					DisplayTimes=33;
					LightOrDark=1;
					Roll_Color=(random1*random2+random4)%7+1;
					time=0;
					for(y= 0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=JiXuYouXi[x][y];
				}
				else if(DisplayTimes==0&&xsta==1)     /*DOUBLE的字显示*/
				{
					DisplayTimes=1;
					LightOrDark=0;
					Roll_Color=(random1*random2+random4)%7+1;
					time=0;
					for(y= 0; y<16; y++)
						for(x=0; x<16; x++)
							Select_Array[x][y]=JiXuYouXi[x][y];
				}
				/*滚动显示文字*/
				{
					dir=FourDirection();
					if(dir==4)
					{
						Roll_Display_Right_sendB(Select_Array,Roll_Color);
					}
					else if(dir==2)
					{
						Roll_Display_Down_sendB(Select_Array,Roll_Color);
					}
					else if(dir==3)
					{
						Roll_Display_Left_sendB(Select_Array,Roll_Color);
					}
					else if(dir==1)
					{
						Roll_Display_Up_sendB(Select_Array,Roll_Color);
					}
				}
			}
			while(ysta==4) /*美图画画*/
			{
				if(Keyscan_Right()==1&&xsta<4)   /*二级菜单向右选择*/
				{
					xsta++;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random5+random7)%7+1;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=one[x][y];
					else if(xsta==2)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=two[x][y];
					else if(xsta==3)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=three[x][y];
				}
				else if(Keyscan_Left()==1&&xsta>0)     /*二级菜单向右选择*/
				{
					xsta--;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2*random4+random7)%7+1;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=one[x][y];
					else if(xsta==2)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=two[x][y];
					else if(xsta==3)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=three[x][y];
				}
				else if(Keyscan_Enter()==1)
				{
					random7++;
					while(xsta==0)   /*开始游戏*/
					{
						painting(0);
						xsta=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
						break;
					}
					while(xsta==1)	 /*1*/
					{
						painting(1);
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
						break;
					}
					while(xsta==2)   /*2*/
					{
						painting(2);
						xsta=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
						break;
					}
					while(xsta==3)   /*3*/
					{
						painting(3);
						xsta=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=new[x][y];
						break;
					}
				}
				else if(Keyscan_Esc()==1)
				{
					xsta=0;
					ysta=4;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					time=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=MeiTuHuaHua[x][y];
					break;
				}
				/*滚动显示文字*/
				{
					dir=FourDirection();
					if(dir==4)
					{
						Roll_Display_Right_sendB(Select_Array,Roll_Color);
					}
					else if(dir==2)
					{
						Roll_Display_Down_sendB(Select_Array,Roll_Color);
					}
					else if(dir==3)
					{
						Roll_Display_Left_sendB(Select_Array,Roll_Color);
					}
					else if(dir==1)
					{
						Roll_Display_Up_sendB(Select_Array,Roll_Color);
					}
				}
			}
			while(ysta==5) /*贪吃蛇*/
			{
				if(Keyscan_Right()==1&&xsta<1)   /*二级菜单向右选择*/
				{
					xsta++;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random5+random7)%7+1;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Lo[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Hi[x][y];
				}
				else if(Keyscan_Left()==1&&xsta>0)     /*二级菜单向右选择*/
				{
					xsta--;
					/*按键更新初始化显示内容：开始游戏 继续游戏 最高分*/
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2*random4+random7)%7+1;
					time=0;
					if(xsta==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Lo[x][y];
					else if(xsta==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Hi[x][y];
				}
				else if(Keyscan_Esc()==1)
				{
					xsta=0;
					ysta=5;
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					time=0;
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							Select_Array[x][y]=TanChiShe[x][y];
					break;
				}
				else if(Keyscan_Enter()==1)
				{
					random7++;
					//PlayMusic();
					while(xsta==0)   /*lo*/
					{
						snake(300);
						xsta=0;
						DisplayTimes=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Lo[x][y];
						ysta=5;
						break;
					}
					while(xsta==1)	 /*hi*/
					{
						snake(150);
						xsta=1;
						DisplayTimes=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=Hi[x][y];
						ysta=5;
						break;
					}
					Before_Wait();
					Before_Wait();
					Before_Wait();
				}
				/*滚动显示文字*/
				{
					dir=FourDirection();
					if(dir==4)
					{
						Roll_Display_Right_sendB(Select_Array,Roll_Color);
					}
					else if(dir==2)
					{
						Roll_Display_Down_sendB(Select_Array,Roll_Color);
					}
					else if(dir==3)
					{
						Roll_Display_Left_sendB(Select_Array,Roll_Color);
					}
					else if(dir==1)
					{
						Roll_Display_Up_sendB(Select_Array,Roll_Color);
					}
				}
			}
		}
		/*滚动一次时间到移动一次*/
		if(time>=50)
		{
			time=0;
			DisplayTimes++;
			if(xsta==0&&ysta==0)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=GunDongXianShi[x][y];
			else if(xsta==0&&ysta==1)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=FlappyBird_Word[x][y];
			else if(xsta==0&&ysta==2)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=JiYiLiDaTiaoZhan[x][y];
			else if(xsta==0&&ysta==3)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=ABDaRen[x][y];
			else if(xsta==0&&ysta==4)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=MeiTuHuaHua[x][y];
			else if(xsta==0&&ysta==5)
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=TanChiShe[x][y];
		}
		/*播放结束初始化继续播放*/
		if(DisplayTimes==66&&ysta==0)   /*滚动显示到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1+random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y]=GunDongXianShi[x][y];
		}
		else if(DisplayTimes==93&&ysta==1)     /*像素鸟到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1*random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y-DisplayTimes]=FlappyBird_Word[x][y];
		}
		else if(DisplayTimes==74&&ysta==2)     /*记忆力大挑战到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1*random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y-DisplayTimes]=JiYiLiDaTiaoZhan[x][y];
		}
		else if(DisplayTimes==60&&ysta==3)     /*AB达人到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1*random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y-DisplayTimes]=ABDaRen[x][y];
		}
		else if(DisplayTimes==65&&ysta==4)     /*美图画画到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1*random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y-DisplayTimes]=MeiTuHuaHua[x][y];
		}
		else if(DisplayTimes==77-16&&ysta==5)     /*好吃的蛇到时间了*/
		{
			Roll_Display_Left_sendB(clearscreen,7);
			DisplayTimes=0;
			Roll_Color=(random1*random2+random3)%7+1;
			time=0;
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					Select_Array[x][y-DisplayTimes]=TanChiShe[x][y];
		}
		/*滚动显示文字*/
		{
			dir=FourDirection();
			if(dir==4)
			{
				Roll_Display_Right_sendB(Select_Array,Roll_Color);
			}
			else if(dir==2)
			{
				Roll_Display_Down_sendB(Select_Array,Roll_Color);
			}
			else if(dir==3)
			{
				Roll_Display_Left_sendB(Select_Array,Roll_Color);
			}
			else if(dir==1)
			{
				Roll_Display_Up_sendB(Select_Array,Roll_Color);
			}
		}
	}
}
