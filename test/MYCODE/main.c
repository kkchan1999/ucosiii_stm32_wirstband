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


float Acc[3] = {0};
float Gyr[3] = {0};
const float fRad2Deg = 57.295779513f; //弧度换算角度乘的系数
const float dt = 0.005; //时间周期
float angle[3] = {0};
float angle_dot[3] = {0};
float R = 0.98f;

void ImuCalculate_Complementary(void)//计算角度
{
    u8 i;
    static float angle_last[3] = {0};
    float temp[3] = {0};
    temp[0] = sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2]);
    temp[1] = sqrt(Acc[0] * Acc[0] + Acc[2] * Acc[2]);

    for (i = 0; i < 2; i++) //pitch and roll
    {
        angle[i] = R * (angle_last[i] + Gyr[i] * dt)
                   + (1 - R) * fRad2Deg * atan(Acc[i] / temp[i]);
        angle_last[i] = angle[i];
    }

    angle[2] = angle_last[2] + Gyr[2] * dt; //yaw
    angle_last[2] = angle[2];
}


int main(void)
{
    u32 i = 0;
    int16_t acc[3] = {0};
    int16_t gyr[3] = {0};
    int32_t avg[6] = {0};


    Delay_Init();
    Led_Init();
    Usart1_Init();
    //IIC_Init();//MPU6050的初始化函数里面包含了这个，所以不用了
    InitMPU6050();
    OLED_Init();

    delay_ms(500);


    //下面的步骤是校准传感器，要弄个函数封装一下
    GetData(ACCEL_XOUT_H);
    GetData(ACCEL_YOUT_H);
    GetData(ACCEL_ZOUT_H);
    GetData(GYRO_XOUT_H);
    GetData(GYRO_YOUT_H);
    GetData(GYRO_ZOUT_H);
    for (i = 0; i < 2000; i++)
    {
        acc[0] = GetData(ACCEL_XOUT_H)   ;
        acc[1] = GetData(ACCEL_YOUT_H)   ;
        acc[2] = GetData(ACCEL_ZOUT_H)   ;
        gyr[0] = GetData(GYRO_XOUT_H)    ;
        gyr[1] = GetData(GYRO_YOUT_H)    ;
        gyr[2] = GetData(GYRO_ZOUT_H)    ;

        avg[0] += acc[0];
        avg[1] += acc[1];
        avg[2] += acc[2];
        avg[3] += gyr[0];
        avg[4] += gyr[1];
        avg[5] += gyr[2];
        delay_ms(1);
    }
    avg[0] /= 2000;
    avg[1] /= 2000;
    avg[2] /= 2000;
    avg[3] /= 2000;
    avg[4] /= 2000;
    avg[5] /= 2000;

    i = 0;
    while (1)
    {
        acc[0]  = GetData(ACCEL_XOUT_H) - avg[0];
        acc[1]  = GetData(ACCEL_YOUT_H) - avg[1];
        acc[2]  = GetData(ACCEL_ZOUT_H) - avg[2] + 8192;
        gyr[0]  = GetData(GYRO_XOUT_H)  - avg[3];
        gyr[1]  = GetData(GYRO_YOUT_H)  - avg[4];
        gyr[2]  = GetData(GYRO_ZOUT_H)  - avg[5];

        for (int i = 0; i < 3; i++)
        {
            Acc[i] = acc[i];
            Gyr[i] = gyr[i];
        }

        for (int i = 0; i < 3; i++)
        {
            Acc[i] /= 8192.0f;
            Gyr[i] /= 16.384f;
        }

        for (int i = 0; i < 3; i++)
        {
            kalman_filter(Acc[i], Gyr[i], &angle[i], &angle_dot[i]);
        }
        //ImuCalculate_Complementary();
        //kalman_filter(Acc[i],Gyr[i],&angle[i],&angle_dot[i]);


        printf("acc:  X=%d   Y=%d   Z=%d  \n", acc[0], acc[1], acc[2]);
        printf("gyro:  X=%d   Y=%d   Z=%d  \n", gyr[0], gyr[1], gyr[2]);
//

//        printf("ACC:  X=%f   Y=%f   Z=%f  \n", Acc[0], Acc[1], Acc[2]);
//        printf("GYRO:  X=%f   Y=%f   Z=%f  \n", Gyr[0], Gyr[1], Gyr[2]);
        printf("angle:  X=%f   Y=%f   Z=%f  \n", angle[0], angle[1], angle[2]);
        printf("angle dot:  X=%f   Y=%f   Z=%f  \n", angle_dot[0], angle_dot[1], angle_dot[2]);
        printf("%d\n\n\n\n\n\n\n\n", i++);

        PFout(9) = !PFout(9);

        delay_ms(500);

        if (i % 30 == 0)
        {
            OLED_Fill(0xFF);//全屏点亮
            delay_s(2);
            OLED_Fill(0x00);//全屏灭
            delay_s(2);
            for (i = 0; i < 6; i++)
            {
                OLED_ShowCN(22 + i * 16, 0, i); //测试显示中文
            }
            delay_s(2);
            OLED_ShowStr(0, 3, "HelTec Automation", 1); //测试6*8字符
            OLED_ShowStr(0, 4, "Hello Tech", 2);            //测试8*16字符
            delay_s(2);
            OLED_CLS();//清屏
            OLED_OFF();//测试OLED休眠
            delay_s(2);
            OLED_ON();//测试OLED休眠后唤醒
            OLED_DrawBMP(0, 0, 128, 8, (unsigned char *)BMP1);
            delay_s(2);
        }

    }
}
