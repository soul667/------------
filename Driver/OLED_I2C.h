#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS	0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void bsp_InitI2C_2(void);
void i2c_Start_2(void);
void i2c_Stop_2(void);
void i2c_SendByte_2(uint8_t _ucByte);
uint8_t i2c_ReadByte_2(void);
uint8_t i2c_WaitAck_2(void);
void i2c_Ack_2(void);
void i2c_NAck_2(void);

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char TextSize,u8 mode);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize,u8 mode);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N,u8 mode);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ShowCentigrade(unsigned char x, unsigned char y);

#endif

