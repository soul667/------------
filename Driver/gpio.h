#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define KEY1 PBin(12)
#define KEY2 PBin(13)
#define KEY3 PBin(14)

#define BEEP   PBout(6)

void KEY_GPIO_Init(void);//���ų�ʼ��

#endif
