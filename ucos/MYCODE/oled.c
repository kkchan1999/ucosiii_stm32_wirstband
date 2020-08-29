#include "oled.h"
#include "codetab.h"
//SDA模式
void Iic_Sda_Mode(GPIOMode_TypeDef Mode)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_10;       //引脚9
    GPIO_InitStruct.GPIO_Mode   = Mode;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;    //推挽
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz; //快速
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);

}

//开始信号
void Iic_Start(void)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);

    //总线空闲
    OLED_SCL = 1;
    OLED_SDA_OUT = 1;
    delay_us(5);


    OLED_SDA_OUT = 0;
    delay_us(5);
    OLED_SCL = 0;  //钳住总线
}

//停止信号
void Iic_Stop(void)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);

    OLED_SCL = 0;
    OLED_SDA_OUT = 0;
    delay_us(5);

    OLED_SCL = 1;
    delay_us(5);
    OLED_SDA_OUT = 1;
}

//发送一位数据
void Iic_Send_Ack(u8 ack)
{
    Iic_Sda_Mode(GPIO_Mode_OUT);

    OLED_SCL = 0;
    //准备数据
    //要发数据1
    if (ack == 1)
    {
        OLED_SDA_OUT = 1;     //引脚输1
    }
    //要发数据0
    if (ack == 0)
    {
        OLED_SDA_OUT = 0;     //引脚输0
    }

    delay_us(5);
    OLED_SCL = 1;
    delay_us(5);
    OLED_SCL = 0;

}

//发一个字节
void Iic_Send_Byte(u8 data)
{
    u8 i;

    Iic_Sda_Mode(GPIO_Mode_OUT);

    OLED_SCL = 0;

    for (i = 0; i < 8; i++)
    {
        //准备数据 如数据 0x87 1 0 0 0 0 1 1 1
        if (data & (1 << (7 - i)))
        {
            OLED_SDA_OUT = 1;     //引脚输1
        }
        //要发数据0
        else
        {
            OLED_SDA_OUT = 0;     //引脚输0
        }
        delay_us(5);
        OLED_SCL = 1;
        delay_us(5);
        OLED_SCL = 0;
    }

}

void Iic_Send_Byte_k(u8 data)
{
    u8 i;
    //从高位发起
    for (i = 0; i < 8; i++)
    {
        Iic_Send_Ack((data >> (7 - i)) & 0x01); //就这？
    }

}



//接受一位数据
u8 Iic_Recv_Ack(void)
{
    u8 ack = 0;

    Iic_Sda_Mode(GPIO_Mode_IN);

    OLED_SCL = 0;
    delay_us(5);
    OLED_SCL = 1;
    delay_us(5);
    if (OLED_SDA_IN == 1) //判断引脚电平是否为高电平
    {
        ack = 1;
    }
    if (OLED_SDA_IN == 0) //判断引脚电平是否为低电平
    {
        ack = 0;
    }

    OLED_SCL = 0;

    return ack;
}

//接受一个字节
u8 Iic_Recv_Byte(void)
{
    u8 i, data = 0;  //0 0 0 0 0 0 0 0

    Iic_Sda_Mode(GPIO_Mode_IN);

    OLED_SCL = 0;
    //循环8次，接受一个字节
    for (i = 0; i < 8; i++)
    {
        delay_us(5);
        OLED_SCL = 1;
        delay_us(5);
        if (OLED_SDA_IN == 1) //判断引脚电平是否为高电平
        {
            data |= (1 << (7 - i));
        }

        OLED_SCL = 0;
    }

    return data;
}

u8 Iic_Recv_Byte_k(void)
{
    u8 i, data, ack;

    data = 0;
    for (i = 0; i < 8; i++)
    {
        ack = Iic_Recv_Ack();
        data |= (ack << (7 - i));
    }
    return data;
}


void I2C_Configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;


    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_8 | GPIO_Pin_10;      //引脚8 10
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;    //输出模式
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;    //推挽
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz; //快速
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);


    //VCC&GND
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    PDout(1) = 0;
    PDout(15) = 1;

    //总线空闲
    OLED_SCL = 1;
    OLED_SDA_OUT = 1;

}

void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    u8 ack;
    //启动信号
    //I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
    //while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); /*EV5,主模式*/
    Iic_Start();

    //发送设备地址（一个字节）
    //I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
    Iic_Send_Byte(OLED_ADDRESS);
    //等待应答
    //while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    ack = Iic_Recv_Ack();
    if (ack == 1)
    {
        printf("ack failure\n");
        Iic_Stop();
        return;
    }

    //发送寄存器地址（一个字节）
    // I2C_SendData(I2C1, addr);//寄存器地址
    Iic_Send_Byte(addr);
    //等待应答
    //while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    ack = Iic_Recv_Ack();
    if (ack == 1)
    {
        printf("ack failure\n");
        Iic_Stop();
        return;
    }

    //发送数据（一个字节）
    // I2C_SendData(I2C1, data);//发送数据
    Iic_Send_Byte(data);
    //等待应答
    ack = Iic_Recv_Ack();
    if (ack == 1)
    {
        printf("ack failure\n");
        Iic_Stop();
        return;
    }
    //while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    //停止信号
    //I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
    Iic_Stop();
}


void WriteCmd(unsigned char I2C_Command)//写命令
{
    I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
    I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
    I2C_Configuration();
    delay_ms(100); //这里的延时很重要

    WriteCmd(0xAE); //display off
    WriteCmd(0x20); //Set Memory Addressing Mode
    WriteCmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8); //Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //亮度调节 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
    unsigned char m, n;
    for (m = 0; m < 8; m++)
    {
        WriteCmd(0xb0 + m);     //page0-page1
        WriteCmd(0x00);     //low column start address
        WriteCmd(0x10);     //high column start address
        for (n = 0; n < 128; n++)
        {
            WriteDat(fill_Data);
        }
    }
}

void OLED_CLS(void)//清屏
{
    OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X14);  //开启电荷泵
    WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X10);  //关闭电荷泵
    WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          :
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    unsigned char c = 0, i = 0, j = 0;
    switch (TextSize)
    {
    case 1:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 126)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x, y);
            for (i = 0; i < 6; i++)
                WriteDat(F6x8[c][i]);
            x += 6;
            j++;
        }
    }
    break;
    case 2:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 120)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(x, y);
            for (i = 0; i < 8; i++)
                WriteDat(F8X16[c * 16 + i]);
            OLED_SetPos(x, y + 1);
            for (i = 0; i < 8; i++)
                WriteDat(F8X16[c * 16 + i + 8]);
            x += 8;
            j++;
        }
    }
    break;

    }
}



void OLED_ShowBigNum(unsigned char x, unsigned char y, unsigned char num, u8 rev)
{
    u8 i, j;
    for (j = 0; j < 3; j++)
    {
        OLED_SetPos(x, y + j);
        for (i = 0; i < 16; i++)
		if(rev)
            WriteDat(~(F16x24NUM[num * 48 + i + 16 * j]));
		else
			WriteDat((F16x24NUM[num * 48 + i + 16 * j]));
    }
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          :
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm = 0;
    unsigned int  adder = 32 * N;
    OLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          :
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        OLED_SetPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            WriteDat(BMP[j++]);
        }
    }
}

