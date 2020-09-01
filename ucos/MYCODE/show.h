#ifndef __SHOW_H
#define __SHOW_H
#include "stm32f4xx.h"
#include "rtc.h"
#include "oled.h"

void ShowTime(unsigned char x, unsigned char y, u8 mode);
void ShowDate(unsigned char x, unsigned char y, u8 mode);

#endif
