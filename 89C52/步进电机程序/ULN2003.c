#include <STC89C5xRC.H>
#include "ULN2003.h"

#define IN1 P00
#define IN2 P01
#define IN3 P02
#define IN4 P03
bit Uln2003Enable;
bit Uln2003Direction;
char Uln2003Loop=0;
char Uln2003Speed=0;
void Uln2003Ctrl(void);
//定时器控制输出
void Uln2003LoopAdd(void)
{
	static unsigned char speedtemp=0;
	if(Uln2003Enable)
	{
		if(Uln2003Loop==4)
		{
			Uln2003Loop=1;
		}
		speedtemp++;
		if(speedtemp==Uln2003Speed)
		{
			speedtemp=0;
			Uln2003Loop++;
		}
		Uln2003Ctrl();
	}
}

void Uln2003Ctrl(void)
{
	if(Uln2003Direction)
	{
			switch(Uln2003Loop)
		{
			case 1: IN1=1;IN2=0;IN3=0;IN4=0;	
				break;
			case 2: IN1=0;IN2=1;IN3=0;IN4=0;
				break;
			case 3: IN1=0;IN2=0;IN3=1;IN4=0;
				break;
			case 4: IN1=0;IN2=0;IN3=0;IN4=1;
				break;
		}
	}else
	{
			switch(Uln2003Loop)
		{
			case 1: IN1=0;IN2=0;IN3=0;IN4=1;	
				break;
			case 2: IN1=0;IN2=0;IN3=1;IN4=0;
				break;
			case 3: IN1=0;IN2=1;IN3=0;IN4=0;
				break;
			case 4: IN1=1;IN2=0;IN3=0;IN4=0;
				break;
		}
	}
}
//正转
void Foreward(char Speed)
{
	Uln2003Enable=1;
	Uln2003Speed = Speed;
	Uln2003Direction =1;
}
//反转
void Reversal(char Speed)
{
	Uln2003Enable=1;
	Uln2003Speed = Speed;
	Uln2003Direction =0;
}
//停止
void Stop(void)
{
	Uln2003Enable=0;
}
