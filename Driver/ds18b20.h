#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
   	
//�����Ҫ�޸����ţ�ֻ���޸�����ĺ�
#define RCC_DS18B20_PORT 	    RCC_APB2Periph_GPIOA		/* GPIO�˿�ʱ�� */
#define DS18B20_GPIO_PIN      GPIO_Pin_8
#define DS18B20_GPIO_PORT     GPIOA

//IO�������ã�CRL�Ĵ�����Ӧ����0~7,CRH�Ĵ�����Ӧ����8~15��
//DS18B20_GPIO_PORT->CRH&=0xFFFFFFF0ΪPA8�������ģʽ��Ӧ�ļĴ������
//DS18B20_GPIO_PORT->CRH|=0x00000008��(CNF8[1:0]����Ϊ10:����/��������ģʽ��MODE8[1;0]����Ϊ00:����ģʽ)
//DS18B20_GPIO_PORT->CRH|=0x00000003��(CNF8[1:0]����Ϊ00:ͨ���������ģʽ ��MODE8[1;0]����Ϊ11:���50MHZ)
#define DS18B20_IO_IN()  {DS18B20_GPIO_PORT->CRH&=0xFFFFFFF0;DS18B20_GPIO_PORT->CRH|=0x00000008;}
#define DS18B20_IO_OUT() {DS18B20_GPIO_PORT->CRH&=0xFFFFFFF0;DS18B20_GPIO_PORT->CRH|=0x00000003;}

#define DS18B20_OUT_0   GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)//IOΪ�͵�ƽ
#define DS18B20_OUT_1   GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)//IOΪ�ߵ�ƽ
#define READ_DS18B20_IO GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN)//��ȡIO��ƽ
		
void DS18B20_GPIO_Init(void);
u8 DS18B20_Init(void);			//��ʼ��DS18B20
float ReadTemperature(void); //��ȡ�¶�ֵ

#endif

















