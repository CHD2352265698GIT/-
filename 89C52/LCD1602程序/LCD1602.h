#ifndef __LCD1602_H
#define __LCD1602_H 

#include <STC89C5xRC.H>
/*****功能设置******/
#define function_set				0x38	//功能设置
#define display_switch1			0x0c	//显示开关,关闭光标
#define display_switch2			0x0f	//显示开关，开启光标
#define clean_lcd1602				0x01	//清屏
#define moving_direction		0x06	//移动方向

#define moving_left			0x10	//光标左移1格
#define moving_Right		0x08	//光标右移1格
/***引脚定义***/
sbit RS = P1^1;			//控制字符显示
sbit RW	= P1^0;			//控制读写
sbit E	= P4^2;			//使能引脚
#define DATA 	P0			//数据传输引脚
sbit	LCD_BLK=P2^4;

extern unsigned char code table[][8];
void lcd_writecmd(unsigned char cmd);		//写指令
void lcd_writedata(unsigned char dat);	//写数据
void LCD1602_Init(void);
void lcd1602_LeftMove(void);
void lcd1602_RightMove(void);
void LCD1602_set_pos(unsigned char x,unsigned char y);
void LCD1602_show_char(unsigned char x,unsigned char y,unsigned char zi);	//显示一个字符
void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str);//显示字符串
void LCD1602_show_num(unsigned char x,unsigned char y,unsigned int Number,unsigned char Length);		//显示数字
//void LCD1602_pic(unsigned char x,unsigned char y,unsigned char CGRAM,unsigned char count,unsigned char* pic); 


#endif
