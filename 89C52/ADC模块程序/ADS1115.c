#include "ADS1115.H"
#include "delay.h"
#include <STDIO.H>
#include <intrins.h>
#include "file.h"                 




unsigned char Initdata[4]={0};
unsigned char cnt;


xdata ADC ADS1115={0,0,0,0};
person Person={0,0,0,0};
//******************延时子程序 *******************************
void delayus(unsigned char a)
{
	unsigned char i=0;
  for(i=0;i>a;i++);
}
void delay(unsigned char t)
{
	unsigned char i=0;
  for(i=0;i>t;i++)
	{
	
	}
}
void delay1ms(unsigned char t)
{
 unsigned char i;
 unsigned char j;
 for(i=124;i>0;i--);
        for(j=0;j<t;j++);
}

/****************************开始ADC通信*************************/
void StartADcom()
{
  SDA =0;
  SCL=1;
  delayus(100);
  SDA =1;
  delayus(20);
  SDA =0;
  SCL=0;  
  delayus(10);
}

/***********************************停止ADC通信**************************************/
void StopADcom()
{
  SDA=0;
  delayus(10);
  SCL=1;
  delayus(20);
  SDA=1;
  delayus(10);
}
/********************************单字节I2C传输**************************************/
void Send_Byte(unsigned char byte)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
		if((byte<<i)&0x80)
		{
			SDA=1;
		}
    else
    {
			SDA=0;
		}
		delay(5);
		SCL=1;
		delay(10);
		SCL=0;       
    delay(10);             
   }
  delay(5);
  SDA=1;
  delay(5);
  SCL=1;
  delay(10);
  SCL=0;
  delay(10);  
}
/*************************读取一个字节*****************************/
unsigned char Read_Byte()
{
  unsigned char temp=0;
  unsigned char i;
  for(i=0;i<8;i++)
        {
                temp=temp<<1;           //shift left to receive next bit
                SCL=0;                        //set clock high
                delayus(10);
                SCL=1;
                delayus(5);
                if(SDA==1)                //check if Data=high
                temp|=0x01;            //yes - set bit high
                _nop_();
                delayus(10);               
        }
  SCL=0;
  delayus(5);
  SDA=0;
  delayus(5);
  SCL=1;
  delayus(20);
  SCL=0;
  delayus(5);
  SDA=1;
  return temp;
}
/**************************配置ADC转换芯片*************************************/
void Confige1115(unsigned char channel_x)
{
  unsigned char i=0;
  unsigned char a_channel;
  switch(channel_x)
        {
                case 1:              
                a_channel=0xc2;               
                break;       
               
                case 2:              
                a_channel=0xd2;               
                break;
               
                case 3:              
                a_channel=0xe2;               
                break;
               
                case 4:              
                a_channel=0xf2;               
                break;
               
                default:
                break;       
        }
  Initdata[0] =0x90;      
  Initdata[1] =0x01;      
  Initdata[2]=a_channel;   
  Initdata[3] =0x83;      
  SCL=1;
  StartADcom();
  for(i=0;i<4;i++)
        {
                Send_Byte(Initdata[i]);
                delayus(20);
        }
  StopADcom();
}
/********指向转换结果寄存器**************************/
void Pointregister(void)
{
  unsigned char i=0;
  Initdata[0] =0x90;           
  Initdata[1] =0x00;            
  SCL=1;
  StartADcom();
  for(i=0;i<2;i++)
  {
          Send_Byte(Initdata[i]);
    delayus(20);
        }
  StopADcom();
  delayus(10);
}
/*****************读取ADC转换结果*******************/
unsigned int Read1115(void)
{
  unsigned char   ResultL,ResultH;
  unsigned int    Result;
  Initdata[0] =0x91;           
  SCL=1;
  StartADcom();
  delayus(10);
  Send_Byte(Initdata[0]);
  delayus(20);
  ResultH=Read_Byte();
  delayus(10);
  ResultL=Read_Byte();        
  StopADcom();
  Result=ResultH*256+ResultL;   
  return   Result;
}
/*****ADC模拟量采集转化为数字量*********/
unsigned int Ad_1115(unsigned char channel_Ad)                        
{
	unsigned int adc_value;
	Confige1115(channel_Ad);
	delay1ms(1);
	Pointregister();               
	delay_ms(20);
	adc_value=Read1115();    
	(adc_value>32766)?adc_value=32767:adc_value;
	return   adc_value;
}
char channel[4]={0x01,0x02,0x03,0x04};


void ADS1115_Read(void)
{	
		ADS1115.channel_1=Ad_1115(channel[1]);

  	ADS1115.channel_3=Ad_1115(channel[3]);

		ADS1115.channel_2=Ad_1115(channel[2]);
}




void LOGIC(void)
{
		//判断出
	 if(ADS1115.channel_2 ==  Person.state && ADS1115.channel_3 !=  Person.state)
	 {
			  Person.Out=1;
	 }
	 if( Person.Out==1)
	 {
		  if(ADS1115.channel_2 !=  Person.state && ADS1115.channel_3 !=  Person.state)
			{
				 Person.Out=2;
			}
	 }
	 if( Person.Out==2)
	 {
//			if(ADS1115.channel_2 != Person.state && ADS1115.channel_3 == Person.state)
//			{
				 Person.Out=3;
//			}
	 }
	 	 if( Person.Out==3)
	 {
		  if(ADS1115.channel_2 ==  Person.state && ADS1115.channel_3 ==  Person.state)
			{
				 Person.Out=4;
			}
	 }
	 if(Person.Out==4)
	 {     
//					Person.state=ADS1115.channel_3; //校准adc比较值
					 Person.number--;
					 Person.Out=0;
	 }

	 //判断进
	 if(ADS1115.channel_2 !=  Person.state && ADS1115.channel_3 ==  Person.state)
	 {
			  Person.IN=1;
	 }
	 if( Person.IN==1)
	 {
//		  if(ADS1115.channel_2 != Person.state && ADS1115.channel_3 != Person.state)
//			{
				 Person.IN=2;
//			}
	 }
	 if( Person.IN==2)
	 {
		  if(ADS1115.channel_2 ==  Person.state && ADS1115.channel_3 !=  Person.state)
			{
				 Person.IN=3;
			}
	 }
	 	 if( Person.IN==3)
	 {
		  if(ADS1115.channel_2 ==  Person.state && ADS1115.channel_3 ==  Person.state)
			{
				 Person.IN=4;
			}
	 }
	 if( Person.IN==4)
	 {
//					 Person.state=ADS1115.channel_3;//校准adc比较值
					  Person.number++;
					  Person.IN=0;
	 }
	 	(Person.number>60)?Person.number=Person.number:Person.number;
	 (Person.number<0)?Person.number=0:Person.number;	 

	 		if(LED.col)
		{
			if( Person.number>0){
				LED.LED1=1;
			}else {
				LED.LED1=0;
			}
			if( Person.number>2){
				LED.LED2=1;
			}else {
				LED.LED2=0;
			}
			if( Person.number>4){
				LED.LED3=1;
			}else {
				LED.LED3=0;
			}
			if( Person.number>6){
				LED.LED4=1;
			}else {
				LED.LED4=0;
			}	
			if( Person.number>8){
				LED.LED5=1;
			}else {
				LED.LED5=0;
			}
			if( Person.number>10){
				LED.LED6=1;
			}else {
				LED.LED6=0;
			}
		}	 


}

//光敏电阻判断
