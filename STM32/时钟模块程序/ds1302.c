#include "ds1302.h"               
#include "main.h"
#include "stdio.h"
#include "GPIO_RW.h"
#include "waterquality.h"                
#include "watertemp.h"                
#include "waterlevel.h"
#include "ds18b20.h"          
#include "hc_sr04.h"           
#include "tdswater.h"  
#include "music.h"
DS1302Time DS1302={00,0,0,0,0,0};
DS1302Time DS1302_TimeUpData; //时间事件更新
//硬件需要上拉电阻
#define I2C_CLK_PORT        GPIOA
#define I2C_CLK_PIN         GPIO_PIN_7
 
#define I2C_SDA_PORT        GPIOB
#define I2C_SDA_PIN         GPIO_PIN_0

#define TSCLK  PA7
#define TIO  PB0
#define SCL_High()        HAL_GPIO_WritePin(I2C_CLK_PORT, I2C_CLK_PIN, GPIO_PIN_SET) 
#define SCL_Low()         HAL_GPIO_WritePin(I2C_CLK_PORT, I2C_CLK_PIN, GPIO_PIN_RESET)  
#define SDA_High()        HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET) 
#define SDA_Low()         HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET) 
#define SDAM()           (HAL_GPIO_ReadPin(I2C_SDA_PORT,I2C_SDA_PIN) & 0x20) ?1:0
#define TRST  PB1
 
uint8_t g_u8ReadData[8];
static void Delay_us(uint32_t xus)
{
   uint8_t i; 
   for (i=5; i>0; i--);
} 


void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		TSCLK=0;
		TIO =temp&0x01;
		temp>>=1;
		TSCLK=1;
	}
}   

//单字节写,Command为命令字(地址)，Data为要写入的数据
void DS1302_WriteByte(unsigned char Command,unsigned char Data){
 	TRST=0;  Delay_us(1);
 	TSCLK=0; Delay_us(1);
 	TRST=1;  Delay_us(1);
 	Write_Ds1302(Command);	
 	Write_Ds1302(Data);		
 	TRST=0; 
}
//DS1302读一个字节，Command为命令字(地址)，Data为读出的数据
unsigned char DS1302_ReadByte(unsigned char Command){
 unsigned char i,temp=0x00;
 	TRST=0;	Delay_us(1);
 	TSCLK=0;Delay_us(1);
 	TRST=1;	Delay_us(1);
 	Write_Ds1302(Command);
 	for (i=0;i<8;i++) 	
 	{		
		TSCLK=0;
		temp>>=1;	
 		if(HAL_GPIO_ReadPin(I2C_SDA_PORT,I2C_SDA_PIN))
 		temp|=0x80;	
 		TSCLK=1;
	} 
 	TRST=0;	Delay_us(1);
 	TSCLK=0;Delay_us(1);
	TSCLK=1;Delay_us(1);
	TIO=0;	Delay_us(1);
	TIO=1;	Delay_us(1);
	return (temp);			
}

uint8_t dat_to_BCD(uint8_t dat)//数据转BCD码
{
	uint8_t dat1,dat2;
	dat1 = dat / 10;
	dat2 = dat % 10;
	dat2 = dat2 + dat1 * 16;
	return dat2;
}

uint8_t BDD_to_dat(uint8_t dat)//BCD码转数据
{
	uint8_t dat1,dat2;
	dat1 = dat /16;
	dat2 = dat % 16;
	dat2 = dat2 + dat1 * 10;
	return dat2;
}

#define  second_reg 0x81
#define  minute_reg 0x83
#define  hour_reg   0x85
#define  day_reg    0x87
#define  month_reg  0x89
#define  year_reg   0x8D


