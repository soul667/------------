#ifndef __usart3_H
#define __usart3_H	 
#include "stm32f10x.h"
#include "stm32f10x_usart.h"

#define USART3_RXBUFF_SIZE   80 

extern unsigned int Rx3Counter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart3RecBuf[USART3_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���

void USART3_Init(u32 baud);
void USART3_Sned_Char(u8 temp);
void Uart3_SendStr(char*SendBuf);
void uart3_send(unsigned char *bufs,unsigned char len);

#endif

