#include <STC89C5xRC.H>
#include "DHT11.h"

#define DHT11_Data P33
unsigned char rec_dat[9]; //储存数据
void DHT11_delay_us(unsigned char n)
{
    while(--n);
}
void DHT11_delay_ms(unsigned int z)
{
   unsigned int i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}

/*
主机（单片机）发送起始信号：
1.主机先拉高data。
2.拉低data延迟18ms。
3.拉高data并延迟等待（通过此操作将单片机引脚设置为输入）。
*/
void DHT11_start()
{
   DHT11_Data=1;
   DHT11_delay_us(2);
   DHT11_Data=0;
   DHT11_delay_ms(18);   //拉低延时18ms以上
   DHT11_Data=1;
   DHT11_delay_us(30);   //拉高 延时 20~40us，取中间值 30us
}

/*------------------------------------------------
              接收八位二进制
------------------------------------------------*/
unsigned char DHT11_rec_byte()      //接收一个字节
{
  unsigned char i,dat=0;
  for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {          
      while(DHT11_Data);   //等待进入低电平
      while(!DHT11_Data);   //等待50us低电平过去
				DHT11_delay_us(8);     //延时60us，如果还为高则数据为1，否则为0 
      dat<<=1;//移位（低位补零）使正确接收8位数据，数据为0时直接移位
      if(DHT11_Data==1)    //数据为1时，使dat加1来接收数据1
        dat+=1;
      while(DHT11_Data);  //等待数据线拉低    
   }  
    return dat;
}


/*------------------------------------------------
              接收40bit数据（具体的温湿度）
1.主机先把data线拉高（io设置为输入）。
2.从机把data线拉低，主机读取data线电平，直到低电平结束（大约50us）
  从机拉高data线后，延迟40us左右（28~70us之间）主机再次读取data线电平，如果为低电平，则为“0”，如果为高电平，则为“1”。
3.继续重复上述1,2步骤累计40次。
------------------------------------------------*/

void DHT11_receive()      //接收40位的数据
{
    unsigned char R_H,R_L,T_L,T_H,revise; 
    DHT11_start();//发送起始信号：
    if(DHT11_Data==0)
    {
        while(DHT11_Data==0);   //等待拉高     
        DHT11_delay_us(40);  //拉高后延时80us
			
        R_H=DHT11_rec_byte();    //接收湿度高八位  
        R_L=DHT11_rec_byte();    //接收湿度低八位  
        T_H=DHT11_rec_byte();    //接收温度高八位  
        T_L=DHT11_rec_byte();    //接收温度低八位
        revise=DHT11_rec_byte(); //接收校正位

        DHT11_delay_us(25);    //结束

        if((R_H+R_L+T_H+T_L)==revise)      //最后一字节为校验位，校验是否正确
        {
					//湿度
					rec_dat[0]=R_H;
					rec_dat[1]=R_L;	
					//温度
					rec_dat[4]=T_H;
					rec_dat[5]=T_L; 	
					
        } 
				
				

				
				
//        /*数据处理，转换为字符，方便显示*/
//			  //湿度
//				rec_dat[0]='0'+(RH/10);
//        rec_dat[1]='0'+(RH%10);
//				rec_dat[2]=' ';
//				rec_dat[3]=' ';
//				
//				//温度
//        rec_dat[4]='0'+(TH/10);
//        rec_dat[5]='0'+(TH%10); 
//		    rec_dat[6]=' ';
			
    }

}
