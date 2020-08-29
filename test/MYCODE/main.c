#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "iic.h"
#include "MPU6050.h"
#include "math.h"
#include "oled.h"
#include "rtc.h"
#include "show.h"

#include "algorithm.h"
#include "max30102.h"
#include "heartrate.h"

int main(void)
{
    int16_t HR;
    Delay_Init();
    Usart1_Init();
    max30102_init();
    OLED_Init();
//    HR_init();
    while (1)
    {
        HR = Show_HR();
        OLED_CLS();
        OLED_ShowBigNum(0, 0, HR / 100);
        OLED_ShowBigNum(16, 0, (HR % 100) / 10);
        OLED_ShowBigNum(32, 0, (HR % 10));
        OLED_ShowBigNum(48, 0, 10);
        delay_s(1);
    }
}
