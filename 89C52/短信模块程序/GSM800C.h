#ifndef __GSM800C_H
#define __GSM800C_H

#include <STC89C5xRC.H>
typedef   unsigned char  u8;
typedef   unsigned int   u16;
typedef   unsigned long  u32;
static unsigned char *SMS_Num="\"+8613800816500\"";//�������ĺ�
static unsigned char *phone="\"13228330886\"";     //���Ž��շ��ֻ���
static unsigned char *content="HELLO SIM800C";		 //���Ͷ�������
#define Automatic_Startup 1     //���������� V1.2�汾��������������
#define Buf1_Max 30 					  //����1���泤��
//����1���ͻس�����
#define UART1_SendLR() UART1_SendData(0X0D);\
											 UART1_SendData(0X0A)
extern xdata u8 Uart1_Buf[Buf1_Max];
extern u8 First_Int;

sbit LED = P3^7;


void GPIO_config(void); //���ų�ʼ��
void Uart1Init(void);		//9600bps@11.0592MHz
void CLR_Buf1(void);    //�崮�ڽ��ջ���
u8 Find(u8 *a);         //�����ַ���
void Second_AT_Command(u8 *b,u8 *a,u8 wait_time);  //����ָ��
void Wait_CREG(void);   //��ѯ�ȴ�ģ��ע��ɹ�
void Set_Text_Mode(void); //���ö���ΪTEXTģʽ
void Send_Text_Sms(void); //����һ��TEXT����


#endif

