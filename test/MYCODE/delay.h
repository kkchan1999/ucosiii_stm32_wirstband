#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f4xx.h"

void Delay_Init(void);


void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_s(int n);

#endif
