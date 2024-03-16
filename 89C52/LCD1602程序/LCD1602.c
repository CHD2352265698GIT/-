#include "LCD1602.H"
#include <intrins.H>
#include "delay.h"

//unsigned char code table[][8]={0x07,0x08,0x1c,0x08,0x1c,0x08,0x07,0x00,
//															 0x04,0x04,0x1F,0x04,0x04,0x04,0x04,0x00};

/**
  * @brief  次方函数
  * @retval 返回值等于X的Y次方
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

 void lcd_writecmd(unsigned char cmd)		//写指令
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

void lcd_writedata(unsigned char dat)	//写数据
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

void LCD1602_clean(void)	//清屏
{
	lcd_writecmd(clean_lcd1602);
}

void LCD1602_Init(void)
{
	lcd_writecmd(0x3c);
	lcd_writecmd(0x0C); // no cursor, no blink, enable display
	lcd_writecmd(0x06); // auto-increment on
	lcd_writecmd(0x01); // clear screen
	LCD_BLK=1;       //开启背光
}

void LCD1602_set_pos(unsigned char x,unsigned char y)
{
	if(y>1||x>15)return;//行列参数不对则强制退出
	
	if(y<1)	//第1行显示
	{	
		lcd_writecmd(0x80+x);//第一行显示地址设置	
	}
	else 
	{
		lcd_writecmd(0x80+0x40+x);	//第二行显示地址设置	
	}

}
void LCD1602_show_char(unsigned char x,unsigned char y,unsigned char zi)	//显示一个字符
{
	LCD1602_set_pos(x,y);	
	lcd_writedata(zi);//显示内容	
}

void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str)//显示字符串
{
	unsigned char i=0;

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		while(*str!='\0')//字符串是以'\0'结尾，只要前面有内容就显示
		{
			if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
			{
				lcd_writecmd(0x80+i+x);//第一行显示地址设置	
			}
			else
			{
				lcd_writecmd(0x40+0x80+i+x-16);//第二行显示地址设置	
			}
			lcd_writedata(*str);//显示内容
			str++;//指针递增
			i++;	
		}	
	}
	else	//第2行显示
	{
		while(*str!='\0')
		{
			if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
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

void LCD1602_show_num(unsigned char x,unsigned char y,unsigned int Number,unsigned char Length)		//显示数字
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
