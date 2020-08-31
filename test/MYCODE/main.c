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
#include "step.h"

filter_avg_t filter;
peak_value_t peak;
slid_reg_t slid;
axis_info_t sample;

extern int16_t step_cnt;

int main(void)
{
    int16_t temp = step_cnt;
    Delay_Init();
    Usart1_Init();
    Led_Init();
    //max30102_init();
    OLED_Init();
    InitMPU6050();
    Check_MPU6050();//У׼

	printf("start!\n");
	
    while (1)
    {
        get_4_gyr_data(&filter);
		//get_4_acc_data(&filter);//不太准，还是要用陀螺仪
        filter_calculate(&filter, &sample);
        peak_update(&peak, &sample);
        slid_update(&slid, &sample);
        detect_step(&peak, &slid, &sample);

        if (temp != step_cnt)
        {
            printf("%d\n", step_cnt);
            temp = step_cnt;
        }
        delay_ms(40);

    }
}
