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
    //IIC_Init();//MPU6050�ĳ�ʼ�����������������������Բ�����
    //InitMPU6050();
    //Check_MPU6050();//У׼������

    OLED_Init();
    delay_ms(500);

	OLED_Fill(0xFF);//ȫ������
    delay_s(1);
	OLED_Fill(0x00);//ȫ����
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
        //OLED_CLS();//����
//        for (int i = 0; i < 6; i++)
//        {
//            OLED_ShowCN(i * 16, 0, 1); //������ʾ����
//        }
//        delay_s(2);
//        OLED_ShowStr(0, 3, "This is my car", 1); //����6*8�ַ�
//        OLED_ShowStr(0, 4, "Hello Tech", 2);            //����8*16�ַ�
//        delay_s(2);
//        OLED_CLS();//����
//        OLED_OFF();//����OLED����
//        delay_s(2);
//        OLED_ON();//����OLED���ߺ���
//        OLED_DrawBMP(0, 0, 128, 8, (unsigned char *)BMP1);
//        delay_s(2);


    }
}
