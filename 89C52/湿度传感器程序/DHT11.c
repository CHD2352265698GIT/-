#include <STC89C5xRC.H>
#include "DHT11.h"

#define DHT11_Data P33
unsigned char rec_dat[9]; //��������
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
��������Ƭ����������ʼ�źţ�
1.����������data��
2.����data�ӳ�18ms��
3.����data���ӳٵȴ���ͨ���˲�������Ƭ����������Ϊ���룩��
*/
void DHT11_start()
{
   DHT11_Data=1;
   DHT11_delay_us(2);
   DHT11_Data=0;
   DHT11_delay_ms(18);   //������ʱ18ms����
   DHT11_Data=1;
   DHT11_delay_us(30);   //���� ��ʱ 20~40us��ȡ�м�ֵ 30us
}

/*------------------------------------------------
              ���հ�λ������
------------------------------------------------*/
unsigned char DHT11_rec_byte()      //����һ���ֽ�
{
  unsigned char i,dat=0;
  for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
   {          
      while(DHT11_Data);   //�ȴ�����͵�ƽ
      while(!DHT11_Data);   //�ȴ�50us�͵�ƽ��ȥ
				DHT11_delay_us(8);     //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0 
      dat<<=1;//��λ����λ���㣩ʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
      if(DHT11_Data==1)    //����Ϊ1ʱ��ʹdat��1����������1
        dat+=1;
      while(DHT11_Data);  //�ȴ�����������    
   }  
    return dat;
}


/*------------------------------------------------
              ����40bit���ݣ��������ʪ�ȣ�
1.�����Ȱ�data�����ߣ�io����Ϊ���룩��
2.�ӻ���data�����ͣ�������ȡdata�ߵ�ƽ��ֱ���͵�ƽ��������Լ50us��
  �ӻ�����data�ߺ��ӳ�40us���ң�28~70us֮�䣩�����ٴζ�ȡdata�ߵ�ƽ�����Ϊ�͵�ƽ����Ϊ��0�������Ϊ�ߵ�ƽ����Ϊ��1����
3.�����ظ�����1,2�����ۼ�40�Ρ�
------------------------------------------------*/

void DHT11_receive()      //����40λ������
{
    unsigned char R_H,R_L,T_L,T_H,revise; 
    DHT11_start();//������ʼ�źţ�
    if(DHT11_Data==0)
    {
        while(DHT11_Data==0);   //�ȴ�����     
        DHT11_delay_us(40);  //���ߺ���ʱ80us
			
        R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
        R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ  
        T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
        T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ
        revise=DHT11_rec_byte(); //����У��λ

        DHT11_delay_us(25);    //����

        if((R_H+R_L+T_H+T_L)==revise)      //���һ�ֽ�ΪУ��λ��У���Ƿ���ȷ
        {
					//ʪ��
					rec_dat[0]=R_H;
					rec_dat[1]=R_L;	
					//�¶�
					rec_dat[4]=T_H;
					rec_dat[5]=T_L; 	
					
        } 
				
				

				
				
//        /*���ݴ���ת��Ϊ�ַ���������ʾ*/
//			  //ʪ��
//				rec_dat[0]='0'+(RH/10);
//        rec_dat[1]='0'+(RH%10);
//				rec_dat[2]=' ';
//				rec_dat[3]=' ';
//				
//				//�¶�
//        rec_dat[4]='0'+(TH/10);
//        rec_dat[5]='0'+(TH%10); 
//		    rec_dat[6]=' ';
			
    }

}
