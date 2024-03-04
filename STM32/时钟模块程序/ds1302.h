#ifndef __DS1302_H__
#define __DS1302_H__
#include "stm32f1xx_hal.h"

struct DS1302Time{
		char year;   //��
		char month;  //��
		char day;    //��
		char week;   //����
		char hour;   //Сʱ
		char minute; //����
  	char second; //��
};             
typedef struct DS1302Time DS1302Time;
extern DS1302Time DS1302;

//���ֽ�д,CommandΪ������(��ַ)��DataΪҪд�������
void DS1302_WriteByte(unsigned char Command,unsigned char Data);

//��ȡʱ��
void DS1302_ReadTime(void);
//дʱ��
void DS1302_WriteTime(void);
//ʱ������¼�
void TimeUpDataEvent(void);


//����ʱ�䵽������
void SendTime(void);

#endif
