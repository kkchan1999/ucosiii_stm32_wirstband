#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"


#define OLED_SCL         PEout(8)
#define OLED_SDA_OUT     PEout(10)
#define OLED_SDA_IN      PEin(10)

#define OLED_ADDRESS    0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);//��ʼ��
void OLED_SetPos(unsigned char x, unsigned char y);//������ʼ������
void OLED_Fill(unsigned char fill_Data);//ȫ�����
void OLED_CLS(void);//����
void OLED_ON(void);//��
void OLED_OFF(void);//��
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);//��ʾASCII�ַ�
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);//��ʾ����
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);//��ͼ
void OLED_ShowBigNum(unsigned char x, unsigned char y, unsigned char num);//�������Ƶ����֣���������������
#endif
