#include "tdswater.h"                  
#include "adc.h"
#include "stdio.h"

unsigned int ADC_value;
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float temp_data=0.0;
float compensationCoefficient=1.0;//�¶�У׼ϵ��
float compensationVolatge;
float kValue=1.0;
//��ֵ�˲��㷨
int middleValueFilter(int N,unsigned int* val)
{
    int value_buf[N];
    int i,j,k,temp;
    for( i = 0; i < N; ++i)
    {
        value_buf[i] = *val;	
				
    }
    for(j = 0 ; j < N-1; ++j)
    {
        for(k = 0; k < N-j-1; ++k)
        {
            //��С��������ð�ݷ�����
            if(value_buf[k] > value_buf[k+1])
            {
                temp = value_buf[k];
                value_buf[k] = value_buf[k+1];
                value_buf[k+1] = temp;
            }
        }
    }
		
    return value_buf[(N-1)/2];
}

unsigned int ReadVal(void)
{
	HAL_ADC_Start(&hadc1);						//����ADC1��������whileѭ����
	ADC_value=HAL_ADC_GetValue(&hadc1);			//��ȡADC1����ֵ	
//	return middleValueFilter(50,&ADC_value);
	return ADC_value;
}

float GetTds(void)
{
	TDS_voltage =(float)ReadVal()/4096*3.3; // ��ȡת����ADֵ
	
	compensationCoefficient=1.0+0.02*((temp_data/10)-25.0); 
	compensationVolatge=TDS_voltage/compensationCoefficient;
	TDS_value=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 
	255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue;
	
	  if((TDS_value<=0)){TDS_value=0;}
		if((TDS_value>1400)){TDS_value=1400;}
		return TDS_value;
}
