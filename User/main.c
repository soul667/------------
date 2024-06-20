#include "sys.h"
#include "delay.h"
#include "gpio.h"
#include "OLED_I2C.h"
#include "ds18b20.h"
#include "usart1.h"
#include "usart3.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STM32_RX1_BUF       Usart1RecBuf 
#define STM32_Rx1Counter    RxCounter
#define STM32_RX1BUFF_SIZE  USART1_RXBUFF_SIZE

#define RATIO  4.51/4.08

short temperature=0;  //�¶�
u8    tempSetVal=40;  //�¶�����
u8 shuaxin = 0;  //ˢ�±�־
u8 setFlag = 0;  //���ñ�־
unsigned char tdsBuf[12];
unsigned char cmd[7] = {0xA0,0x00,0x00,0x00,0x00,0xA0};
unsigned int tds_value=0;
unsigned int tds_Max = 300;       //TDS����
float PH  = 0.0;
u16 Ph_min = 300,Ph_max = 900;   //PH��������
u16 Turbidity=0;
u16 TurSetMax=1000;  //�Ƕ�����
unsigned long int avgValue;	//Store the average value of the sensor feedback 

char display[16];

void Usart1RxBufClear(void) //������ڽ��ջ���
{
	  memset(STM32_RX1_BUF, 0, STM32_RX1BUFF_SIZE);//�������
		STM32_Rx1Counter = 0; 
}

void InitDisplay(void)   //��ʼ����ʾ
{
	  unsigned char i=0;
	  
	  OLED_ShowStr(0, 0, "PH:", 2,0);
	  OLED_ShowStr(0, 2, "TDS:", 2,0);
	  for(i=0;i<2;i++)OLED_ShowCN(i*16,4,i+0,0);//��ʾ���ģ��¶�
	  for(i=0;i<2;i++)OLED_ShowCN(i*16,6,i+2,0);//��ʾ���ģ��Ƕ�
	  OLED_ShowChar(32,4,':',2,0);
	  OLED_ShowChar(32,6,':',2,0);
}

void displaySetValue(void)  //��ʾ���õ�ֵ
{
	  if(setFlag == 1 ||setFlag == 2)
		{
				sprintf(display,"%5.2f",(float)Ph_min/100); 
				OLED_ShowStr(40, 4,(u8 *)display, 2,setFlag+1-1);
			
				sprintf(display,"%5.2f",(float)Ph_max/100); 
				OLED_ShowStr(40, 6,(u8 *)display, 2,setFlag+1-2);
		}
		if(setFlag == 3)
		{
				sprintf(display,"%04d",tds_Max); 
				OLED_ShowStr(40, 4,(u8 *)display, 2,setFlag+1-3);
		}
		if(setFlag == 4)
		{
				sprintf(display,"%02d",tempSetVal); 
				OLED_ShowStr(40, 4,(u8 *)display, 2,setFlag+1-4);
		}
		if(setFlag == 5)
		{
				sprintf(display,"%04d",TurSetMax); 
				OLED_ShowStr(40, 4,(u8 *)display, 2,setFlag+1-5);
		}
}

