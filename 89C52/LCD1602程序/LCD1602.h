#ifndef __LCD1602_H
#define __LCD1602_H 

#include <STC89C5xRC.H>
/*****��������******/
#define function_set				0x38	//��������
#define display_switch1			0x0c	//��ʾ����,�رչ��
#define display_switch2			0x0f	//��ʾ���أ��������
#define clean_lcd1602				0x01	//����
#define moving_direction		0x06	//�ƶ�����

#define moving_left			0x10	//�������1��
#define moving_Right		0x08	//�������1��
/***���Ŷ���***/
sbit RS = P1^1;			//�����ַ���ʾ
sbit RW	= P1^0;			//���ƶ�д
sbit E	= P4^2;			//ʹ������
#define DATA 	P0			//���ݴ�������
sbit	LCD_BLK=P2^4;

extern unsigned char code table[][8];
void lcd_writecmd(unsigned char cmd);		//дָ��
void lcd_writedata(unsigned char dat);	//д����
void LCD1602_Init(void);
void lcd1602_LeftMove(void);
void lcd1602_RightMove(void);
void LCD1602_set_pos(unsigned char x,unsigned char y);
void LCD1602_show_char(unsigned char x,unsigned char y,unsigned char zi);	//��ʾһ���ַ�
void lcd1602_show_string(unsigned char x,unsigned char y,unsigned char *str);//��ʾ�ַ���
void LCD1602_show_num(unsigned char x,unsigned char y,unsigned int Number,unsigned char Length);		//��ʾ����
//void LCD1602_pic(unsigned char x,unsigned char y,unsigned char CGRAM,unsigned char count,unsigned char* pic); 


#endif
