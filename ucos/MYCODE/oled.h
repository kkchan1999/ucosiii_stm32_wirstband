#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"


#define OLED_SCL         PEout(8)
#define OLED_SDA_OUT     PEout(10)
#define OLED_SDA_IN      PEin(10)

#define OLED_ADDRESS    0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);//初始化
void OLED_SetPos(unsigned char x, unsigned char y);//设置起始点坐标
void OLED_Fill(unsigned char fill_Data);//全屏填充
void OLED_CLS(void);//清屏
void OLED_ON(void);//开
void OLED_OFF(void);//关
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);//显示ASCII字符
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);//显示中文
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);//画图
void OLED_ShowBigNum(unsigned char x, unsigned char y, unsigned char num);//画出定制的数字，可以用来清屏的
#endif