void keyscan(void)   //����ɨ��
{
	 unsigned char i=0;
	
	 if(KEY1 == 0) //���ü�
	 {
			delay_ms(20);
		  if(KEY1 == 0)
			{
					while(KEY1 == 0);
				  BEEP=0;
				  setFlag ++;
				  if(setFlag == 1)
					{
							OLED_CLS();    //����
						  for(i=0;i<2;i++)OLED_ShowCN(i*16+32,0,i+8,0);//��ʾ���ģ�����
						  OLED_ShowStr(62, 0, " PH", 2,0);
						  for(i=0;i<2;i++)OLED_ShowCN(i*16,4,i+4,0);//��ʾ���ģ�����
							for(i=0;i<2;i++)OLED_ShowCN(i*16,6,i+6,0);//��ʾ���ģ�����
						  OLED_ShowChar(32,4,':',2,0);
						  OLED_ShowChar(32,6,':',2,0);
						  
					}
					if(setFlag == 3)
					{
							OLED_ShowStr(62, 0, " TDS", 2,0);
						  OLED_ShowStr(0, 4, "                ", 2,0);
						  for(i=0;i<2;i++)OLED_ShowCN(i*16,4,i+6,0);//��ʾ���ģ�����
						  OLED_ShowChar(32,4,':',2,0);
						  OLED_ShowStr(0, 6, "                ", 2,0);
					}
					if(setFlag == 4)
					{
							for(i=0;i<2;i++)OLED_ShowCN(i*16+64,0,i+0,0);//��ʾ���ģ��¶�
						  OLED_ShowStr(56, 4, "  ", 2,0);
						  OLED_ShowCentigrade(56, 4);    //��
						  
					}
					if(setFlag == 5)
					{
							for(i=0;i<2;i++)OLED_ShowCN(i*16+64,0,i+2,0);//��ʾ���ģ��Ƕ�
						  OLED_ShowStr(72, 4, "NTU", 2,0);
					}
					if(setFlag >= 6)
					{
						  setFlag = 0;
							OLED_CLS();    //����
						  InitDisplay();
						  
					}
					displaySetValue();
			}
	 }
	 if(KEY2 == 0) //�Ӽ�
	 {
			delay_ms(100);
		  if(KEY2 == 0)
			{
					if(setFlag == 1)
					{
						  if(Ph_max-Ph_min > 10)Ph_min+=10;
					}
					if(setFlag == 2)
					{
							if(Ph_max < 1400)Ph_max+=10;
					}
					if(setFlag == 3)
					{
							if(tds_Max<2000)tds_Max+=10;
					}
					if(setFlag == 4)
					{
							if(tempSetVal<99)tempSetVal++;
					}
					if(setFlag == 5)
					{
						  if(TurSetMax<3000)TurSetMax+=10;
					}
					displaySetValue();   //��ʾû������ֵ
			}
	 }
	 if(KEY3 == 0) //����
	 {
			delay_ms(100);
		  if(KEY3 == 0)
			{
					if(setFlag == 1)
					{
						  if(Ph_min >= 10)Ph_min-=10;
					}
					if(setFlag == 2)
					{
							if(Ph_max-Ph_min > 10)Ph_max-=10;
					}
					if(setFlag == 3)
					{
							if(tds_Max >= 10)tds_Max-=10;
					}
					if(setFlag == 4)
					{
							if(tempSetVal>0)tempSetVal--;
					}
					if(setFlag == 5)
					{
						  if(TurSetMax>=10)TurSetMax-=10;
					}
					displaySetValue();   //��ʾû������ֵ
			}
	 }
}

void Get_TDS(void)     //��ȡTDS
{
	  char i = 0;
	
	  uart1_send(cmd,6);
	
	  delay_ms(20);
	
		if(STM32_RX1_BUF[STM32_Rx1Counter] == 0xAA && STM32_RX1_BUF[STM32_Rx1Counter+6] == 0xAB)
		{
			  for(i = 0; i<12; i++)
        {
            tdsBuf[i] = STM32_RX1_BUF[STM32_Rx1Counter+i];
        }
			
			  tds_value = (unsigned int)(tdsBuf[1]<<8|tdsBuf[2]);

				Usart1RxBufClear();    //��ս��ջ�����
		}
}

void Get_PH(void)    //��ȡPH
{
	  u16 buf[10];//buffer for read analog
	  u8 i,j;	
	  float phValue=0.0;
	
		for(i=0;i<10;i++)	//Get 10 sample value from the sensor for smooth the value
		{
			buf[i]=Get_Adc_Average(ADC_Channel_8,10); 
		}
		for(i=0;i<9;i++)	//sort the analog from small to large
		{
				for(j=i+1;j<10;j++)
				{
						if(buf[i]>buf[j])
						{
							int temp=buf[i];
							buf[i]=buf[j]; 
							buf[j]=temp;
						}
				}
		}
		avgValue=0;
		for(i=2;i<8;i++)	//take the average value of 6 center sample 
		avgValue+=buf[i];
    phValue=((float)avgValue*5.0/4095/6)*RATIO; //convert the analog into millivolt
		PH=(phValue*(-5.290))+23.053;	//convert the millivolt into pH value 
		if(PH<0)PH=0;
		if(PH>14.0)PH=14.0;
}