//读取时间
void DS1302_ReadTime(void)
{
	  DS1302.second =BDD_to_dat( DS1302_ReadByte(second_reg));//读秒寄存器（进行BCD码转换）
		DS1302.minute = BDD_to_dat(DS1302_ReadByte(minute_reg));//读分寄存器
		DS1302.hour = BDD_to_dat(DS1302_ReadByte(hour_reg));//读时寄存器	
		DS1302.day = BDD_to_dat(DS1302_ReadByte(day_reg));//读时寄存器	
		DS1302.month = BDD_to_dat(DS1302_ReadByte(month_reg));//读时寄存器	
		DS1302.year = BDD_to_dat(DS1302_ReadByte(year_reg));//读时寄存器	
}
#define  second_wreg 0x80
#define  minute_wreg 0x82
#define  hour_wreg   0x84
#define  day_wreg    0x86
#define  month_wreg  0x88
#define  year_wreg   0x8c
void DS1302_WriteTime(void)
{
	DS1302_WriteByte(0X8e,0X00);
	DS1302_WriteByte( second_wreg,dat_to_BCD(DS1302.second));
	DS1302_WriteByte( minute_wreg,dat_to_BCD(DS1302.minute));
	DS1302_WriteByte( hour_wreg,dat_to_BCD(DS1302.hour));
	DS1302_WriteByte( day_wreg,dat_to_BCD(DS1302.day));
	DS1302_WriteByte( month_wreg,dat_to_BCD(DS1302.month));
	DS1302_WriteByte( year_wreg,dat_to_BCD(DS1302.year));
}

void SendTime(void)
{
		printf("main.n4.val=%d\xff\xff\xff",2000+DS1302.year);
		printf("main.n5.val=%d\xff\xff\xff",DS1302.month);    
		printf("main.n6.val=%d\xff\xff\xff",DS1302.day);      
		printf("main.n3.val=%d\xff\xff\xff",DS1302.second);   
		printf("main.n2.val=%d\xff\xff\xff",DS1302.minute);   
		printf("main.n1.val=%d\xff\xff\xff",DS1302.hour);
	
		printf("temp.n4.val=%d\xff\xff\xff",DS1302.second);
		printf("temp.n5.val=%d\xff\xff\xff",DS1302.minute);    
		printf("temp.n6.val=%d\xff\xff\xff",DS1302.hour);      
		printf("temp.n7.val=%d\xff\xff\xff",2000+DS1302.year);   
		printf("temp.n8.val=%d\xff\xff\xff",DS1302.month);   
		printf("temp.n9.val=%d\xff\xff\xff",DS1302.day);

}

char timeadd=playerrortime;
void TimeUpDataEvent(void)
{
	if(DS1302.day != DS1302_TimeUpData.day)//天数更新
	 {
	  DS1302_TimeUpData.day = DS1302.day;
		UpdataWQuality();  //更新水质显示
		UpdataTemp();  //更新温度显示
		UpdataLevel();   //更新水位显示
		UpdataHistory(); //更新水质历史
	  UpdataTempHistory();//更新温度历史
		UpdataLevelHistory();//更新水位历史
		 SendTime(); //校准屏幕时间
	 }
	if(DS1302.hour != DS1302_TimeUpData.hour)//小时更新
	 {
		 DS1302_TimeUpData.hour =  DS1302.hour ;
		 SendTime(); //校准屏幕时间
	 }	 
	if(DS1302.second != DS1302_TimeUpData.second)//秒更新
	 {
		 DS1302_TimeUpData.second =  DS1302.second ;	
		 extern WaterQuality WQuality;
		 WQuality.quality = GetTds();
		 extern char Page[5];
		 if(Page[3])
			 printf("watertemp.t1.txt=\"%2.2f\"\xff\xff\xff",DS18B20_ReadT());
		 if(Page[2])
			 printf("waterquality.t8.txt=\"%2.2fppm\"\xff\xff\xff",GetTds());
		 extern void CheckData(char data);
		 CheckData(WQuality.quality);
		 if(Page[2])printf("waterquality.t1.txt=\"%s\"\xff\xff\xff",WQuality.viewdata); //设置优良显示
		 extern char YL38Error;
		 if(YL38Error)
		 {
			 if(timeadd>=playerrortime)
			 {
				 timeadd=0;
				 MusicControl(num,15);
			 }
			 timeadd++;
		 }
		 SendDitance();
	 }
}
