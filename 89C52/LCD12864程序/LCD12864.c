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
LCD12864_ReadData   	    			: LCD12864������
	
�������                   			����
                                  
-----------------------------------------------------------------------*/
uint8_t LCD12864_ReadData()
{
	return 0;
}
void LCD12864_Init()
{
	//���ȶ�LCD���и�λ
//	LCD_REST = 0;
//	Delay_ms(30);
//	LCD_REST = 1;
//	Delay_ms(20);

	/* ��ʼ��ʼ���Ĳ��� */
	  LCD12864_WriteCmd(0x30);                                                             // ѡ�����ָ����ͣ�8λ����ģʽ��
    Delay_ms(5);
    LCD12864_WriteCmd(0x0c);                                                             //����ʾ,���α�,������.
    Delay_ms(5); 
    LCD12864_WriteCmd(0x01);                                                             //�����ʾ������ DDRAM �ĵ�ַ������ AC ��Ϊ 00H.
    Delay_ms(5);
    LCD12864_WriteCmd(0x06);                                                             //���ã��ⲿ��д���ݺ�,��ַ������ AC ���Զ��� 1��
    Delay_ms(5);
    LCD12864_WriteCmd(0x80);                                                             //�� DDRAM ��ַ������ AC ��Ϊ 0.
    Delay_ms(5);	
}

/*-----------------------------------------------------------------------
LCD12864_SetLocal    	    	    : LCD12864��������
	
�������                   			����
                                  
��д����                        	��2019��1��29��
����޸�����                  		��2019��1��29��
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
LCD12864_DisplayStrChinSingle   : LCD12864��ʾ�����ַ����ͺ���
	
�������                   			��uint8_t X ��X������,uint8_t Y ��Y������,char Str ���ַ� 
                                  
��д����                        	��2019��1��29��
����޸�����                  		��2019��1��29��
-----------------------------------------------------------------------*/
void LCD12864_DisplayStrChinSingle(uint8_t X,uint8_t Y,char *Str)
{
	LCD12864_SetLocal(X,Y);                                                                // ��������

	LCD12864_WriteData(*Str);
	LCD12864_WriteData(*(Str+1));

}
/*-----------------------------------------------------------------------
LCD12864_DisplayStrChin    	    : LCD12864��ʾ�ַ����ͺ���
	
�������                   			��uint8_t X ��X������,uint8_t Y ��Y������,char *Str ���ַ���
                                  
��д����                        	��2019��1��29��
����޸�����                  		��2019��1��29��
-----------------------------------------------------------------------*/
void LCD12864_DisplayStrChin(uint8_t X,uint8_t Y,char *Str)
{
	LCD12864_SetLocal(X,Y);                                                                // ��������
	while(*Str != '\0')
	{
		LCD12864_WriteData(*Str);
		Str ++;
	}
		LCD12864_WriteData(*Str);
}

void LCD12864_Clear(void)
{
//	LCD12864_WriteCmd(0x01);                                                               // д������ָ��
//	Delay_ms(6);	
	  LCD12864_WriteCmd(0x30);                                                             // ѡ�����ָ����ͣ�8λ����ģʽ��
    Delay_ms(5);
    LCD12864_WriteCmd(0x0c);                                                             //����ʾ,���α�,������.
    Delay_ms(5); 
    LCD12864_WriteCmd(0x01);                                                             //�����ʾ������ DDRAM �ĵ�ַ������ AC ��Ϊ 00H.
    Delay_ms(5);
    LCD12864_WriteCmd(0x06);                                                             //���ã��ⲿ��д���ݺ�,��ַ������ AC ���Զ��� 1��
    Delay_ms(5);
    LCD12864_WriteCmd(0x80);                                                             //�� DDRAM ��ַ������ AC ��Ϊ 0.
    Delay_ms(5);		
}