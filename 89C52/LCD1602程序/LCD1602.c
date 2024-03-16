#include "LCD1602.H"
#include <intrins.H>
#include "delay.h"

//unsigned char code table[][8]={0x07,0x08,0x1c,0x08,0x1c,0x08,0x07,0x00,
//															 0x04,0x04,0x1F,0x04,0x04,0x04,0x04,0x00};

/**
  * @brief  �η�����
  * @retval ����ֵ����X��Y�η�
  */
unsigned int Pow(unsigned int X, unsigned int Y)
{
	unsigned int Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void check_busy(void)
{ 
  do
  {
     DATA = 0xff;
     E = 0;
     RS = 0; 
     RW = 1;
     E = 1;
     _nop_();
  } while(DATA & 0x80);
  E = 0;
 }

 void lcd_writecmd(unsigned char cmd)		//дָ��
{
	check_busy();
	RS=0;
	RW=0;
	E=0;
	DATA=cmd;
	E = 1;
  _nop_();
  E = 0;
}

void lcd_writedata(unsigned char dat)	//д����
{
	check_busy();
  E = 0;
  RS = 1;
  RW = 0;
  DATA = dat;
  E = 1;
  _nop_();
  E = 0;
}

void lcd1602_LeftMove(void)
{
	lcd_writecmd(0x18);	
}
void lcd1602_RightMove(void)
{
	lcd_writecmd(0x1c);	
}

void LCD1602_clean(void)	//����
{
	lcd_writecmd(clean_lcd1602);
}

void LCD1602_Init(void)
{
	lcd_writecmd(0x3c);
	lcd_writecmd(0x0C); // no cursor, no blink, enable display
	lcd_writecmd(0x06); // auto-increment on
	lcd_writecmd(0x01); // clear screen
	LCD_BLK=1;       //��������
}

void LCD1602_set_pos(unsigned char x,unsigned char y)
{
	if(y>1||x>15)return;//���в���������ǿ���˳�
	
	if(y<1)	//��1����ʾ
	{	
		lcd_writecmd(0x80+x);//��һ����ʾ��ַ����	
	}
	else 
	{
		lcd_writecmd(0x80+0x40+x);	//�ڶ�����ʾ��ַ����	
	}

}
void LCD1602_show_char(unsigned char x,unsigned char y,unsigned char zi)	//��ʾһ���ַ�
{
	LCD1602_set_pos(x,y);	
	lcd_writedata(zi);//��ʾ����	
}

void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str)//��ʾ�ַ���
{
	unsigned char i=0;

	if(y>1||x>15)return;//���в���������ǿ���˳�

	if(y<1)	//��1����ʾ
	{	
		while(*str!='\0')//�ַ�������'\0'��β��ֻҪǰ�������ݾ���ʾ
		{
			if(i<16-x)//����ַ����ȳ�����һ����ʾ��Χ�����ڵڶ��м�����ʾ
			{
				lcd_writecmd(0x80+i+x);//��һ����ʾ��ַ����	
			}
			else
			{
				lcd_writecmd(0x40+0x80+i+x-16);//�ڶ�����ʾ��ַ����	
			}
			lcd_writedata(*str);//��ʾ����
			str++;//ָ�����
			i++;	
		}	
	}
	else	//��2����ʾ
	{
		while(*str!='\0')
		{
			if(i<16-x) //����ַ����ȳ����ڶ�����ʾ��Χ�����ڵ�һ�м�����ʾ
			{
				lcd_writecmd(0x80+0x40+i+x);	
			}
			else
			{
				lcd_writecmd(0x80+i+x-16);	
			}
			lcd_writedata(*str);
			str++;
			i++;	
		}	
	}				
}

void LCD1602_show_num(unsigned char x,unsigned char y,unsigned int Number,unsigned char Length)		//��ʾ����
{
	unsigned char i;
	for (i = 0; i < Length; i++)							
	{
		LCD1602_show_char(x + i, y, Number / Pow(10, Length - i - 1) % 10 + '0');
	}
}

//void LCD1602_pic(unsigned char x,unsigned char y,unsigned char CGRAM,unsigned char count,unsigned char* pic)
//{
//int i=0;
//lcd_writecmd(CGRAM);
//for(i=0;i<8;i++)
//{
//	lcd_writedata(pic[i]);
//	delay_ms(10);
//}
//LCD1602_set_pos(x,y);
//lcd_writedata(count);
//}
