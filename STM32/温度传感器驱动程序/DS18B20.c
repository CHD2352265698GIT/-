#include "ds18b20.h"          
#include "gpio.h"
#include "music.h"              

#define OneWire_DQ(x)   HAL_GPIO_WritePin(GPIOA, DS18B20_Pin, (GPIO_PinState)x)
#define ROneWire_DQ   HAL_GPIO_ReadPin(GPIOA, DS18B20_Pin)
#define DS18B20_SKIP_ROM 			0xCC
#define DS18B20_CONVERT_T 			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE


/*初始化
	主机 将总线拉低至少480us，然后释放总线，
	等待15~60us后，存在的 从机 会拉低总线60~240us以响应 主机
	之后 从机 将释放总线	*/
//单总线
unsigned char OneWire_Init(void)
{
	unsigned char AckBit;
	OneWire_DQ(1);	//先给总线置1
	OneWire_DQ(0);	//再拉低总线
 
	HAL_Delay_us(500);	//延时500us（至少拉低总线480us）
	OneWire_DQ(1);			//然后释放总线
	HAL_Delay_us(70);	//延时70us
	AckBit = ROneWire_DQ;	//读出I/O口电平，存放在AckBit
	HAL_Delay_us(500);	//延时500us（也是至少480us）
	return AckBit;
}
 
/*发送一位
	主机将总线拉低60~120us，然后释放总线，表示发送0；
	主机将总线拉低1~15us，然后释放总线，表示发送1。
	从机 将在总线拉低30us后读取电平，整个时间片应大于60us
	*/
void OneWire_SendBit(unsigned char Bit)
{	
	OneWire_DQ(0);			//先直接拉低，初始化之后总线是1,
	HAL_Delay_us(10);	//延时10us
	OneWire_DQ(Bit);		//10us后将Bit送回总线
	HAL_Delay_us(50);	//延时50us
	OneWire_DQ(1);			//最后释放总线
}
 
/*接收一位
	主机将总线拉低1~15us，然后释放总线，并在拉低后15us内读取总线电平（尽量贴近15us末尾）
	读取为低电平则为接收0，读取为高电平则为接收1，整个时间片应大于60us	*/
unsigned char OneWire_ReceiveBit(void)
{
	unsigned char Bit;
	OneWire_DQ(0);			//主机将总线拉低1~15us
	HAL_Delay_us(5);	//延时5us
	OneWire_DQ(1);			//释放总线
	HAL_Delay_us(5);	//延时5us
	Bit = ROneWire_DQ;		//采样、读取电平（读取到0就是从机给拉低了）
	HAL_Delay_us(50);	//延时50us
	//结束后 从机 会自动释放总线 
	return Bit;
}
 
//发送一个字节
void OneWire_SendByte(unsigned char Byte)
{	
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}	
}
 
//接收一个字节
unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte = 0x00;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit()) {Byte |= (0x01<<i);} //从低位到高位依次取出
		//这个if判断，判断为0的时候，Byte当前位还是0
	}
	return Byte;
}
 
 
//温度转化：初始化-->跳过ROM-->开始温度变换
void DS18B20_ConvertT(void)
{
	OneWire_Init();//初始化
	OneWire_SendByte(DS18B20_SKIP_ROM);//跳过ROM
	OneWire_SendByte(DS18B20_CONVERT_T);//温度变换
}
 
//温度读取：初始化-->跳过ROM-->读暂存器-->连续的读操作
float DS18B20_ReadT(void)
{	
	unsigned char TLSB,TMSB;
	int temp;//16位数据
	float T;
	DS18B20_ConvertT();
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);//读暂存器
	//一旦发送完指令，控制权交给 从机 
	
	TLSB = OneWire_ReceiveByte();	//读的Byte0,低8位数据
	TMSB = OneWire_ReceiveByte();	//读的Byte1,高8位数据
	
	temp = (TMSB << 8) | TLSB;		//正好为int类型16位数据
	 
	/*温度存储格式整体左移了四位，数据扩大了16倍
	bit0是2的-4次幂，bit4才是2的0次幂 = 1*/
	T = temp/16.0;	//强制类型转换
	
	return T;
}
