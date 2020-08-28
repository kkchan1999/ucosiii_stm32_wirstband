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

int main(void)
{
    Delay_Init();
    Led_Init();
    Usart1_Init();
    Rtc_Init();
    OLED_Init();
	max30102_init();
    while (1)
    {
		delay_ms(500);
		printf("123\n");
    }
}