void Get_Turbidity(void)   //��ȡ���Ƕ�
{
		float T;
	  u16 adcx = 0;
	
	  adcx = Get_Adc_Average(ADC_Channel_9,20);//��ȡADֵ
		T = adcx;
		T = T*(3.3/4096)+1.72;
		if(T < 2.5)
		{
				T = 3000;	
		}
		else 
		{
				T = (-1120.4*T*T+5742.3*T-4352.9);	//Tul��ADֵ
		}
		if(T < 0)
		{
			T = 0;
		}
		Turbidity = (u16)T;
		
		if(Turbidity > 3000)Turbidity = 3000;
}

void UsartSendData(void)    //���ڷ�������
{
	  char SEND_BUF[50];
	
	  sprintf(SEND_BUF,"PH:%5.2f\r\n",(float)PH); 
	  Uart3_SendStr(SEND_BUF);
	
	  sprintf(SEND_BUF,"TDS:%d\r\n",tds_value);
	  Uart3_SendStr(SEND_BUF);
	
	  sprintf(SEND_BUF,"�¶�:%d��\r\n",temperature);
	  Uart3_SendStr(SEND_BUF);
	
	  sprintf(SEND_BUF,"�Ƕ�:%dNTU\r\n",Turbidity);
	  Uart3_SendStr(SEND_BUF);
	
	  Uart3_SendStr("\r\n");
}

int main(void)
{
	  u16 timeCount1 = 300;
	  u16 timeCount2 = 10;
	  u8 shanshuo=0;
	
		delay_init();	           //��ʱ������ʼ��	 
    NVIC_Configuration();	   //�ж����ȼ�����
	  delay_ms(300); 
	  I2C_Configuration();     //IIC��ʼ��
	  Adc_Init();		  	      	//ADC��ʼ��	
	  OLED_Init();             //OLEDҺ����ʼ��
	  OLED_CLS();              //����
	  InitDisplay();
		KEY_GPIO_Init();        //�������ų�ʼ��
	  DS18B20_GPIO_Init();
	  DS18B20_Init();         //��ʼ����ʾ
	  uart1_Init(9600);
    USART3_Init(9600); 
		while(1)
		{ 
			   keyscan();  //����ɨ��
         timeCount1 ++;
			   if(timeCount1 >= 300 && !setFlag)   //��ʱһ��ʱ���ȡ
				 {
						timeCount1 = 0;
					 
					  shanshuo=!shanshuo;
					 
					  Get_PH() ;      //��ȡPH
					  /*���޵�ʱ����˸��ʾ*/
					  if(((PH*100)<=Ph_min || (PH*100)>=Ph_max) && shanshuo)
						{
								OLED_ShowStr(32, 0,"     ", 2,0);
						}
						else
						{
								sprintf(display,"%5.2f ",(float)PH); 
								OLED_ShowStr(32, 0,(u8 *)display, 2,0);
						}
					 
					  Get_TDS() ;      //��ȡTDS
						/*���޵�ʱ����˸��ʾ*/
						if((tds_value>=tds_Max) && shanshuo)
						{
								OLED_ShowStr(40, 2,"    ", 2,0);
						}
						else
						{
								sprintf(display,"%d   ",tds_value); 
								OLED_ShowStr(40, 2,(u8 *)display, 2,0);
						}
					 
					  temperature = ReadTemperature();//��ȡ�¶�
						/*���޵�ʱ����˸��ʾ*/
						if((temperature>=tempSetVal) && shanshuo)
						{
								OLED_ShowStr(40, 4,"    ", 2,0);
						}
						else
						{
								sprintf(display,"%02d",temperature); 
								OLED_ShowStr(40, 4,(u8 *)display, 2,0);
							  OLED_ShowCentigrade(56, 4);    //��
						}
					 
					  Get_Turbidity() ;
						/*���޵�ʱ����˸��ʾ*/
						if((Turbidity>=TurSetMax) && shanshuo)
						{
								OLED_ShowStr(40, 6,"       ", 2,0);
						}
						else
						{
								sprintf(display,"%dNTU   ",Turbidity); 
								OLED_ShowStr(40, 6,(u8 *)display, 2,0);
						}
						
						if(((PH*100)<=Ph_min || (PH*100)>=Ph_max)||(tds_value>=tds_Max)||(temperature>=tempSetVal)||(Turbidity>=TurSetMax))BEEP= 1;else BEEP=0;  //���޷��������� 
						timeCount2 ++;
						if(timeCount2 >= 2)
						{
								timeCount2 = 0;
							  UsartSendData();     //���ڷ�������
						}
				 }
			   delay_ms(1);
		}
}


