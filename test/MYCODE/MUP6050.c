#include "MPU6050.h"
#include "iic.h"
#include "math.h"

int16_t acc[3] = {0};
int16_t gyr[3] = {0};
int32_t avg[6] = {0};
float Acc[3] = {0};
float Gyr[3] = {0};
const float fRad2Deg = 57.295779513f; //弧度换算角度乘的系数
const float dt = 0.005;               //时间周期
float angle[3] = {0};
float angle_dot[3] = {0};
float R = 0.98f;

void Check_MPU6050(void)
{
    int16_t i;
    GetData(ACCEL_XOUT_H);
    GetData(ACCEL_YOUT_H);
    GetData(ACCEL_ZOUT_H);
    GetData(GYRO_XOUT_H);
    GetData(GYRO_YOUT_H);
    GetData(GYRO_ZOUT_H);
    for (i = 0; i < 2000; i++)
    {
        acc[0] = GetData(ACCEL_XOUT_H);
        acc[1] = GetData(ACCEL_YOUT_H);
        acc[2] = GetData(ACCEL_ZOUT_H);
        gyr[0] = GetData(GYRO_XOUT_H);
        gyr[1] = GetData(GYRO_YOUT_H);
        gyr[2] = GetData(GYRO_ZOUT_H);

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
}

void ImuCalculate_Complementary(void) //计算角度
{
    u8 i;
    static float angle_last[3] = {0};
    float temp[3] = {0};
    temp[0] = sqrt(Acc[1] * Acc[1] + Acc[2] * Acc[2]);
    temp[1] = sqrt(Acc[0] * Acc[0] + Acc[2] * Acc[2]);

    for (i = 0; i < 2; i++) //pitch and roll
    {
        angle[i] = R * (angle_last[i] + Gyr[i] * dt) + (1 - R) * fRad2Deg * atan(Acc[i] / temp[i]);
        angle_last[i] = angle[i];
    }

    angle[2] = angle_last[2] + Gyr[2] * dt; //yaw
    angle_last[2] = angle[2];
}

void Get_MPU6050_Data(void)
{
    acc[0] = GetData(ACCEL_XOUT_H) - avg[0];
    acc[1] = GetData(ACCEL_YOUT_H) - avg[1];
    acc[2] = GetData(ACCEL_ZOUT_H) - avg[2] + 8192;
    gyr[0] = GetData(GYRO_XOUT_H) - avg[3];
    gyr[1] = GetData(GYRO_YOUT_H) - avg[4];
    gyr[2] = GetData(GYRO_ZOUT_H) - avg[5];

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

    printf("acc:  X=%d   Y=%d   Z=%d  \n", acc[0], acc[1], acc[2]);
    printf("gyro:  X=%d   Y=%d   Z=%d  \n", gyr[0], gyr[1], gyr[2]);

    printf("ACC:  X=%f   Y=%f   Z=%f  \n", Acc[0], Acc[1], Acc[2]);
    printf("GYRO:  X=%f   Y=%f   Z=%f  \n", Gyr[0], Gyr[1], Gyr[2]);
    printf("angle:  X=%f   Y=%f   Z=%f  \n", angle[0], angle[1], angle[2]);
    printf("angle dot:  X=%f   Y=%f   Z=%f  \n", angle_dot[0], angle_dot[1], angle_dot[2]);
}

void get_4_gyr_data(filter_avg_t *filter)
{
    u8 i;
    for (i = 0; i < FILTER_CNT; i++)
    {
        delay_ms(40);
        filter->info[i].x = GetData(GYRO_XOUT_H) - avg[3];
        filter->info[i].y = GetData(GYRO_YOUT_H) - avg[4];
        filter->info[i].z = GetData(GYRO_ZOUT_H) - avg[5];
    }
}

void get_4_acc_data(filter_avg_t *filter)
{
    u8 i;
    for (i = 0; i < FILTER_CNT; i++)
    {
        delay_ms(40);
        filter->info[i].x = GetData(ACCEL_XOUT_H) - avg[0];
        filter->info[i].y = GetData(ACCEL_YOUT_H) - avg[1];
        filter->info[i].z = GetData(ACCEL_ZOUT_H) - avg[2];
    }
}

//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(u8 REG_Address, u8 REG_data)
{
    IIC_Start();                 //起始信号
    IIC_Send_Byte(SlaveAddress); //发送设备地址+写信号
    IIC_Send_Byte(REG_Address);  //内部寄存器地址，
    IIC_Send_Byte(REG_data);     //内部寄存器数据，
    IIC_Stop();                  //发送停止信号
}

//**************************************
//从I2C设备读取一个字节数据
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
    u8 REG_data, ack;
    IIC_Start();                       //起始信号
    ack = IIC_Send_Byte(SlaveAddress); //发送设备地址+写信号
    if (ack)
    {
        printf("ack err1!\n");
    }
    ack = IIC_Send_Byte(REG_Address); //发送存储单元地址，从0开始
    if (ack)
    {
        printf("ack err2!\n");
    }
    IIC_Start();                           //起始信号
    ack = IIC_Send_Byte(SlaveAddress + 1); //发送设备地址+读信号
    if (ack)
    {
        printf("ack err3!\n");
    }
    REG_data = IIC_Read_Byte(0); //读出寄存器数据   0   NACK   1  ACK
    IIC_Stop();                  //停止信号
    return REG_data;
}

