#ifndef __DS1302_H__
#define __DS1302_H__
#include "stm32f1xx_hal.h"

struct DS1302Time{
		char year;   //年
		char month;  //月
		char day;    //日
		char week;   //星期
		char hour;   //小时
		char minute; //分钟
  	char second; //秒
};             
typedef struct DS1302Time DS1302Time;
extern DS1302Time DS1302;

//单字节写,Command为命令字(地址)，Data为要写入的数据
void DS1302_WriteByte(unsigned char Command,unsigned char Data);

//读取时间
void DS1302_ReadTime(void);
//写时间
void DS1302_WriteTime(void);
//时间更新事件
void TimeUpDataEvent(void);


//更新时间到串口屏
void SendTime(void);

#endif
