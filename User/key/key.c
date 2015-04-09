#include "key.h"
extern u8 random1;
extern u8 random2;
extern u8 random3;
extern u8 random4;
extern u8 random5;
extern u8 random6;
extern u8 random7;
extern u8 dir;
/******************************************************************************
**********************************����ģ��*************************************
*******************************************************************************/
/*���س�ʼ��*/
void KEY_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����KEY������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC , ENABLE); 			   
	/*ѡ��Ҫ����GPIOA������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
	/* ���� GPIOA0 Ϊ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	/*���ÿ⺯������ʼ��GPIOA*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*ѡ��Ҫ����GPIOA������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8;
	/* ���� GPIOA0 Ϊ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	/*���ÿ⺯������ʼ��GPIOA*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*ѡ��Ҫ����GPIOA������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11;
	/* ���� GPIOA0 Ϊ�������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	/*���ÿ⺯������ʼ��GPIOA*/
	GPIO_Init(GPIOC, &GPIO_InitStructure);			
}

/*����������Ӧ*/
unsigned char FourDirection(void)
{
	int i;
	int a, b, c, d;
	a = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	b = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10);
	c = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
	d = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
	for(i=500 ; i>0; i--);
	if(a == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) &&
	        b == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) &&
	        c == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) &&
	        d == GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8))
		if (a == 0 && b == 0 && c == 1&& d == 1)
		{
			dir = 1;
		}
		else if (a == 1 && b == 1 && c == 0 &&d == 0)
		{
			dir = 2;
		}
		else if (a == 1 && b == 0 && c == 1&& d == 0)
		{
			dir = 3;
		}
		else if (a == 0 && b == 1 && c == 0&& d == 1)
		{
			dir = 4;
		}
	return dir;
}

/*�߸��ٿؼ�*/
unsigned char Keyscan_Down  (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0)
					{
						random2++;
					}
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Up    (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
					{
						random3++;
					}
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Right (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
					{
						random4++;
					}
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Left  (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 0)
					{
						random5++;
					}
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Esc (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == 0)
					{
						random6++;
					}
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Enter   (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0);
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_Select(void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11) == 0)
				{
					while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11) == 0);
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_B (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) == 0)
				{
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Keyscan_A   (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
		{
			for(i=60000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2) == 0)
				{
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Snake_Down  (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0)
		{
			for(i=6000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 0)
				{
						random2++;
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Snake_Up    (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
		{
			for(i=6000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0)
				{
						random3++;
					return 1;

				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Snake_Right (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
		{
			for(i=6000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
				{
						random4++;
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
unsigned char Snake_Left  (void)
{
	int i;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 0)
		{
			for(i=6000;i>0;i--);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == 0)
				{
					random5++;
					return 1;
				}
			else 
				{
					return 0;
				}		
		}	 
		return 0;	
}
