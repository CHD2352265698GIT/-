 #ifndef __DS18B20_H
 #define __DS18B20_H
#include <STC89C5xRC.H>
 
//温度转化：初始化-->跳过ROM-->开始温度变换
void DS18B20_ConvertT(void);
//温度转化：初始化-->跳过ROM-->开始温度变换
void DS18B20_ConvertT(void);
float DS18B20_ReadT(void);
 #endif
 