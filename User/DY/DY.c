/******************����ģ��******************/
#include "DY.h"
#include "bsp_spi_flash.h"
#include "key.h"
#include "OP.h"
/******************����ģ��******************/
extern int time;
extern char dir;
void OPV(void)
{
	int OPDisplayTimes=0;

	GPIO_ResetBits(GPIOE, GPIO_Pin_6);
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_6);
	while(OPDisplayTimes<637)
	{

		if(time>60)
		{
			time=0;
			OPDisplayTimes++;
			dir=FourDirection();
		}
		if(Keyscan_Esc()==1)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_0);
			while(time<300);
			time=0;
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
			break;
		}
		/*������ʾ*/
		{

			if(dir==1)
				Down_sendscreen_16(OP[OPDisplayTimes]);
			else if(dir==3)
				Left_sendscreen_16(OP[OPDisplayTimes]);
			else if(dir==2)
				Up_sendscreen_16(OP[OPDisplayTimes]);
			else if(dir==4)
				Right_sendscreen_16(OP[OPDisplayTimes]);
		}
	}
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	while(time<300);
	time=0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
}
void DY_GPIO_Config(void)
{
	/*DBP_CL  = c4
	  DBP_DA  = a7
	  DBP_INT = a9 */
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_4;
	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void DA(char da_wei)
{
	if(da_wei)
	{
		DBP_DA_OFF;
	}
	else
	{
		DBP_DA_ON;
	}
}
void DBP_sendscreen_16(void)
{
	/*DBP_CL = D15;
	  DBP_DA = D4;
	  DBP_INT = D11;*/
//	unsigned char abc;
	DBP_delay(1);
	delayms(10);	//�ڷ��Ͷ�������ʱһ��Ҫ�������ʱ����Ȼ�ڶ����������������ź�
	DBP_INT_ON;
	/*
	DBP_CL = D15;
	DBP_DA = D4;
	DBP_INT = D11;*/
	DBP_delay(12);	//���鷢�֣��������ʱ�Ǻ��б�Ҫ�ģ���24�����Ժܱ��ա����Ը�������ʵ�����	//��һ��ҲҪ
	/*	for(abc=0;abc<=9;abc++)
		{
			DBP_sendB(Pic[abc]);
			DBP_delay(1);	//������һ��ż�����������֮��Ͳ��������
		}		*/
	DBP_INT_OFF ;
}
void Up_sendscreen_16  (const unsigned char Pic[4][128]) /*16ɫ������ʾ����*/
{
	int j,i;
	DBP_delay(1);
	/*��һ��Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);	/*���鷢�֣��������ʱ�Ǻ��б�Ҫ�ģ���24�����Ժܱ��ա����Ը�������ʵ�����*/
	DBP_sendB(0x02);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=14-j; i<=126-j; i=i+16)
		{
			DBP_sendB(Pic[1][i]);
			DBP_delay(12);
			DBP_sendB(Pic[1][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*�ڶ���Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x04);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=14-j; i<=126-j; i=i+16)
		{
			DBP_sendB(Pic[3][i]);
			DBP_delay(12);
			DBP_sendB(Pic[3][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x06);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=14-j; i<=126-j; i=i+16)
		{
			DBP_sendB(Pic[0][i]);
			DBP_delay(12);
			DBP_sendB(Pic[0][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x08);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=14-j; i<=126-j; i=i+16)
		{
			DBP_sendB(Pic[2][i]);
			DBP_delay(12);
			DBP_sendB(Pic[2][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
}
void Down_sendscreen_16(const unsigned char Pic[4][128]) /*16ɫ������ʾ����*/
{
	int j,i;
	DBP_delay(1);
	/*��һ��Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);	/*���鷢�֣��������ʱ�Ǻ��б�Ҫ�ģ���24�����Ժܱ��ա����Ը�������ʵ�����*/
	DBP_sendB(0x02);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=112+j; i>=0+j; i=i-16)
		{
			DBP_sendB(Pic[2][i]);
			DBP_delay(12);
			DBP_sendB(Pic[2][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*�ڶ���Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x04);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=112+j; i>=0+j; i=i-16)
		{
			DBP_sendB(Pic[0][i]);
			DBP_delay(12);
			DBP_sendB(Pic[0][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x06);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=112+j; i>=0+j; i=i-16)
		{
			DBP_sendB(Pic[3][i]);
			DBP_delay(12);
			DBP_sendB(Pic[3][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x08);
	delayms(10);
	for(j=0; j<=14; j=j+2)
		for(i=112+j; i>=0+j; i=i-16)
		{
			DBP_sendB(Pic[1][i]);
			DBP_delay(12);
			DBP_sendB(Pic[1][i+1]);
			DBP_delay(12);
		}
	DBP_INT_OFF;
}
void Left_sendscreen_16 (const unsigned char Pic[4][128]) /*16ɫ������ʾ����*/
{
	int i;
	DBP_delay(1);
	/*��һ��Ļ*/
	delayms(10);	/*�ڷ��Ͷ�������ʱһ��Ҫ�������ʱ����Ȼ�ڶ����������������ź�*/
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);	/*���鷢�֣��������ʱ�Ǻ��б�Ҫ�ģ���24�����Ժܱ��ա����Ը�������ʵ�����*/
	DBP_sendB(0x02);
	delayms(10);
	for(i=0; i<128; i++)
	{
		DBP_sendB(Pic[0][i]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*�ڶ���Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x04);
	delayms(10);
	for(i=0; i<128; i++)
	{
		DBP_sendB(Pic[1][i]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x06);
	delayms(10);
	for(i=0; i<128; i++)
	{
		DBP_sendB(Pic[2][i]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x08);
	delayms(10);
	for(i=0; i<128; i++)
	{
		DBP_sendB(Pic[3][i]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
}
void Right_sendscreen_16(const unsigned char Pic[4][128]) /*16ɫ������ʾ����*/
{
	int i;
	DBP_delay(1);
	/*��һ��Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);	/*���鷢�֣��������ʱ�Ǻ��б�Ҫ�ģ���24�����Ժܱ��ա����Ը�������ʵ�����*/
	DBP_sendB(0x02);
	delayms(10);
	for(i=126; i>=0; i=i-2)
	{
		DBP_sendB(Pic[3][i]);
		DBP_delay(12);
		DBP_sendB(Pic[3][i+1]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*�ڶ���Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x04);
	delayms(10);
	for(i=126; i>=0; i=i-2)
	{
		DBP_sendB(Pic[2][i]);
		DBP_delay(12);
		DBP_sendB(Pic[2][i+1]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x06);
	delayms(10);
	for(i=126; i>=0; i=i-2)
	{
		DBP_sendB(Pic[1][i]);
		DBP_delay(12);
		DBP_sendB(Pic[1][i+1]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
	/*������Ļ*/
	delayms(10);
	DBP_INT_ON;
	delayms(10);
	DBP_delay(12);
	DBP_sendB(0x08);
	delayms(10);
	for(i=126; i>=0; i=i-2)
	{
		DBP_sendB(Pic[0][i]);
		DBP_delay(12);
		DBP_sendB(Pic[0][i+1]);
		DBP_delay(12);
	}
	DBP_INT_OFF;
}
void Roll_Display_Left_sendB  (unsigned char Display_State[16][16],unsigned char Word_Color)  /*��ɫ���Ϲ�����ʾ����*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=0; y<=7; y++)
		for(x=7; x>=0; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=8; y<=15; y++)
		for(x=7; x>=0; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=0; y<=7; y++)
		for(x=15; x>=8; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	for(y=8; y<=15; y++)
		for(x=15; x>=8; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
void Roll_Display_Down_sendB (unsigned char Display_State[16][16],unsigned char Word_Color)  /*��ɫ���¹�����ʾ����*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=0; x<=8; x++)
		for(y=8; y<=15; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=8; x<=15; x++)
		for(y=8; y<=15; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=0; x<=7; x++)
		for(y=0; y<=7; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	for(x=8; x<=15; x++)
		for(y=0; y<=7; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
}
void Roll_Display_Up_sendB (unsigned char Display_State[16][16],unsigned char Word_Color)  /*��ɫ���������ʾ����*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=15; x>=8; x--)
		for(y=7; y>=0; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=7; x>=0; x--)
		for(y=7; y>=0; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(x=15; x>=8; x--)
		for(y=15; y>=8; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	for(x=7; x>=0; x--)
		for(y=15; y>=8; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
void Roll_Display_Right_sendB(unsigned char Display_State[16][16],unsigned char Word_Color)  /*��ɫ���ҹ�����ʾ����*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=15; y>=8; y--)
		for(x=8; x<=15; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}

	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=7; y>=0; y--)
		for(x=8; x<=15; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	DBP_delay(30);
	for(y=15; y>=8; y--)
		for(x=0; x<=7; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+0xF8);
	for(y=7; y>=0; y--)
		for(x=0; x<=7; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
void FlappyBird_Left_Roll_Display_sendB (unsigned char Display_State[16][16])   /*��ɫ������ʾ������*/
{
	signed char x,y,i;
	for(i=1; i<=4; i=i*2)
	{
		/*��һ��*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x03);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=0; y<=7; y++)
			for(x=7; x>=0; x--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*�ڶ���*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x05);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=8; y<=15; y++)
			for(x=7; x>=0; x--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x07);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=0; y<=7; y++)
			for(x=15; x>=8; x--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x09);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=8; y<=15; y++)
			for(x=15; x>=8; x--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
	}
}
void FlappyBird_Right_Roll_Display_sendB (unsigned char Display_State[16][16])  /*��ɫ������ʾ������*/
{
	signed char x,y,i;
	for(i=1; i<=4; i=i*2)
	{
		/*��һ��*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x03);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=15; y>=8; y--)
			for(x=8; x<=15; x++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*�ڶ���*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x05);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=7; y>=0; y--)
			for(x=8; x<=15; x++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x07);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=15; y>=8; y--)
			for(x=0; x<=7; x++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x09);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(y=7; y>=0; y--)
			for(x=0; x<=7; x++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
	}
}
void FlappyBird_Up_Roll_Display_sendB (unsigned char Display_State[16][16])     /*��ɫ������ʾ������*/
{
	signed char x,y,i;
	for(i=1; i<=4; i=i*2)
	{
		/*��һ��*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x03);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=15; x>=8; x--)
			for(y=7; y>=0; y--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*�ڶ���*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x05);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=7; x>=0; x--)
			for(y=7; y>=0; y--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x07);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=15; x>=8; x--)
			for(y=15; y>=8; y--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x09);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=7; x>=0; x--)
			for(y=15; y>=8; y--)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
	}
}
void FlappyBird_Down_Roll_Display_sendB (unsigned char Display_State[16][16])   /*��ɫ������ʾ������*/
{
	signed char x,y,i;
	for(i=1; i<=4; i=i*2)
	{
		/*��һ��*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x03);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=0; x<=8; x++)
			for(y=8; y<=15; y++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*�ڶ���*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x05);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=8; x<=15; x++)
			for(y=8; y<=15; y++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x07);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=0; x<=7; x++)
			for(y=0; y<=7; y++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
		/*������*/
		DBP_delay(50);
		delayms(100);
		DBP_INT_ON;
		delayms(20);
		DBP_delay(50);
		DBP_sendB(0x09);
		DBP_delay(50);
		DBP_sendB(i+0xF8);
		DBP_delay(50);
		for(x=8; x<=15; x++)
			for(y=0; y<=7; y++)
			{
				DA(Display_State[x][y]&i);
				DBP_delay(10);
				DBP_CL_ON;
				DBP_delay(10);
				DBP_CL_OFF;
				DBP_delay(10);
			}
		DBP_INT_OFF;
	}
}
void delayms (unsigned int a)
{
	unsigned int i;
	while( a-- != 0)
	{
		for(i = 0; i < 600; i++);
	}
}
void DBP_delay(unsigned char n)
{
	for(; n>0; n--)
	{
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	}
}
void DBP_sendB(unsigned char b)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		DA(b&0x80); //ȡ�����λ
		DBP_delay(1);
		DBP_CL_ON;
		DBP_delay(1);
		DBP_CL_OFF;
		DBP_delay(1);
		b<<=1; //����
	}
}
void LightChange_Roll_Display_Left_sendB  (unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light)  /*��ɫ���Ϲ�����ʾ�������ȱ仯*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=0; y<=7; y++)
		for(x=7; x>=0; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=8; y<=15; y++)
		for(x=7; x>=0; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=0; y<=7; y++)
		for(x=15; x>=8; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	for(y=8; y<=15; y++)
		for(x=15; x>=8; x--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
void LightChange_Roll_Display_Down_sendB (unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light)  /*��ɫ���¹�����ʾ�������ȱ仯*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=0; x<=8; x++)
		for(y=8; y<=15; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=8; x<=15; x++)
		for(y=8; y<=15; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=0; x<=7; x++)
		for(y=0; y<=7; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	for(x=8; x<=15; x++)
		for(y=0; y<=7; y++)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
}
void LightChange_Roll_Display_Up_sendB (unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light)  /*��ɫ���������ʾ�������ȱ仯*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=15; x>=8; x--)
		for(y=7; y>=0; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=7; x>=0; x--)
		for(y=7; y>=0; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(x=15; x>=8; x--)
		for(y=15; y>=8; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	for(x=7; x>=0; x--)
		for(y=15; y>=8; y--)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
void LightChange_Roll_Display_Right_sendB(unsigned char Display_State[16][16],unsigned char Word_Color,unsigned char light)  /*��ɫ���ҹ�����ʾ�������ȱ仯*/
{
	signed char x,y;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x03);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=15; y>=8; y--)
		for(x=8; x<=15; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}

	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x05);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=7; y>=0; y--)
		for(x=8; x<=15; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(1);
			DBP_CL_ON;
			DBP_delay(1);
			DBP_CL_OFF;
			DBP_delay(1);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x07);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	DBP_delay(30);
	for(y=15; y>=8; y--)
		for(x=0; x<=7; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
	DBP_delay(30);
	delayms(130);
	DBP_INT_ON;
	delayms(30);
	DBP_delay(30);
	DBP_sendB(0x09);
	DBP_delay(30);
	DBP_sendB(Word_Color+light*8);
	for(y=7; y>=0; y--)
		for(x=0; x<=7; x++)
		{
			DA(Display_State[x][y]);
			DBP_delay(10);
			DBP_CL_ON;
			DBP_delay(10);
			DBP_CL_OFF;
			DBP_delay(10);
		}
	DBP_INT_OFF;
}
