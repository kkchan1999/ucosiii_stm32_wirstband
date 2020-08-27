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

int main(void)
{
    u32 i = 0;

    extern const unsigned char BMP1[];



    Delay_Init();
    Led_Init();
    Usart1_Init();
    Rtc_Init();
    //IIC_Init();//MPU6050的初始化函数里面包含了这个，所以不用了
    //InitMPU6050();
    //Check_MPU6050();//校准传感器

    OLED_Init();
    delay_ms(500);

	OLED_Fill(0xFF);//全屏点亮
    delay_s(1);
	OLED_Fill(0x00);//全屏灭
    while (1)
    {
        //Get_MPU6050_Data();
        //printf("%d\n\n\n\n\n\n\n\n", i++);

        PFout(9) = !PFout(9);


        
        delay_s(1);
		ShowDate(0, 0);
        ShowTime(0, 4);
		
        //delay_s(1);
		//OLED_ShowBigNum(16*6,4,12);
        //OLED_CLS();//清屏
//        for (int i = 0; i < 6; i++)
//        {
//            OLED_ShowCN(i * 16, 0, 1); //测试显示中文
//        }
//        delay_s(2);
//        OLED_ShowStr(0, 3, "This is my car", 1); //测试6*8字符
//        OLED_ShowStr(0, 4, "Hello Tech", 2);            //测试8*16字符
//        delay_s(2);
//        OLED_CLS();//清屏
//        OLED_OFF();//测试OLED休眠
//        delay_s(2);
//        OLED_ON();//测试OLED休眠后唤醒
//        OLED_DrawBMP(0, 0, 128, 8, (unsigned char *)BMP1);
//        delay_s(2);


    }
}
