/*
random1为tim7值更新，
random2为按键上时间，
random3为按键下时间，
random4为按键左时间，
random5为按键右时间,
random6为按键取消时间
内存管理：
0x000000 鸟最高分 1
0x001000 鸟状态存储
0x002000 第一幅画
0x003000 第二幅画
0x004000 第三幅画
0x005000 remeber最高分 2
0x006000 花水木最高分 3
0x007000 DOUBLEVISION最高分 4
0x008000 低难度 5
0x009000 高难度 6
*/
extern unsigned char random1;
extern unsigned char random2;
extern unsigned char random3;
extern unsigned char random4;
extern unsigned char random5;
extern unsigned char random6;
extern unsigned char random7;
extern unsigned char Select_Array[16][16];
extern char xsta,ysta;
extern unsigned char dir;
unsigned char Gtime;
extern unsigned int time;
extern unsigned char high_score;
extern unsigned char Save_State[10];		 /* 保存状态数组 x1 y1 x2 y2 bird_position score time timeorder Bird_Color Gtime*/
void FlappyBird(char FlappyBird_State) ;
void dead(unsigned int score,unsigned char SelectScore) ;
void save(unsigned char savestate[]);
void Remember(void);
void MusicGame(unsigned char songorder);
void snake(unsigned int snakelevel);
void FlappyBird(char FlappyBird_State)
{
	unsigned char GtimeArray[11]= {150,150,82,63,53,42,39,36,34,32,31};
	unsigned char BirdColorArray[3]= {1,4,5};
	unsigned char x,y;
	unsigned char x1_flag,x2_flag,Aflag;
	unsigned char Display_State[16][16]= {0};
	unsigned int score;
	unsigned char Bird_Color;
	unsigned char timeorder;
	signed char x1,y1,x2,y2;/*x1与x1差距 y1与y2为下管长 上下管距离为5  程序中要减一*/
	char Bird_Position;/*Y轴坐栿 X轴坐标为4 程序中要凿*/
	/*****************GAME STRAT******************/
	x1_flag=0;
	x2_flag=0;
	Aflag=0;
	random7++;
	time=0;
	if(FlappyBird_State==1)   /*重新开始游戏*/
	{
		y1=(random1+random2)%10;
		y2=(random1+random3)%10;
		Bird_Color=BirdColorArray[(random1+random3+random7)%3];
		score=0;
		Bird_Position=8;
		timeorder=0;
		x1=6+13;
		x2=14+13;
		time=0;
		Gtime=0;
	}
	else if(FlappyBird_State==2)     /* 读取状态数为x1 y1 x2 y2 bird_position score time timeorder Bird_Color Gtime*/
	{
		SPI_FLASH_BufferRead(Save_State, 0x001000, 10);
		x1=Save_State[0];
		y1=Save_State[1];
		x2=Save_State[2];
		y2=Save_State[3];
		Bird_Position=Save_State[4];
		score=Save_State[5];
		time=Save_State[6];
		timeorder=Save_State[7];
		Bird_Color=Save_State[8];
		Gtime=Save_State[9];
	}
	while(1)
	{
		if(Keyscan_Select()==1)   /*存档*/
		{
			Save_State[0]=x1;
			Save_State[1]=y1;
			Save_State[2]=x2;
			Save_State[3]=y2;
			Save_State[4]=Bird_Position;
			Save_State[5]=score;
			Save_State[6]=time;
			Save_State[7]=timeorder;
			Save_State[8]=Bird_Color;
			Save_State[9]=Gtime;	/* 保存状态数为x1 y1 x2 y2 bird_position score time timeorder*/
			save(Save_State);
		}
		if(Keyscan_A()==1&&Aflag==0)   /*跳一次立刻上次*/
		{
			Bird_Position++;
			timeorder=0;
			Gtime=0;
			Aflag=1;
			dir=FourDirection();
			BEEP_ON;
		}
		if(Gtime>=GtimeArray[timeorder]&&timeorder>0)   /*之后加速下次*/
		{
			BEEP_OFF;
			Bird_Position--;
			timeorder++;
			Gtime=0;
			Aflag=0;
			dir=FourDirection();
		}
		else if(Gtime>=GtimeArray[timeorder]&&timeorder==0)     /*再上升一次*/
		{
			Bird_Position++;
			timeorder++;
			Gtime=0;
			Aflag=0;
			dir=FourDirection();
		}
		if(time>=500)   /*屏幕移动*/
		{
			BEEP_OFF;
			x1--;
			x2--;
			time=0;
			Aflag=0;
			dir=FourDirection();
		}
		if(x1==3||x1==2)   /*第一根死亡判断*/
		{
			if( (y1-Bird_Position)>=0 ||( (Bird_Position) - (y1+6) )>=0 )
			{
				Roll_Display_Left_sendB(clearscreen,7);
				dead(score,1);
				break;
			}

		}
		else if(x2==3||x2==2)     /*第二根死亡判断*/
		{
			if( (y2-Bird_Position)>=0 ||( (Bird_Position) - (y2+6) )>=0 )
			{
				Roll_Display_Left_sendB(clearscreen,7);
				dead(score,1);
				break;
			}

		}
		if(x1_flag!=1&&x1<2)
		{
			dir=FourDirection();
			score++;
			x1_flag=1;
		}
		else if(x2_flag!=1&&x2<2)
		{
			dir=FourDirection();
			score++;
			x2_flag=1;
		}
		if(Bird_Position==0)
		{
			dir=FourDirection();
			Roll_Display_Left_sendB(clearscreen,7);
			dead(score,1);
			break;
		}
		else if(Bird_Position>16)
		{
			dir=FourDirection();
			Bird_Position=16;
		}
		if(x1==-1)   /*第一根到屏幕左边*/
		{
			dir=FourDirection();
			x1_flag=0;
			x1=14;
			y1=(random1+random2+random7)%8+2;
		}
		else if(x2==-1)     /*第二根到屏幕左边*/
		{
			dir=FourDirection();
			x2_flag=0;
			x2=14;
			y2=(random7+random3)%8+2;
		}
		/*更改游戏显示内容*/
		{

			for(y=0; y<16; y++)
				for(x=0; x<16; x++)
					Display_State[x][y]=3;
			for(x=0; x<16; x++)
				Display_State[x][0]=6;
			if(x1<=14)
			{
				for(y=1; y<y1; y++)
					Display_State[x1][y]=2;

				for(y=15; y>=y1+5; y--)
					Display_State[x1][y]=2;

				for(y=1; y<y1; y++)
					Display_State[x1+1][y]=2;

				for(y=15; y>=y1+5; y--)
					Display_State[x1+1][y]=2;

			}
			if(x2<=14)
			{
				for(y=1; y<y2; y++)
					Display_State[x2][y]=2;
				for(y=15; y>=y2+5; y--)
					Display_State[x2][y]=2;
				for(y=15; y>=y2+5; y--)
					Display_State[x2+1][y]=2;
				for(y=1; y<y2; y++)
					Display_State[x2+1][y]=2;
			}
			Display_State[3][Bird_Position-1]=Bird_Color;
		}
		/*显示分数*/
		DisplayScore(score);
		/*四向显示*/
		{
			dir=FourDirection();
			if(dir==2)
				FlappyBird_Right_Roll_Display_sendB (Display_State);/*xia*/
			else if(dir==3)
				FlappyBird_Down_Roll_Display_sendB  (Display_State);/*zuo*/
			else if(dir==4)
				FlappyBird_Up_Roll_Display_sendB    (Display_State);/*you*/
			else if(dir==1)
				FlappyBird_Left_Roll_Display_sendB  (Display_State);/*shang*/
		}
	}
}
void dead(unsigned int score,unsigned char SelectScore)   /*死亡函数*/
{
	unsigned char x,y;
	unsigned char Roll_Color;
	unsigned char DisplayTimes;
	if(SelectScore==1)
		SPI_FLASH_BufferRead(&high_score, 0x000000,1);
	else if(SelectScore==2)
		SPI_FLASH_BufferRead(&high_score, 0x005000,1);
	else if(SelectScore==3)
		SPI_FLASH_BufferRead(&high_score, 0x006000,1);
	else if(SelectScore==4)
		SPI_FLASH_BufferRead(&high_score, 0x007000,1);
	else if(SelectScore==5)
		SPI_FLASH_BufferRead(&high_score, 0x008000,1);
	else if(SelectScore==6)
		SPI_FLASH_BufferRead(&high_score, 0x009000,1);
	Roll_Display_Left_sendB(clearscreen,7);
	if(score>high_score)
	{
		high_score=score;
		if(SelectScore==1)
		{
			SPI_FLASH_SectorErase(0x000000);
			SPI_FLASH_BufferWrite(&high_score, 0x000000,1);
		}
		else if(SelectScore==2)
		{
			SPI_FLASH_SectorErase(0x005000);
			SPI_FLASH_BufferWrite(&high_score, 0x005000,1);
		}
		else if(SelectScore==3)
		{
			SPI_FLASH_SectorErase(0x006000);
			SPI_FLASH_BufferWrite(&high_score, 0x006000,1);
		}
		else if(SelectScore==4)
		{
			SPI_FLASH_SectorErase(0x007000);
			SPI_FLASH_BufferWrite(&high_score, 0x007000,1);
		}
		else if(SelectScore==5)
		{
			SPI_FLASH_SectorErase(0x008000);
			SPI_FLASH_BufferWrite(&high_score, 0x008000,1);
		}
		else if(SelectScore==6)
		{
			SPI_FLASH_SectorErase(0x009000);
			SPI_FLASH_BufferWrite(&high_score, 0x009000,1);
		}
		DisplayScore(score);
		DisplayTimes=0;
		Roll_Color=(random1+random2+random3)%7+1;
		time=0;
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
				Select_Array[x-DisplayTimes][y]=ZuiGaoFen[x][y];
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
			if(time<25)
				DisplayScore(score);
			else if(time>25)
				DisplayScoreClear();
			if(DisplayTimes==53)   /*播放结束返回上级*/
			{
				Roll_Display_Left_sendB(clearscreen,7);
				DisplayScoreClear();
				xsta=0;
				ysta=0;
				break;
			}
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
	else
	{
		DisplayScore(score);
		DisplayTimes=0;
		Roll_Color=time%7+1;
		time=0;
		for(y=0; y<10; y++)
			for(x=0; x<16; x++)
				FenShuWei[x][y+55]=DianZhenNumber[score/10][x][y];
		for(y=0; y<10; y++)
			for(x=0; x<16; x++)
				FenShuWei[x][y+67]=DianZhenNumber[score%10][x][y];
		for(y=0; y<16; y++)
			for(x=0; x<16; x++)
				Select_Array[x-DisplayTimes][y]=FenShuWei[x][y];
		while(1)
		{
			if(time>=50)
			{
				time=0;
				DisplayTimes++;
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=FenShuWei[x][y];
			}
			if(DisplayTimes==84-16)   /*播放结束返回上级*/
			{
				Roll_Display_Left_sendB(clearscreen,7);
				DisplayScoreClear();
				xsta=0;
				ysta=0;
				break;
			}
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
}
void save(unsigned char savestate[])
{
	SPI_FLASH_SectorErase(0x001000);
	SPI_FLASH_BufferWrite(savestate,0x001000,10);
}
void Remember(void)
{
	unsigned char score,scanorder,righttime,i,RememberRandom,deathflag;
	unsigned char RandomArray[15]= {1,0,3,2,0,1,2,3,0,1,3,2,0,1,2};
	unsigned char direction[10]  = {0};
	unsigned char judgearray[10] = {0};
	unsigned char x,y;
	unsigned char DisplayTimes,Roll_Color;
	Roll_Display_Left_sendB(clearscreen,7);
	/*产生第一个随机数*/
	direction[0]=RandomArray[(random1+random3)%15];
	deathflag=0;
	scanorder=0;
	righttime=0;
	score=0;
	DisplayTimes=0;
	/*显示一欿*/
	time=0;
	Roll_Display_Up_sendB(Direction[direction[0]],(RememberRandom/random3*random5+random1+random2*random4)%3+1);
	while(time<2311);
	time=0;
	Roll_Color=(random1+3+random2)%7+1;
	Roll_Display_Up_sendB(clearscreen,7);
	for(y=0; y<16; y++)
		for(x=0; x<16; x++)
			Select_Array[x][y]=PinJiYiShuRu[x][y];
	while(1)
	{
		/*进入判断状态*/
		while(1)
		{
			/*四向输入*/
			if(Keyscan_Down()==1)
			{
				judgearray[scanorder]=1;
				scanorder++;
			}
			if(Keyscan_Up()==1)
			{
				judgearray[scanorder]=0;
				scanorder++;
			}
			if(Keyscan_Right()==1)
			{
				judgearray[scanorder]=3;
				scanorder++;
			}
			if(Keyscan_Left()==1)
			{
				judgearray[scanorder]=2;
				scanorder++;
			}
			if( scanorder == (score+1) )
			{
				for(i=0; i<=score; i++)
					if(direction[i]==judgearray[i])
						righttime++;
				if( righttime==(score+1) )
				{
					score++;
					scanorder=0;
					righttime=0;
					DisplayScore(score);
					break;
				}
				else
				{
					dead(score,2);
					deathflag=1;
					break;
				}
			}
			/*显示“输入方向*/
			if(time>=50)
			{
				time=0;
				DisplayTimes++;
				for(x=0; x<16; x++)
					for(y=DisplayTimes; y<16+DisplayTimes; y++)
						Select_Array[x][y-DisplayTimes]=PinJiYiShuRu[x][y];
			}
			if(DisplayTimes==88)   /*播放结束返继续播放*/
			{
				time=0;
				DisplayTimes=0;
				for(y=0; y<16; y++)
					for(x=0; x<16; x++)
						Select_Array[x][y]=PinJiYiShuRu[x][y];
				Roll_Color=(random1+3+random2)%5+1;
				Roll_Display_Left_sendB(clearscreen,7);
			}
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
		if(deathflag==1)
			break;
		else
			for(i=0; i<=score; i++)
			{
				Roll_Display_Up_sendB(clearscreen,7);
				RememberRandom=(RememberRandom+time+random3+1+random1+random2+random4+random5)%15;
				direction[i]=RandomArray[RememberRandom];
				Roll_Display_Up_sendB(Direction[direction[i]],(RememberRandom+random1+random2+random3+random4+random5)%5+1);
				while(time<2311);
				time=0;
				Roll_Display_Up_sendB(clearscreen,7);
				time=0;
				DisplayTimes=0;
				Roll_Color=(random1+3+random2)%5+1;
				for(y=0; y<16; y++)
					for(x=0; x<16; x++)
						Select_Array[x][y]=PinJiYiShuRu[x][y];
			}
	}
}
void MusicGame(unsigned char songorder)
{
	/*x=4,5 10,11 y=3,4*/
	unsigned char DoubleVision[2][87]=/*0A1B 0.25s*/
	{
		/*   1              234                   456                     789                            101112              131415                              */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,4,0,4,4,0,0,4,4,0,0,4,4,4,0,4,4,0,0,4,4,0,0,4,4,4,0,4,4,0,0,4,4,0,0,4,4,4,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	unsigned char flour[2][119]=
	{
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,4,4,0,0,4,4,0,0,0,0,4,4,0,0,4,4,0,0,4,4,0,0,0,0,4,4,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	} ;
	unsigned char DisplayState[16][16]= {0};
	unsigned char x,y;
	unsigned int musicsize,score;
	unsigned char ThisSongOrder;/*i和i+1*/
	unsigned char Aflag,Bflag,Anbeat,Bnbeat,BnbeatTotal,AnbeatTotal,AThisBeatRecord,BThisBeatRecord;
	PlayMusic();
	if(songorder==2)/*DOUBLEVISION*/
	{
		musicsize=87;
		score=0;
		time=0;
		Anbeat=0;
		Bnbeat=0;
		BnbeatTotal=0;
		AnbeatTotal=0;
		AThisBeatRecord=0;
		BThisBeatRecord=0;
		ThisSongOrder=0;/*歌曲顺序 为开头第0个点*/
		Roll_Display_Left_sendB(clearscreen,7);
		while(ThisSongOrder<musicsize-16)
		{
			if(time>=250)
			{
				ThisSongOrder++;
				time=0;
				Aflag=0;
				Bflag=0;
				{
					if( (ThisSongOrder-Bnbeat-BThisBeatRecord)!=0 )
						Bnbeat=0;
					if( (ThisSongOrder-Anbeat-AThisBeatRecord)!=0 )
						Anbeat=0;
				}
				/*改变显示状态*/
				{
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							DisplayState[x][y]=7;
					for(x=0; x<16; x++)
					{
						DisplayState[x][3]=6;
						DisplayState[x][4]=6;
					}
					for(y=0; y<16; y++)
					{
						DisplayState[4][y]=DoubleVision[1][ThisSongOrder+y];
						DisplayState[5][y]=DoubleVision[1][ThisSongOrder+y];
						DisplayState[10][y]=DoubleVision[0][ThisSongOrder+y];
						DisplayState[11][y]=DoubleVision[0][ThisSongOrder+y];
					}
				}
			}
			/* 判断 中了的话周围为蓝色变青色 紫色变红色*/
			if(Keyscan_B()==1&&Bflag==0)
			{
				/*多节拍判断第一个并截取节拍*/
				if(Bnbeat==0&&DoubleVision[1][ThisSongOrder+2]==0&&DoubleVision[1][ThisSongOrder+3]!=0&&DoubleVision[1][ThisSongOrder+4]!=0&&DoubleVision[1][ThisSongOrder+5]!=0)
				{
					Bnbeat=1;
					BnbeatTotal=0;
					BThisBeatRecord=ThisSongOrder;
					DoubleVision[1][ThisSongOrder+3]=0;
					for(x=0; x<100; x++)
					{
						if(DoubleVision[1][ThisSongOrder+4+x]==0)
							break;
						else
							BnbeatTotal++;
					}
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[6][5]=3;
				}
				/*多节拍中间*/
				else if(Bnbeat!=0&&(ThisSongOrder-Bnbeat-BThisBeatRecord)==0&&DoubleVision[1][ThisSongOrder+2]==0&&DoubleVision[1][ThisSongOrder+3]!=0&&DoubleVision[1][ThisSongOrder+4]!=0&&DoubleVision[1][ThisSongOrder+5]!=0)
				{
					Bnbeat++;
					DoubleVision[1][ThisSongOrder+3]=0;
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[6][5]=3;
				}
				/*多节拍结尾*/
				else if(Bnbeat!=0&&(Bnbeat+1)==BnbeatTotal)
				{
					Bnbeat=0;
					score=score+BnbeatTotal+1;
					DoubleVision[1][ThisSongOrder+3]=0;
					DoubleVision[1][ThisSongOrder+4]=0;
					DisplayState[4][4]=1;
					DisplayState[5][4]=1;
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[4][5]=3;
					DisplayState[5][5]=3;
					DisplayState[6][5]=3;

					DisplayState[2][1]=3;
					DisplayState[3][1]=3;
					DisplayState[4][1]=3;
					DisplayState[5][1]=3;
					DisplayState[6][1]=3;
					DisplayState[7][1]=3;
					DisplayState[2][6]=3;
					DisplayState[3][6]=3;
					DisplayState[4][6]=3;
					DisplayState[5][6]=3;
					DisplayState[6][6]=3;
					DisplayState[7][6]=3;
					DisplayState[7][1]=3;
					DisplayState[7][2]=3;
					DisplayState[7][3]=3;
					DisplayState[7][4]=3;
					DisplayState[7][5]=3;
					DisplayState[7][6]=3;
					DisplayState[2][1]=3;
					DisplayState[2][2]=3;
					DisplayState[2][3]=3;
					DisplayState[2][4]=3;
					DisplayState[2][5]=3;
					DisplayState[2][6]=3;
				}
				/*单节拍*/
				else if(Bnbeat==0&&DoubleVision[1][ThisSongOrder+3]!=0&&DoubleVision[1][ThisSongOrder+4]!=0&&DoubleVision[1][ThisSongOrder+5]==0&&DoubleVision[1][ThisSongOrder+2]==0)
				{
					DoubleVision[1][ThisSongOrder+3]=0;
					DoubleVision[1][ThisSongOrder+4]=0;
					DisplayState[5][3]=0;
					DisplayState[4][3]=0;
					DisplayState[5][4]=0;
					DisplayState[4][4]=0;
					score++;
					Bnbeat=0;
					BnbeatTotal=0;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[4][5]=3;
					DisplayState[5][5]=3;
					DisplayState[6][5]=3;
				}
				else if(score>0)
				{
					score--;
					Bnbeat=0;
				}
				Bflag=1;
			}
			/* 判断 中了的话周围为蓝色变青色 紫色变红色*/
			if(Keyscan_A()==1&&Aflag==0)
			{
				/*多节拍判断第一个并截取节拍*/
				if(Anbeat==0&&DoubleVision[0][ThisSongOrder+2]==0&&DoubleVision[0][ThisSongOrder+3]!=0&&DoubleVision[0][ThisSongOrder+4]!=0&&DoubleVision[0][ThisSongOrder+5]!=0)
				{
					Anbeat=1;
					AnbeatTotal=0;
					AThisBeatRecord=ThisSongOrder;
					DoubleVision[0][ThisSongOrder+3]=0;
					for(x=0; x<100; x++)
					{
						if(DoubleVision[0][ThisSongOrder+4+x]==0)
							break;
						else
							AnbeatTotal++;
					}
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[12][5]=5;
				}
				/*多节拍中间*/
				else if(Anbeat!=0&&(ThisSongOrder-Anbeat-AThisBeatRecord)==0&&DoubleVision[0][ThisSongOrder+2]==0&&DoubleVision[0][ThisSongOrder+3]!=0&&DoubleVision[0][ThisSongOrder+4]!=0&&DoubleVision[0][ThisSongOrder+5]!=0)
				{
					Anbeat++;
					DoubleVision[0][ThisSongOrder+3]=0;
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[12][5]=5;
				}
				/*多节拍结尾*/
				else if(Anbeat!=0&&(Anbeat+1)==AnbeatTotal)
				{
					Anbeat=0;
					score=score+AnbeatTotal+1;
					DoubleVision[0][ThisSongOrder+3]=0;
					DoubleVision[0][ThisSongOrder+4]=0;
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[10][4]=4;
					DisplayState[11][4]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[10][5]=5;
					DisplayState[11][5]=5;
					DisplayState[12][5]=5;

					DisplayState[8][1]=5;
					DisplayState[9][1]=5;
					DisplayState[10][1]=5;
					DisplayState[11][1]=5;
					DisplayState[12][1]=5;
					DisplayState[13][1]=5;
					DisplayState[8][6]=5;
					DisplayState[9][6]=5;
					DisplayState[10][6]=5;
					DisplayState[11][6]=5;
					DisplayState[12][6]=5;
					DisplayState[13][6]=5;
					DisplayState[13][1]=5;
					DisplayState[13][2]=5;
					DisplayState[13][3]=5;
					DisplayState[13][4]=5;
					DisplayState[13][5]=5;
					DisplayState[13][6]=5;
					DisplayState[8][1]=5;
					DisplayState[8][2]=5;
					DisplayState[8][3]=5;
					DisplayState[8][4]=5;
					DisplayState[8][5]=5;
					DisplayState[8][6]=5;
				}
				/*单节拍*/
				else if(Anbeat==0&&DoubleVision[0][ThisSongOrder+3]!=0&&DoubleVision[0][ThisSongOrder+4]!=0&&DoubleVision[0][ThisSongOrder+5]==0&&DoubleVision[0][ThisSongOrder+2]==0)
				{
					DoubleVision[0][ThisSongOrder+3]=0;
					DoubleVision[0][ThisSongOrder+4]=0;
					DisplayState[11][3]=0;
					DisplayState[10][3]=0;
					DisplayState[11][4]=0;
					DisplayState[10][4]=0;
					score++;
					Anbeat=0;
					AnbeatTotal=0;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[10][5]=5;
					DisplayState[11][5]=5;
					DisplayState[12][5]=5;
				}
				else if(score>0)
				{
					score--;
					Anbeat=0;
				}
				Aflag=1;
			}
			DisplayScore(score);
			if(DisplayState[5][3]==0)
			{
				DisplayState[5][3]=6;
				DisplayState[4][3]=6;
			}
			if(DisplayState[5][4]==0)
			{
				DisplayState[5][4]=6;
				DisplayState[4][4]=6;
			}
			if(DisplayState[10][3]==0)
			{
				DisplayState[11][3]=6;
				DisplayState[10][3]=6;
			}
			if(DisplayState[10][4]==0)
			{
				DisplayState[11][4]=6;
				DisplayState[10][4]=6;
			}
			/*四向显示*/
			{
				dir=FourDirection();
				if(dir==2)
					FlappyBird_Right_Roll_Display_sendB (DisplayState);/*xia*/
				else if(dir==3)
					FlappyBird_Down_Roll_Display_sendB  (DisplayState);/*zuo*/
				else if(dir==4)
					FlappyBird_Up_Roll_Display_sendB    (DisplayState);/*you*/
				else if(dir==1)
					FlappyBird_Left_Roll_Display_sendB  (DisplayState);/*shang*/
			}

		}
		Roll_Display_Left_sendB(clearscreen,7);
		dead(score,4);
	}
	else if(songorder==1)/*花水木*/
	{
		musicsize=119;
		score=0;
		time=0;
		Anbeat=0;
		Bnbeat=0;
		BnbeatTotal=0;
		AnbeatTotal=0;
		AThisBeatRecord=0;
		BThisBeatRecord=0;
		ThisSongOrder=0;/*歌曲顺序 为开头第0个点*/
		Roll_Display_Left_sendB(clearscreen,7);
		while(ThisSongOrder<musicsize-16)
		{
			if(time>=250)
			{
				ThisSongOrder++;
				time=0;
				Aflag=0;
				Bflag=0;
				{
					if( (ThisSongOrder-Bnbeat-BThisBeatRecord)!=0 )
						Bnbeat=0;
					if( (ThisSongOrder-Anbeat-AThisBeatRecord)!=0 )
						Anbeat=0;
				}
				/*改变显示状态*/
				{
					for(x=0; x<16; x++)
						for(y=0; y<16; y++)
							DisplayState[x][y]=7;
					for(x=0; x<16; x++)
					{
						DisplayState[x][3]=6;
						DisplayState[x][4]=6;
					}
					for(y=0; y<16; y++)
					{
						DisplayState[4][y]=flour[1][ThisSongOrder+y];
						DisplayState[5][y]=flour[1][ThisSongOrder+y];
						DisplayState[10][y]=flour[0][ThisSongOrder+y];
						DisplayState[11][y]=flour[0][ThisSongOrder+y];
					}
					/*DisplayState[4][3]=0;
					DisplayState[4][4]=0;
					DisplayState[5][3]=0;
					DisplayState[5][4]=0;
					DisplayState[10][3]=0;
					DisplayState[10][4]=0;
					DisplayState[11][3]=0;
					DisplayState[11][4]=0;*/
				}
			}
			/* 判断 中了的话周围为蓝色变青色 紫色变红色*/
			if(Keyscan_B()==1&&Bflag==0)
			{
				/*多节拍判断第一个并截取节拍*/
				if(Bnbeat==0&&flour[1][ThisSongOrder+2]==0&&flour[1][ThisSongOrder+3]!=0&&flour[1][ThisSongOrder+4]!=0&&flour[1][ThisSongOrder+5]!=0)
				{
					Bnbeat=1;
					BnbeatTotal=0;
					BThisBeatRecord=ThisSongOrder;
					flour[1][ThisSongOrder+3]=0;
					for(x=0; x<100; x++)
					{
						if(flour[1][ThisSongOrder+4+x]==0)
							break;
						else
							BnbeatTotal++;
					}
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[6][5]=3;
				}
				/*多节拍中间*/
				else if(Bnbeat!=0&&(ThisSongOrder-Bnbeat-BThisBeatRecord)==0&&flour[1][ThisSongOrder+2]==0&&flour[1][ThisSongOrder+3]!=0&&flour[1][ThisSongOrder+4]!=0&&flour[1][ThisSongOrder+5]!=0)
				{
					Bnbeat++;
					flour[1][ThisSongOrder+3]=0;
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[6][5]=3;
				}
				/*多节拍结尾*/
				else if(Bnbeat!=0&&(Bnbeat+1)==BnbeatTotal)
				{
					Bnbeat=0;
					score=score+BnbeatTotal+1;
					flour[1][ThisSongOrder+3]=0;
					flour[1][ThisSongOrder+4]=0;
					DisplayState[4][4]=1;
					DisplayState[5][4]=1;
					DisplayState[4][3]=1;
					DisplayState[5][3]=1;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[4][5]=3;
					DisplayState[5][5]=3;
					DisplayState[6][5]=3;

					DisplayState[2][1]=3;
					DisplayState[3][1]=3;
					DisplayState[4][1]=3;
					DisplayState[5][1]=3;
					DisplayState[6][1]=3;
					DisplayState[7][1]=3;
					DisplayState[2][6]=3;
					DisplayState[3][6]=3;
					DisplayState[4][6]=3;
					DisplayState[5][6]=3;
					DisplayState[6][6]=3;
					DisplayState[7][6]=3;
					DisplayState[7][1]=3;
					DisplayState[7][2]=3;
					DisplayState[7][3]=3;
					DisplayState[7][4]=3;
					DisplayState[7][5]=3;
					DisplayState[7][6]=3;
					DisplayState[2][1]=3;
					DisplayState[2][2]=3;
					DisplayState[2][3]=3;
					DisplayState[2][4]=3;
					DisplayState[2][5]=3;
					DisplayState[2][6]=3;
				}
				/*单节拍*/
				else if(Bnbeat==0&&flour[1][ThisSongOrder+3]!=0&&flour[1][ThisSongOrder+4]!=0&&flour[1][ThisSongOrder+5]==0&&flour[1][ThisSongOrder+2]==0)
				{
					flour[1][ThisSongOrder+3]=0;
					flour[1][ThisSongOrder+4]=0;
					DisplayState[5][3]=0;
					DisplayState[4][3]=0;
					DisplayState[5][4]=0;
					DisplayState[4][4]=0;
					score++;
					Bnbeat=0;
					BnbeatTotal=0;
					DisplayState[3][2]=3;
					DisplayState[4][2]=3;
					DisplayState[5][2]=3;
					DisplayState[6][2]=3;
					DisplayState[3][3]=3;
					DisplayState[6][3]=3;
					DisplayState[3][4]=3;
					DisplayState[6][4]=3;
					DisplayState[3][5]=3;
					DisplayState[4][5]=3;
					DisplayState[5][5]=3;
					DisplayState[6][5]=3;
				}
				else if(score>0)
				{
					score--;
					Bnbeat=0;
				}
				Bflag=1;
			}
			/* 判断 中了的话周围为蓝色变青色 紫色变红色*/
			if(Keyscan_A()==1&&Aflag==0)
			{
				/*多节拍判断第一个并截取节拍*/
				if(Anbeat==0&&flour[0][ThisSongOrder+2]==0&&flour[0][ThisSongOrder+3]!=0&&flour[0][ThisSongOrder+4]!=0&&flour[0][ThisSongOrder+5]!=0)
				{
					Anbeat=1;
					AnbeatTotal=0;
					AThisBeatRecord=ThisSongOrder;
					flour[0][ThisSongOrder+3]=0;
					for(x=0; x<100; x++)
					{
						if(flour[0][ThisSongOrder+4+x]==0)
							break;
						else
							AnbeatTotal++;
					}
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[12][5]=5;
				}
				/*多节拍中间*/
				else if(Anbeat!=0&&(ThisSongOrder-Anbeat-AThisBeatRecord)==0&&flour[0][ThisSongOrder+2]==0&&flour[0][ThisSongOrder+3]!=0&&flour[0][ThisSongOrder+4]!=0&&flour[0][ThisSongOrder+5]!=0)
				{
					Anbeat++;
					flour[0][ThisSongOrder+3]=0;
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[12][5]=5;
				}
				/*多节拍结尾*/
				else if(Anbeat!=0&&(Anbeat+1)==AnbeatTotal)
				{
					Anbeat=0;
					score=score+AnbeatTotal+1;
					flour[0][ThisSongOrder+3]=0;
					flour[0][ThisSongOrder+4]=0;
					DisplayState[10][3]=4;
					DisplayState[11][3]=4;
					DisplayState[10][4]=4;
					DisplayState[11][4]=4;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[10][5]=5;
					DisplayState[11][5]=5;
					DisplayState[12][5]=5;

					DisplayState[8][1]=5;
					DisplayState[9][1]=5;
					DisplayState[10][1]=5;
					DisplayState[11][1]=5;
					DisplayState[12][1]=5;
					DisplayState[13][1]=5;
					DisplayState[8][6]=5;
					DisplayState[9][6]=5;
					DisplayState[10][6]=5;
					DisplayState[11][6]=5;
					DisplayState[12][6]=5;
					DisplayState[13][6]=5;
					DisplayState[13][1]=5;
					DisplayState[13][2]=5;
					DisplayState[13][3]=5;
					DisplayState[13][4]=5;
					DisplayState[13][5]=5;
					DisplayState[13][6]=5;
					DisplayState[8][1]=5;
					DisplayState[8][2]=5;
					DisplayState[8][3]=5;
					DisplayState[8][4]=5;
					DisplayState[8][5]=5;
					DisplayState[8][6]=5;
				}
				/*单节拍*/
				else if(Anbeat==0&&flour[0][ThisSongOrder+3]!=0&&flour[0][ThisSongOrder+4]!=0&&flour[0][ThisSongOrder+5]==0&&flour[0][ThisSongOrder+2]==0)
				{
					flour[0][ThisSongOrder+3]=0;
					flour[0][ThisSongOrder+4]=0;
					DisplayState[11][3]=0;
					DisplayState[10][3]=0;
					DisplayState[11][4]=0;
					DisplayState[10][4]=0;
					score++;
					Anbeat=0;
					AnbeatTotal=0;
					DisplayState[9][2]=5;
					DisplayState[10][2]=5;
					DisplayState[11][2]=5;
					DisplayState[12][2]=5;
					DisplayState[9][3]=5;
					DisplayState[12][3]=5;
					DisplayState[9][4]=5;
					DisplayState[12][4]=5;
					DisplayState[9][5]=5;
					DisplayState[10][5]=5;
					DisplayState[11][5]=5;
					DisplayState[12][5]=5;
				}
				else if(score>0)
				{
					score--;
					Anbeat=0;
				}
				Aflag=1;
			}
			DisplayScore(score);
			if(DisplayState[5][3]==0)
			{
				DisplayState[5][3]=6;
				DisplayState[4][3]=6;
			}
			if(DisplayState[5][4]==0)
			{
				DisplayState[5][4]=6;
				DisplayState[4][4]=6;
			}
			if(DisplayState[10][3]==0)
			{
				DisplayState[11][3]=6;
				DisplayState[10][3]=6;
			}
			if(DisplayState[10][4]==0)
			{
				DisplayState[11][4]=6;
				DisplayState[10][4]=6;
			}
			/*四向显示*/
			{
				dir=FourDirection();
				if(dir==2)
					FlappyBird_Right_Roll_Display_sendB (DisplayState);/*xia*/
				else if(dir==3)
					FlappyBird_Down_Roll_Display_sendB  (DisplayState);/*zuo*/
				else if(dir==4)
					FlappyBird_Up_Roll_Display_sendB    (DisplayState);/*you*/
				else if(dir==1)
					FlappyBird_Left_Roll_Display_sendB  (DisplayState);/*shang*/
			}

		}
		Roll_Display_Left_sendB(clearscreen,7);
		StopMusic();
		dead(score,3);
	}
}
void painting(unsigned char state)
{
	/* demo
	0,0 1,0 2,0
	0,1 1,1 2,1
	0,2 1,2 2,2
	*/
	unsigned char Displapaintystate[16][16]= {0};
	unsigned char SaveReadArray[256];
	unsigned char x,y,selectstate,DisplayTimes,saveorder,Roll_Color,recordpointcolor;
	char paintxsta,paintysta;
	paintxsta=0;
	paintysta=0;
	time=0;
	selectstate=1;
	DisplayTimes=0;
	recordpointcolor=7;
	for(x=0; x<16; x++)
		for(y=0; y<16; y++)
			Displapaintystate[x][y]=7;
	if(state==1)
	{
		SPI_FLASH_BufferRead(SaveReadArray,0x002000,256);
		for(y=0; y<8; y++)
			for(x=0; x<8; x++)
				Displapaintystate[x][y]=SaveReadArray[y*8+x];
	}
	else if(state==2)
	{
		SPI_FLASH_BufferRead(SaveReadArray,0x003000,256);
		for(y=0; y<8; y++)
			for(x=0; x<8; x++)
				Displapaintystate[x][y]=SaveReadArray[y*8+x];
	}
	else if(state==3)
	{
		SPI_FLASH_BufferRead(SaveReadArray,0x004000,256);
		for(y=0; y<8; y++)
			for(x=0; x<8; x++)
				Displapaintystate[x][y]=SaveReadArray[y*8+x];
	}
	while(1)
	{
		/*四向控制*/
		{
			if(Keyscan_Up()==1)
			{
				time=0;
				Displapaintystate[paintxsta][paintysta]=recordpointcolor;
				if(paintysta<15)
					paintysta++;
				recordpointcolor=Displapaintystate[paintxsta][paintysta];
			}
			if(Keyscan_Down()==1)
			{
				time=0;
				Displapaintystate[paintxsta][paintysta]=recordpointcolor;
				if(paintysta>0)
					paintysta--;
				recordpointcolor=Displapaintystate[paintxsta][paintysta];
			}
			if(Keyscan_Right()==1)
			{
				time=0;
				Displapaintystate[paintxsta][paintysta]=recordpointcolor;
				if(paintxsta<15)
					paintxsta++;
				recordpointcolor=Displapaintystate[paintxsta][paintysta];
			}
			if(Keyscan_Left()==1)
			{
				time=0;
				Displapaintystate[paintxsta][paintysta]=recordpointcolor;
				if(paintxsta>0)
					paintxsta--;
				recordpointcolor=Displapaintystate[paintxsta][paintysta];
			}
		}
		if(Keyscan_Enter()==1)
		{
			Displapaintystate[paintxsta][paintysta]=recordpointcolor;
			if(Displapaintystate[paintxsta][paintysta]<=6)
				Displapaintystate[paintxsta][paintysta]++;
			else
				Displapaintystate[paintxsta][paintysta]=1;
			recordpointcolor=Displapaintystate[paintxsta][paintysta];
		}
		else if(Keyscan_Esc()==1)
		{
			Displapaintystate[paintxsta][paintysta]=recordpointcolor;
			Displapaintystate[paintxsta][paintysta]=0;
			recordpointcolor=Displapaintystate[paintxsta][paintysta];
		}
		else if(Keyscan_Select()==1)/*功能设置*/
		{
			/*进入选项卡 存档 退出*/
			Roll_Display_Left_sendB(clearscreen,7);
			selectstate=1;
			Roll_Color=(random1+random2+random3)%7+1;
			if(selectstate==1)
				for(x=0; x<16; x++)
					for(y=0; y<16; y++)
						Select_Array[x][y]=savewords[x][y];
			while(1)
			{
				if(Keyscan_Up()==1&&selectstate<1)
				{
					Roll_Display_Left_sendB(clearscreen,7);
					selectstate++;
					DisplayTimes=0;
					if(selectstate==1)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=savewords[x][y];

				}
				else if(Keyscan_Down()==1&&selectstate>0)
				{
					Roll_Display_Left_sendB(clearscreen,7);
					selectstate--;
					DisplayTimes=0;
					if(selectstate==0)
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=quitwords[x][y];
				}
				else if(Keyscan_Esc()==1)
				{
					time=0;
					selectstate=1;
					break;
				}
				else if(Keyscan_Enter()==1)
				{
					Roll_Display_Left_sendB(clearscreen,7);
					if(selectstate==0)
					{
						Roll_Display_Left_sendB(clearscreen,7);
						return;
					}
					else if(selectstate==1)
					{
						Roll_Color=(random1+random2+random3)%7+1;
						saveorder=0;
						for(x=0; x<16; x++)
							for(y=0; y<16; y++)
								Select_Array[x][y]=one[x][y];
						while(1)
						{
							if(Keyscan_Down()==1&&saveorder<2)
							{
								saveorder++;
								Roll_Display_Left_sendB(clearscreen,7);
								Roll_Color=(random1+random2+random3)%7+1;
								DisplayTimes=0;
								if(saveorder==0)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=one[x][y];
								else if(saveorder==1)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=two[x][y];
								else if(saveorder==2)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=three[x][y];

							}
							else if(Keyscan_Up()==1&&saveorder>0)
							{
								saveorder--;
								Roll_Display_Left_sendB(clearscreen,7);
								Roll_Color=(random1+random2+random3)%7+1;
								DisplayTimes=0;
								if(saveorder==0)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=one[x][y];
								else if(saveorder==1)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=two[x][y];
								else if(saveorder==2)
									for(x=0; x<16; x++)
										for(y=0; y<16; y++)
											Select_Array[x][y]=three[x][y];
							}
							else if(Keyscan_Esc()==1)
							{
								Roll_Display_Left_sendB(clearscreen,7);
								time=0;
								selectstate=0;
								break;
							}
							else if(Keyscan_Enter()==1)
							{
								if(saveorder==0)
								{
									for(y=0; y<8; y++)
										for(x=0; x<8; x++)
											SaveReadArray[y*8+x]=Displapaintystate[x][y];
									SPI_FLASH_SectorErase(0x002000);
									SPI_FLASH_BufferWrite(SaveReadArray,0x002000,256);
									break;
								}
								else if(saveorder==1)
								{
									for(y=0; y<8; y++)
										for(x=0; x<8; x++)
											SaveReadArray[y*8+x]=Displapaintystate[x][y];
									SPI_FLASH_SectorErase(0x003000);
									SPI_FLASH_BufferWrite(SaveReadArray,0x003000,256);
									break;
								}
								else if(saveorder==2)
								{
									for(y=0; y<8; y++)
										for(x=0; x<8; x++)
											SaveReadArray[y*8+x]=Displapaintystate[x][y];
									SPI_FLASH_SectorErase(0x004000);
									SPI_FLASH_BufferWrite(SaveReadArray,0x004000,256);
									break;
								}
							}
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
				if(time>=50)
				{
					DisplayTimes++;
					time=0;
					if(selectstate==1)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=savewords[x][y];
					else if(selectstate==0)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=quitwords[x][y];
				}
				if(DisplayTimes==28-16)   /*滚动显示到时间了*/
				{
					Roll_Display_Left_sendB(clearscreen,7);
					DisplayTimes=0;
					Roll_Color=(random1+random2+random3)%7+1;
					time=0;
					if(selectstate==1)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=savewords[x][y];
					else if(selectstate==0)
						for(x=0; x<16; x++)
							for(y=DisplayTimes; y<16+DisplayTimes; y++)
								Select_Array[x][y-DisplayTimes]=quitwords[x][y];
				}
				/*显示文字*/
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
		/*闪烁显示*/
		{
			if(time<=300)
			{
				Displapaintystate[paintxsta][paintysta]=recordpointcolor;
				dir=FourDirection();
				if(dir==2)
					FlappyBird_Right_Roll_Display_sendB (Displapaintystate);/*xia*/
				else if(dir==3)
					FlappyBird_Down_Roll_Display_sendB  (Displapaintystate);/*zuo*/
				else if(dir==4)
					FlappyBird_Up_Roll_Display_sendB    (Displapaintystate);/*you*/
				else if(dir==1)
					FlappyBird_Left_Roll_Display_sendB  (Displapaintystate);/*shang*/
			}
			else if(time>300&&time<=600)
			{
				Displapaintystate[paintxsta][paintysta]=0;
				dir=FourDirection();
				if(dir==2)
					FlappyBird_Right_Roll_Display_sendB (Displapaintystate);/*xia*/
				else if(dir==3)
					FlappyBird_Down_Roll_Display_sendB  (Displapaintystate);/*zuo*/
				else if(dir==4)
					FlappyBird_Up_Roll_Display_sendB    (Displapaintystate);/*you*/
				else if(dir==1)
					FlappyBird_Left_Roll_Display_sendB  (Displapaintystate);/*shang*/
			}
			else if(time>600)
				time=0;
		}
	}
}
void snake(unsigned int snakelevel)/*150 300*/
{
	unsigned char SnakeArray[2][16][16]= {0}; /*上层颜色 下层顺序*/
	unsigned char x,y,i;
	unsigned char SnakeDirection=3;/*上1下2左3右4*/
	unsigned char snakelength,xnow,ynow,xran,yran,deathflag;
	unsigned char xrandom,yrandom;
	/*初始化蛇和第一个食物*/
	{
		for(x=0; x<16; x++)
			for(y=0; y<16; y++)
			{
				SnakeArray[0][x][y]=0;
				SnakeArray[1][x][y]=0;
			}
		SnakeArray[0][8][8]=random1%7+1;
		SnakeArray[1][8][8]=1;
		SnakeArray[0][8][9]=random3%7+1;
		SnakeArray[1][8][9]=2;
		while(1)
		{
			xran++;
			yran++;
			xrandom=(xrandom+Gtime+random1+random3+random7+random4+xran+5)%16;
			yrandom=(yrandom+random1+random3+random7+random5+yran+5)%16;
			if(SnakeArray[0][xrandom][yrandom]==0)
			{
				SnakeArray[0][xrandom][yrandom]=(random1+random3+random7)%7+1;
				SnakeArray[1][xrandom][yrandom]=250;
				break;
			}
		}
	}
	/*数字大为头*/
	deathflag=0;
	xnow=8;
	ynow=9;
	snakelength=2;
	time=0;
	while(1)
	{
		/*蛇前进一格*/
		if(time>=snakelevel-snakelength*2)
		{
			BEEP_OFF;
			time=0;
			/*找头*/
			for(x=0; x<16; x++)
				for(y=0; y<16; y++)
					if(snakelength==SnakeArray[1][x][y])
					{
						xnow=x;
						ynow=y;
						break;
					}
			if(SnakeDirection==3&&xnow>0&&SnakeArray[1][xnow-1][ynow]==250)
			{
				BEEP_ON;
				snakelength++;
				SnakeArray[1][xnow-1][ynow]=snakelength;
				xnow=xnow-1;
				/*产生新食物*/
				while(1)
				{
					xran++;
					yran++;
					xrandom=(xrandom+Gtime+random1+random3+random7+random4+xran+5)%16;
					yrandom=(yrandom+random1+random3+random7+random5+yran+5)%16;
					if(SnakeArray[0][xrandom][yrandom]==0)
					{
						SnakeArray[0][xrandom][yrandom]=(random1+random3+random7)%7+1;
						SnakeArray[1][xrandom][yrandom]=250;
						break;
					}
				}

			}
			else if(SnakeDirection==2&&ynow>0&&SnakeArray[1][xnow][ynow-1]==250)
			{
				BEEP_ON;
				snakelength++;
				SnakeArray[1][xnow][ynow-1]=snakelength;
				ynow=ynow-1;
				/*产生新食物*/
				while(1)
				{
					xran++;
					yran++;
					xrandom=(xrandom+Gtime+random1+random3+random7+random4+xran+5)%16;
					yrandom=(yrandom+random1+random3+random7+random5+yran+5)%16;
					if(SnakeArray[0][xrandom][yrandom]==0)
					{
						SnakeArray[0][xrandom][yrandom]=(random1+random3+random7)%7+1;
						SnakeArray[1][xrandom][yrandom]=250;
						break;
					}
				}

			}
			else if(SnakeDirection==1&&ynow<15&&SnakeArray[1][xnow][ynow+1]==250)
			{
				BEEP_ON;
				snakelength++;
				SnakeArray[1][xnow][ynow+1]=snakelength;
				ynow=ynow+1;
				/*产生新食物*/
				while(1)
				{
					xran++;
					yran++;
					xrandom=(xrandom+Gtime+random1+random3+random7+random4+xran+5)%16;
					yrandom=(yrandom+random1+random3+random7+random5+yran+5)%16;
					if(SnakeArray[0][xrandom][yrandom]==0)
					{
						SnakeArray[0][xrandom][yrandom]=(random1+random3+random7)%7+1;
						SnakeArray[1][xrandom][yrandom]=250;
						break;
					}
				}

			}
			else if(SnakeDirection==4&&xnow<15&&SnakeArray[1][xnow+1][ynow]==250)
			{
				BEEP_ON;
				snakelength++;
				SnakeArray[1][xnow+1][ynow]=snakelength;
				xnow=xnow+1;
				/*产生新食物*/
				while(1)
				{
					xran++;
					yran++;
					xrandom=(xrandom+Gtime+random1+random3+random7+random4+xran+5)%16;
					yrandom=(yrandom+random1+random3+random7+random5+yran+5)%16;
					if(SnakeArray[0][xrandom][yrandom]==0)
					{
						SnakeArray[0][xrandom][yrandom]=(random1+random3+random7)%7+1;
						SnakeArray[1][xrandom][yrandom]=250;
						break;
					}
				}

			}
			else
			{
				/*蛇移动*/
				for(i=snakelength; i>0; i--)
				{
					/*头运动并且死亡判断*/
					if(i==snakelength)
					{
						if(SnakeDirection==1)
						{
							if( (ynow<15&&SnakeArray[1][xnow][ynow+1]!=0&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=1&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=250) || ynow+1>=16 )
							{
								Roll_Display_Left_sendB(clearscreen,7);
								deathflag=1;
								break;
							}
							else
							{
								SnakeArray[1][xnow][ynow+1]=SnakeArray[1][xnow][ynow];
								SnakeArray[0][xnow][ynow+1]=SnakeArray[0][xnow][ynow];
							}
						}
						else if(SnakeDirection==2)
						{
							if( (ynow>0&&SnakeArray[1][xnow][ynow-1]!=0&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=1&&ynow>0&&SnakeArray[1][xnow][ynow-1]!=250) || ynow-1<0 )
							{
								Roll_Display_Left_sendB(clearscreen,7);
								deathflag=1;
								break;
							}
							else
							{
								SnakeArray[1][xnow][ynow-1]=SnakeArray[1][xnow][ynow];
								SnakeArray[0][xnow][ynow-1]=SnakeArray[0][xnow][ynow];
							}
						}
						else if(SnakeDirection==3)
						{
							if( (xnow>0&&SnakeArray[1][xnow-1][ynow]!=0&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=1&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=250) || xnow-1<0)
							{
								Roll_Display_Left_sendB(clearscreen,7);
								deathflag=1;
								break;
							}
							else
							{
								SnakeArray[1][xnow-1][ynow]=SnakeArray[1][xnow][ynow];
								SnakeArray[0][xnow-1][ynow]=SnakeArray[0][xnow][ynow];
							}
						}
						else if(SnakeDirection==4)
						{
							if( (xnow<15&&SnakeArray[1][xnow+1][ynow]!=0&&ynow<15&&SnakeArray[1][xnow][ynow+1]!=1&&xnow<15&&SnakeArray[1][xnow+1][ynow]!=250) || xnow+1>=16 )
							{
								Roll_Display_Left_sendB(clearscreen,7);
								deathflag=1;
								break;
							}
							else
							{
								SnakeArray[1][xnow+1][ynow]=SnakeArray[1][xnow][ynow];
								SnakeArray[0][xnow+1][ynow]=SnakeArray[0][xnow][ynow];
							}
						}
					}
					/*四向寻找*/
					else if(i>1)
					{
						if(ynow<15&&SnakeArray[1][xnow][ynow+1]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow][ynow+1];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow][ynow+1];
							ynow=ynow+1;
						}
						else if(xnow<15&&SnakeArray[1][xnow+1][ynow]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow+1][ynow];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow+1][ynow];
							xnow=xnow+1;
						}
						else if(xnow>0&&SnakeArray[1][xnow-1][ynow]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow-1][ynow];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow-1][ynow];
							xnow=xnow-1;
						}
						else if(ynow>0&&SnakeArray[1][xnow][ynow-1]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow][ynow-1];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow][ynow-1];
							ynow=ynow-1;
						}
					}
					/*尾运动*/
					else if(i==1)
					{
						/*四向寻找*/
						if(ynow<15&&SnakeArray[1][xnow][ynow+1]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow][ynow+1];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow][ynow+1];
							SnakeArray[1][xnow][ynow+1]=0;
							SnakeArray[0][xnow][ynow+1]=0;
							ynow=ynow+1;
						}
						else if(xnow<15&&SnakeArray[1][xnow+1][ynow]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow+1][ynow];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow+1][ynow];
							SnakeArray[1][xnow+1][ynow]=0;
							SnakeArray[0][xnow+1][ynow]=0;
							xnow=xnow+1;
						}
						else if(xnow>0&&SnakeArray[1][xnow-1][ynow]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow-1][ynow];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow-1][ynow];
							SnakeArray[1][xnow-1][ynow]=0;
							SnakeArray[0][xnow-1][ynow]=0;
							xnow=xnow-1;
						}
						else if(ynow>0&&SnakeArray[1][xnow][ynow-1]==i)
						{
							SnakeArray[1][xnow][ynow]=SnakeArray[1][xnow][ynow-1];
							SnakeArray[0][xnow][ynow]=SnakeArray[0][xnow][ynow-1];
							SnakeArray[1][xnow][ynow-1]=0;
							SnakeArray[0][xnow][ynow-1]=0;
							ynow=ynow-1;
						}

					}
				}
			}
		}
		dir=FourDirection();
		{
			if(dir==2)
				FlappyBird_Right_Roll_Display_sendB (SnakeArray[0]);/*xia*/
			else if(dir==3)
				FlappyBird_Down_Roll_Display_sendB  (SnakeArray[0]);/*zuo*/
			else if(dir==4)
				FlappyBird_Up_Roll_Display_sendB    (SnakeArray[0]);/*you*/
			else if(dir==1)
				FlappyBird_Left_Roll_Display_sendB  (SnakeArray[0]);/*shang*/
		}
		/*方向输入*/
		{
			if(Snake_Down()==1&&SnakeDirection!=1)
			{
				SnakeDirection=2;
			}
			else if(Snake_Up()==1&&SnakeDirection!=2)
			{
				SnakeDirection=1;
			}
			if(Snake_Right()==1&&SnakeDirection!=3)
			{
				SnakeDirection=4;
			}
			if(Snake_Left()==1&&SnakeDirection!=4)
			{
				SnakeDirection=3;
			}
		}
		if(Keyscan_Select()==1)//暂停
		{
			while(Keyscan_Select()==0)
			{
				time=0;
			}
			while(Keyscan_Select()==1)
			{
				time=0;
			}
		}
		if(Keyscan_Esc()==1)
			break;
		DisplayScore(snakelength-2);
		if(deathflag==1)
			break;
	}
	Roll_Display_Left_sendB(clearscreen,7);
	if(snakelevel==300)
		dead(snakelength-2,5);
	else if(snakelevel==150)
		dead(snakelength-2,6);
}
