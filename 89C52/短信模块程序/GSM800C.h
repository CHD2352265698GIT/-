#ifndef __GSM800C_H
#define __GSM800C_H

#include <STC89C5xRC.H>
typedef   unsigned char  u8;
typedef   unsigned int   u16;
typedef   unsigned long  u32;
static unsigned char *SMS_Num="\"+8613800816500\"";//短信中心号
static unsigned char *phone="\"13228330886\"";     //短信接收方手机号
static unsigned char *content="HELLO SIM800C";		 //发送短信内容
#define Automatic_Startup 1     //定义自启动 V1.2版本起有自启动功能
#define Buf1_Max 30 					  //串口1缓存长度
//串口1发送回车换行
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)
extern xdata u8 Uart1_Buf[Buf1_Max];
extern u8 First_Int;

sbit LED = P3^7;


void GPIO_config(void); //引脚初始化
void Uart1Init(void);		//9600bps@11.0592MHz
void CLR_Buf1(void);    //清串口接收缓存
u8 Find(u8 *a);         //查找字符串
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);  //发送指令
void Wait_CREG(void);   //查询等待模块注册成功
void Set_Text_Mode(void); //设置短信为TEXT模式
void Send_Text_Sms(void); //发送一条TEXT短信


#endif

