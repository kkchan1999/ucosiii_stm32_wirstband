#include "MPU6050.h"
#include "iic.h"
//**************************************
//向I2C设备写入一个字节数据
//**************************************
void Single_WriteI2C(u8 REG_Address, u8 REG_data)
{
    IIC_Start();                  //起始信号
    IIC_Send_Byte(SlaveAddress);   //发送设备地址+写信号
    IIC_Send_Byte(REG_Address);    //内部寄存器地址，
    IIC_Send_Byte(REG_data);       //内部寄存器数据，
    IIC_Stop();                   //发送停止信号
}

//**************************************
//从I2C设备读取一个字节数据
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
    u8 REG_data, ack;
    IIC_Start();                   //起始信号
    ack = IIC_Send_Byte(SlaveAddress);    //发送设备地址+写信号
    if (ack)
    {
        printf("ack err1!\n");
    }
    ack = IIC_Send_Byte(REG_Address);    //发送存储单元地址，从0开始
    if (ack)
    {
        printf("ack err2!\n");
    }
    IIC_Start();                   //起始信号
    ack = IIC_Send_Byte(SlaveAddress + 1); //发送设备地址+读信号
    if (ack)
    {
        printf("ack err3!\n");
    }
    REG_data = IIC_Read_Byte(0);     //读出寄存器数据   0   NACK   1  ACK
    IIC_Stop();                    //停止信号
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
        { 1, 0 },
        { 0, 1 }
    };
    static float Pdot[4] = {0, 0, 0, 0};
    const u8 C_0 = 1;
    static float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
    //------------------------------
    angle += (gyro_m - q_bias) * dt;

    Pdot[0]  = Q_angle - P[0][1] - P[1][0];
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

    angle   += K_0 * angle_err;
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m - q_bias;

    *angle_f = angle;
    *angle_dot_f = angle_dot;
}


