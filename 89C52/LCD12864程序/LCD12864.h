#ifndef __LCD12864_
#define __LCD12864_

#include <STC89C5xRC.H>

#define LCD_RW  P37
#define LCD_DI  P36
#define LCD_EN  P40
#define LCD_REST P41
#define LCD_DATA P2
void LCD12864_WriteData(uint8_t Data);                        // LCD12864д����
void LCD12864_WriteCmd(uint8_t Cmd);                          // LCD12864д����
void LCD12864_Init(void);                                     // LCD12864��ʼ��
void LCD12864_SetLocal(uint8_t X,uint8_t Y);
void LCD12864_DisplayStrChin(uint8_t X,uint8_t Y,char *Str);
void LCD12864_DisplayStrChinSingle(uint8_t X,uint8_t Y,char *Str);
void LCD12864_Clear(void);
void LCD12864_DrawClear(void);

#endif
