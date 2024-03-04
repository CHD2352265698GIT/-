#include "DS18B20.h"
#include <intrins.h>
#include "DELAY.H"
#define OneWire_DQ P35   //DS18B20������
#define DS18B20_SKIP_ROM 			0xCC
#define DS18B20_CONVERT_T 			0x44
#define DS18B20_READ_SCRATCHPAD 	0xBE
/*��ʼ��
	���� ��������������480us��Ȼ���ͷ����ߣ�
	�ȴ�15~60us�󣬴��ڵ� �ӻ� ����������60~240us����Ӧ ����
	֮�� �ӻ� ���ͷ�����	*/
//������
unsigned char OneWire_Init(void)
{
	unsigned char i;
	unsigned char AckBit;
	EA = 0;	
	OneWire_DQ = 1;	//�ȸ�������1
	OneWire_DQ = 0;	//����������
 
	i = 230; while (--i);	//��ʱ500us��������������480us��
	OneWire_DQ = 1;			//Ȼ���ͷ�����
	i = 32; while (--i);	//��ʱ70us
	AckBit = OneWire_DQ;	//����I/O�ڵ�ƽ�������AckBit
	i = 230; while (--i);	//��ʱ500us��Ҳ������480us��
	EA = 1;	
	return AckBit;
}
 
/*����һλ
	��������������60~120us��Ȼ���ͷ����ߣ���ʾ����0��
	��������������1~15us��Ȼ���ͷ����ߣ���ʾ����1��
	�ӻ� ������������30us���ȡ��ƽ������ʱ��ƬӦ����60us
	*/
void OneWire_SendBit(unsigned char Bit)
{	
	unsigned char i;
	EA = 0;	
	OneWire_DQ = 0;			//��ֱ�����ͣ���ʼ��֮��������1,
	i = 4;	while (--i);	//��ʱ10us
	OneWire_DQ = Bit;		//10us��Bit�ͻ�����
	i = 23;	while (--i);	//��ʱ50us
	OneWire_DQ = 1;			//����ͷ�����
	EA = 1;	
}
 
/*����һλ
	��������������1~15us��Ȼ���ͷ����ߣ��������ͺ�15us�ڶ�ȡ���ߵ�ƽ����������15usĩβ��
	��ȡΪ�͵�ƽ��Ϊ����0����ȡΪ�ߵ�ƽ��Ϊ����1������ʱ��ƬӦ����60us	*/
unsigned char OneWire_ReceiveBit(void)
{
	
	unsigned char i;
	unsigned char Bit;
	EA = 0;
	OneWire_DQ = 0;			//��������������1~15us
	i = 2;	while (--i);	//��ʱ5us
	OneWire_DQ = 1;			//�ͷ�����
	i = 2;	while (--i);	//��ʱ5us
	Bit = OneWire_DQ;		//��������ȡ��ƽ����ȡ��0���Ǵӻ��������ˣ�
	i = 23;	while (--i);	//��ʱ50us
	//������ �ӻ� ���Զ��ͷ����� 
	EA = 1;	
	return Bit;
}
 
//����һ���ֽ�
void OneWire_SendByte(unsigned char Byte)
{	
	unsigned char i;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}	
}
 
//����һ���ֽ�
unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte = 0x00;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit()) {Byte |= (0x01<<i);} //�ӵ�λ����λ����ȡ��
		//���if�жϣ��ж�Ϊ0��ʱ��Byte��ǰλ����0
	}
	return Byte;
}
 
 
//�¶�ת������ʼ��-->����ROM-->��ʼ�¶ȱ任
void DS18B20_ConvertT(void)
{
	OneWire_Init();//��ʼ��
	OneWire_SendByte(DS18B20_SKIP_ROM);//����ROM
	OneWire_SendByte(DS18B20_CONVERT_T);//�¶ȱ任
}
 
//�¶ȶ�ȡ����ʼ��-->����ROM-->���ݴ���-->�����Ķ�����
float DS18B20_ReadT(void)
{	
	unsigned char TLSB,TMSB;
	int temp;//16λ����
	float T;
	DS18B20_ConvertT();
	OneWire_Init();
	OneWire_SendByte(DS18B20_SKIP_ROM);
	OneWire_SendByte(DS18B20_READ_SCRATCHPAD);//���ݴ���
	//һ��������ָ�����Ȩ���� �ӻ� 
	
	TLSB = OneWire_ReceiveByte();	//����Byte0,��8λ����
	TMSB = OneWire_ReceiveByte();	//����Byte1,��8λ����
	
	temp = (TMSB << 8) | TLSB;		//����Ϊint����16λ����
	 
	/*�¶ȴ洢��ʽ������������λ������������16��
	bit0��2��-4���ݣ�bit4����2��0���� = 1*/
	T = temp/16.0;	//ǿ������ת��
	
	return T;
}