#include "LCD12864.H"
#include "DELAY.H"
#include "string.h"
#include "stdio.h"


void LCD12864_WriteData(uint8_t Data)
{
  	LCD_EN=0;
		LCD_DI=1;
		LCD_RW=0;
		Delay_10us(10);
		LCD_DATA =  Data;
	  LCD_EN=1;
		Delay_10us(20);
}

void LCD12864_WriteCmd(uint8_t CMD)
{
  	LCD_EN=0;
		LCD_DI=0;
		LCD_RW=0;
	  Delay_10us(10);
		LCD_DATA =  CMD;
  	LCD_EN=1;
	  Delay_10us(20);
}
/*-----------------------------------------------------------------------
LCD12864_ReadData   	    			: LCD12864读数据
	
输入参数                   			：无
                                  
-----------------------------------------------------------------------*/
uint8_t LCD12864_ReadData()
{
	return 0;
}
void LCD12864_Init()
{
	//首先对LCD进行复位
//	LCD_REST = 0;
//	Delay_ms(30);
//	LCD_REST = 1;
//	Delay_ms(20);

	/* 开始初始化的步骤 */
	  LCD12864_WriteCmd(0x30);                                                             // 选择基本指令集，和，8位数据模式。
    Delay_ms(5);
    LCD12864_WriteCmd(0x0c);                                                             //开显示,无游标,不反白.
    Delay_ms(5); 
    LCD12864_WriteCmd(0x01);                                                             //清除显示，并将 DDRAM 的地址计数器 AC 设为 00H.
    Delay_ms(5);
    LCD12864_WriteCmd(0x06);                                                             //设置，外部读写数据后,地址记数器 AC 会自动加 1。
    Delay_ms(5);
    LCD12864_WriteCmd(0x80);                                                             //将 DDRAM 地址计数器 AC 设为 0.
    Delay_ms(5);	
}

/*-----------------------------------------------------------------------
LCD12864_SetLocal    	    	    : LCD12864设置坐标
	
输入参数                   			：无
                                  
编写日期                        	：2019年1月29日
最后修改日期                  		：2019年1月29日
-----------------------------------------------------------------------*/
void LCD12864_SetLocal(uint8_t X,uint8_t Y)
{
	switch(X)
	{
		case 0: LCD12864_WriteCmd(0x80+Y); break;
		case 1: LCD12864_WriteCmd(0x90+Y); break;
		case 2: LCD12864_WriteCmd(0x88+Y); break;
		case 3: LCD12864_WriteCmd(0x98+Y); break;
	}
}
/*-----------------------------------------------------------------------
LCD12864_DisplayStrChinSingle   : LCD12864显示单个字符串和汉字
	
输入参数                   			：uint8_t X ：X轴坐标,uint8_t Y ：Y轴坐标,char Str ：字符 
                                  
编写日期                        	：2019年1月29日
最后修改日期                  		：2019年1月29日
-----------------------------------------------------------------------*/
void LCD12864_DisplayStrChinSingle(uint8_t X,uint8_t Y,char *Str)
{
	LCD12864_SetLocal(X,Y);                                                                // 设置坐标

	LCD12864_WriteData(*Str);
	LCD12864_WriteData(*(Str+1));

}
/*-----------------------------------------------------------------------
LCD12864_DisplayStrChin    	    : LCD12864显示字符串和汉字
	
输入参数                   			：uint8_t X ：X轴坐标,uint8_t Y ：Y轴坐标,char *Str ：字符串
                                  
编写日期                        	：2019年1月29日
最后修改日期                  		：2019年1月29日
-----------------------------------------------------------------------*/
void LCD12864_DisplayStrChin(uint8_t X,uint8_t Y,char *Str)
{
	LCD12864_SetLocal(X,Y);                                                                // 设置坐标
	while(*Str != '\0')
	{
		LCD12864_WriteData(*Str);
		Str ++;
	}
		LCD12864_WriteData(*Str);
}

void LCD12864_Clear(void)
{
//	LCD12864_WriteCmd(0x01);                                                               // 写入清屏指令
//	Delay_ms(6);	
	  LCD12864_WriteCmd(0x30);                                                             // 选择基本指令集，和，8位数据模式。
    Delay_ms(5);
    LCD12864_WriteCmd(0x0c);                                                             //开显示,无游标,不反白.
    Delay_ms(5); 
    LCD12864_WriteCmd(0x01);                                                             //清除显示，并将 DDRAM 的地址计数器 AC 设为 00H.
    Delay_ms(5);
    LCD12864_WriteCmd(0x06);                                                             //设置，外部读写数据后,地址记数器 AC 会自动加 1。
    Delay_ms(5);
    LCD12864_WriteCmd(0x80);                                                             //将 DDRAM 地址计数器 AC 设为 0.
    Delay_ms(5);		
}