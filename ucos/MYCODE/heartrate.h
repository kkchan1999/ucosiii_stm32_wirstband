#ifndef __HEARTRATE_H
#define __HEARTRATE_H

#include "stm32f4xx.h"
#include "max30102.h"
#include "algorithm.h"
#include "oled.h"
#include "delay.h"
#include "usart.h"

void HR_init(void);
u8 check_heart_rate(void);
int16_t Show_HR(void);
#endif


