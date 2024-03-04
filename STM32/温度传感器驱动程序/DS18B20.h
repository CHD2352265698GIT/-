#ifndef __DS18B20_H__
#define __DS18B20_H__

//温度转化：初始化-->跳过ROM-->开始温度变换
void DS18B20_ConvertT(void);
//温度读取：初始化-->跳过ROM-->读暂存器-->连续的读操作
float DS18B20_ReadT(void);


#endif
