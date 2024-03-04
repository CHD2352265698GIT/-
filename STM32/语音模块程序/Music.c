#include "music.h"              
#include "stdio.h"
#include "string.h"
#include "waterquality.h"                
#include "watertemp.h"                
#include "waterlevel.h"                
#include "ds1302.h"               
#include "ds18b20.h"          
#include "hc_sr04.h"           
#include "tdswater.h"                  
#include "gpio_rw.h" 
#include "tim.h"
void HAL_Delay_us(uint32_t us)
{
   uint32_t tem=us;
   __HAL_TIM_SET_COUNTER(&htim2,0);            //将初值设置为0
    HAL_TIM_Base_Start(&htim2);                //开启定时器2
   while(__HAL_TIM_GET_COUNTER(&htim2)<tem);  //计时等待，当count达到所需的us跳出循环
   HAL_TIM_Base_Stop(&htim2);                // 关闭定时器
  
}
void TIdelay_ms(unsigned int ms)
{
	for(unsigned int i=0;i<ms;i++)
	HAL_Delay_us(1000);
}
void TIdelay_s(unsigned int s)
{
	for(unsigned int i=0;i<s;i++)
	TIdelay_ms(1000);
} 
void baud(void)
{
	 HAL_Delay_us(104);
}
#define OI_TXD	PA4
//模拟串口发送字节
void MyuartsendByte(char Byte)
{
	 OI_TXD =0;
		baud();
	for(char conter=0;conter<8;conter++)
	{
		OI_TXD = (Byte>>conter)&1;
		baud();
 }
		OI_TXD =1;
		baud();
}  
//发送字符
void MyUartSend(char* str,const char lenth)
{
	for(char i=0;i<lenth;i++)
	{
		 MyuartsendByte(str[i]);
	}
}
//发送语音模块指令
char mu_play[5]={0x7e,0x03,0x01,0x02,0xef};
char mu_stop[5]={0x7e,0x03,0x02,0x01,0xef};
char mu_next[5]={0x7e,0x03,0x03,0x00,0xef};
char mu_select[7]={0x7e,0x05,0x41,0x00,0x00,0x45,0xef};
char mu_volume[6]={0x7e,0x04,0x31,0x00,0x2c,0xef};



void MusicControl(char command,char data)
{
		switch(command)
		{
			case play:MyUartSend(mu_play,5); HAL_Delay(100);
			break;
			case stop:MyUartSend(mu_stop,5); HAL_Delay(100);
			break;
			case num: mu_select[4]=data;
								mu_select[5]=mu_select[1]^mu_select[2]^mu_select[3]^mu_select[4];
								MyUartSend(mu_select,7);
//								if(data==15)HAL_Delay(3000);
//								else if(data==11||data==17||data==18) HAL_Delay(2000);
//								else HAL_Delay(900);
			break;
			case volume:if(data>30)data=30;
									mu_volume[3]=data;
									mu_volume[4]=mu_volume[1]^mu_volume[2]^mu_volume[3];
									MyUartSend(mu_volume,6);
				break;
			case next:MyUartSend(mu_next,5);
				break;
			default:break;
		}
}