//**************************************
//初始化MPU6050
//**************************************
void InitMPU6050(void)
{
    //IIC(&dis[0],1,0x1c,GY_ADDR,WRITE);
    //      Single_WriteI2C(PWR_MGMT_1, 0x00);  //解除休眠状态
    //        Single_WriteI2C(SMPLRT_DIV, 0x07);
    //        Single_WriteI2C(CONFIG, 0x06);
    //        Single_WriteI2C(GYRO_CONFIG, 0x18);
    //        Single_WriteI2C(ACCEL_CONFIG, 0x01);

    IIC_Init();
    delay_ms(10);
    Single_WriteI2C(MPU_60X0_PWR_MGMT_1_REG_ADDR, MPU_60X0_RESET_REG_VALU);
    delay_ms(50);
    Single_WriteI2C(MPU_60X0_PWR_MGMT_1_REG_ADDR, MPU_60X0_PWR_MGMT_1_REG_VALU);
    Single_WriteI2C(MPU_60X0_USER_CTRL_REG_ADDR, MPU_60X0_USER_CTRL_REG_VALU);
    Single_WriteI2C(MPU_60X0_SMPLRT_DIV_REG_ADDR, MPU_60X0_SMPLRT_DIV_REG_VALU);
    Single_WriteI2C(MPU_60X0_CONFIG_REG_ADDR, MPU_60X0_CONFIG_REG_VALU);
    Single_WriteI2C(MPU_60X0_GYRO_CONFIG_REG_ADDR, MPU_60X0_GYRO_CONFIG_REG_VALU);
    Single_WriteI2C(MPU_60X0_ACCEL_CONFIG_REG_ADDR, MPU_60X0_ACCEL_CONFIG_REG_VALU);
    Single_WriteI2C(MPU_60X0_FIFO_EN_REG_ADDR, MPU_60X0_FIFO_EN_REG_VALU);
}

//**************************************
//合成数据
//**************************************
int32_t GetData(u8 REG_Address)
{
    u8 H, L;
    H = Single_ReadI2C(REG_Address);
    L = Single_ReadI2C(REG_Address + 1);

    return (H << 8) + L; //合成数据
}

void kalman_filter(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f)
{
    //------------------------------
    static float angle, angle_dot;
    const float Q_angle = 0.000001, Q_gyro = 0.0001, R_angle = 0.5, dt = 0.002;
    static float P[2][2] =
        {
            {1, 0},
            {0, 1}};
    static float Pdot[4] = {0, 0, 0, 0};
    const u8 C_0 = 1;
    static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
    //------------------------------
    angle += (gyro_m - q_bias) * dt;

    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;

    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;

    angle_err = angle_m - angle;

    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;

    angle += K_0 * angle_err;
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m - q_bias;

    *angle_f = angle;
    *angle_dot_f = angle_dot;
}
