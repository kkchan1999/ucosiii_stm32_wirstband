#ifndef __IIC_H
#define __IIC_H
#include "stm32f4xx.h"
#include "sys.h"

//IO��������,Ҫ�ú�����
#define SDA_IN()  IIC_Change_Mode(GPIO_Mode_IN)
#define SDA_OUT() IIC_Change_Mode(GPIO_Mode_OUT)

//IO��������
#define IIC_SCL    PBout(8) //SCL
#define IIC_SDA    PBout(9) //SDA    
#define READ_SDA   PBin(9)  //����SDA 

//IIC���в�������
void IIC_Init(void);              //��ʼ��IIC��IO��
void IIC_Change_Mode(u8 mode);    //�л�ģʽ
void IIC_Start(void);               //����IIC��ʼ�ź�
void IIC_Stop(void);                //����IICֹͣ�ź�
u8 IIC_Send_Byte(u8 txd);         //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);              //IIC�ȴ�ACK�ź�
void IIC_Ack(void);                 //IIC����ACK�ź�
void IIC_NAck(void);                //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);


#endif
