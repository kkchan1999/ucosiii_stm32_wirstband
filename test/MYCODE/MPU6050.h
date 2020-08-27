#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f4xx.h"
#include "delay.h"
//****************************************
// 定义MPU6050内部地址
//****************************************
#define SMPLRT_DIV      0x19    //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG          0x1A    //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG     0x1B    //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG    0x1C    //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40
#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42
#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48
#define PWR_MGMT_1      0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I        0x75    //IIC地址寄存器(默认数值0x68，只读)
#define SlaveAddress    0xD0    //IIC写入时的地址字节数据，+1为读取

#define MPU_60X0_PWR_MGMT_1_REG_ADDR        0x6B
#define MPU_60X0_USER_CTRL_REG_ADDR         0x6A
#define MPU_60X0_SMPLRT_DIV_REG_ADDR        0x19
#define MPU_60X0_CONFIG_REG_ADDR            0x1A
#define MPU_60X0_GYRO_CONFIG_REG_ADDR       0x1B
#define MPU_60X0_ACCEL_CONFIG_REG_ADDR      0x1C
#define MPU_60X0_FIFO_EN_REG_ADDR           0x23

#define MPU_60X0_RESET_REG_VALU             0x80
#define MPU_60X0_PWR_MGMT_1_REG_VALU        0x09    // Disable temperature sensor, PLL with X axis gyroscope reference  
#define MPU_60X0_USER_CTRL_REG_VALU         0x45    // Enable FIFO. Reset FIFO and signal paths for all sensors  
#define MPU_60X0_SMPLRT_DIV_REG_VALU        0x00    // DLPF_CFG is 0x01, so Gyroscope Output Rate = 1kHz, divided by 1, still 1kHz  
//本来是0x03
#define MPU_60X0_CONFIG_REG_VALU            0x02    // 184Hz 2.0ms 188Hz 1.9ms 1kHz. So there will be 6x2 bytes new data in FIFO every 1ms  
//下面的改过，本来是0x08
#define MPU_60X0_GYRO_CONFIG_REG_VALU       0x18    // Gyroscope works at 500dps. If selftest is needed, REMEMBER to put this to 250dps  
#define MPU_60X0_ACCEL_CONFIG_REG_VALU      0x08    // Accelerometer works at 4g range. If selftest is needed, REMEMBER to put this to 8g range  
#define MPU_60X0_FIFO_EN_REG_VALU           0x78    // Only enable accel and gyro  

int32_t GetData(u8 REG_Address) ;
void InitMPU6050(void);
void kalman_filter(float angle_m, float gyro_m, float *angle_f, float *angle_dot_f);//卡尔曼滤波
void Check_MPU6050(void);
void Get_MPU6050_Data(void);
#endif
