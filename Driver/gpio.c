#include "gpio.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������GPIO����								  
////////////////////////////////////////////////////////////////////////////////// 	   

void KEY_GPIO_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PABC�˿�ʱ��
   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  //�ر�JTAGģʽ ʹPB3��PB4�����ͨIO��
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				 // �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	 GPIO_Init(GPIOB, &GPIO_InitStructure);			

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);		
   GPIO_ResetBits(GPIOB,GPIO_Pin_6);    //���0	
}




