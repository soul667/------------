#include "ds18b20.h"
#include "delay.h"	

/*******************************************************************************
��������DS18B20_GPIO_Init
���ܣ���ʼ��DS18B20����
���룺
�����
����ֵ��
��ע��
*******************************************************************************/
void DS18B20_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_DS18B20_PORT, ENABLE);	 //ʹ��PORTA��ʱ�� 
		
		GPIO_InitStructure.GPIO_Pin = DS18B20_GPIO_PIN;				
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);

		GPIO_SetBits(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN);    //���1
}

/*******************************************************************************
��������DS18B20_Init
���ܣ���ʼ��DS18B20
���룺
�����
����ֵ����ʼ���ɹ�Ϊ0�����ɹ�Ϊ1
��ע��
*******************************************************************************/
u8 DS18B20_Init(void)
{
	  unsigned char wait=0;
	    
		DS18B20_IO_OUT(); //���ģʽ
	  DS18B20_OUT_0;     //����
	  delay_us(750);     //������ʱ480us
	  DS18B20_OUT_1;     //���� 
	  delay_us(15);      //15us
	  DS18B20_IO_IN();  //����ģʽ
	  while(READ_DS18B20_IO && wait++<200)delay_us(1);//�ȴ��ߵ�ƽ����
	  if(wait>=200)return 1;
	  else wait=0;
	  while(!READ_DS18B20_IO && wait++<240)delay_us(1);//�ȴ��͵�ƽ����
	  if(wait>=240)return 1;
	  else return 0; 
}

/*******************************************************************************
��������DS18B20_ReadByte
���ܣ���DS18B20��һ���ֽ�
���룺
�����
����ֵ����ȡ�����ֽ�
��ע��
*******************************************************************************/
unsigned char DS18B20_ReadByte(void)
{
		unsigned char i;
    unsigned char dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;                 
			  DS18B20_IO_OUT();           //���ģʽ
        DS18B20_OUT_0;              //��ʼʱ��Ƭ
        delay_us(2);                //��ʱ�ȴ�
        DS18B20_OUT_1;              //׼������
			  DS18B20_IO_IN();            //����ģʽ
        delay_us(12);                //������ʱ
        if(READ_DS18B20_IO) dat |= 0x80;   //��ȡ���� 
        delay_us(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/*******************************************************************************
��������DS18B20_WriteByte
���ܣ�дһ���ֽ�
���룺unsigned char dat
�����
����ֵ��
��ע��
*******************************************************************************/
void DS18B20_WriteByte(unsigned char dat)
{
	  unsigned char i;
	  unsigned char temp;
	
		DS18B20_IO_OUT();//���ģʽ
    for (i=1; i<=8; i++) 
		{
					temp = dat & 0x01;  
					dat  = dat >> 1;
					if (temp) 
					{
							DS18B20_OUT_0;
							delay_us(2);                            
							DS18B20_OUT_1; //д1
							delay_us(60);             
					}
					else 
					{
							DS18B20_OUT_0;//д0
							delay_us(60);             
							DS18B20_OUT_1;
							delay_us(2);                          
					}		
		}
}

/**************************************
��DS18B20�л�ȡ�¶�ֵ�ø���ֵ
����: ��
����ֵ:  ��ȡ�����¶�ֵ(��Ч��Χ-55.0~125.0)
**************************************/
float ReadTemperature(void)
{
    unsigned char TPH;							//����¶�ֵ�ĸ��ֽ�
    unsigned char TPL;							//����¶�ֵ�ĵ��ֽ�
    short i16=0;
    float f32=0;

	  DS18B20_Init();
    DS18B20_WriteByte(0xCC);  //����ROM����
    DS18B20_WriteByte(0x44);   //��ʼת������
    DS18B20_Init();
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0xBE);        //���ݴ�洢������
    TPL = DS18B20_ReadByte();       //���¶ȵ��ֽ�
    TPH = DS18B20_ReadByte();       //���¶ȸ��ֽ�
    i16 = 0;
    i16 = (TPH<<8) |TPL;				// ����λ(MSB)���λ(LSB)�ϲ�
    f32 = i16 * 0.0625;				// 12bit����ʱ�¶�ֵ����
    return(f32);					// ���ض�ȡ�����¶���ֵ(float��)
}


