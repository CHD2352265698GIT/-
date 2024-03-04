#ifndef __ADS1115_H
#define __ADS1115_H
#include <STC89C5xRC.H>

sbit   SCL=P3^7;         //SCL
sbit   SDA=P3^6;         //SDA
sbit   ALE=P2^1;
struct ADC{
	unsigned int channel_3;
	unsigned int channel_2;
	unsigned int channel_1;
	unsigned int channel_0;
};
typedef struct ADC ADC;
extern xdata ADC ADS1115;

struct person{
	int state;
	int IN;
	int Out;
	int number;
};
typedef struct person person;
extern  person Person;

void ADC_Timer0Init(void);
/*****ADC模拟量采集转化为数字量*********/
unsigned int  Ad_1115(unsigned char channel_Ad);

void ADS1115_Read(void); //读取4个通道

void LOGIC(void);

#endif
