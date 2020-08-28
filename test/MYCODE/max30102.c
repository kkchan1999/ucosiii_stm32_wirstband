#include "max30102.h"
#include "delay.h"

void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    Max30102_IIC_SCL = 1;
    Max30102_IIC_SDA = 1;
}

//�л�ģʽ
void IIC_Change_Mode(u8 mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = mode; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}



//����IIC��ʼ�ź�
void IIC_Start(void)
{
    SDA_OUT(); //sda�����
    Max30102_IIC_SDA = 1;
    Max30102_IIC_SCL = 1;
    delay_us(4);
    Max30102_IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
    delay_us(4);
    Max30102_IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT(); //sda�����
    Max30102_IIC_SCL = 0;
    Max30102_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    Max30102_IIC_SCL = 1;
    Max30102_IIC_SDA = 1; //����I2C���߽����ź�
    delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN(); //SDA����Ϊ����
    Max30102_IIC_SDA = 1;
    delay_us(1);
    Max30102_IIC_SCL = 1;
    delay_us(1);
    while (Max30102_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    Max30102_IIC_SCL = 0; //ʱ�����0
    return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
    Max30102_IIC_SCL = 0;
    SDA_OUT();
    Max30102_IIC_SDA = 0;
    delay_us(2);
    Max30102_IIC_SCL = 1;
    delay_us(2);
    Max30102_IIC_SCL = 0;
}
//������ACKӦ��
void IIC_NAck(void)
{
    Max30102_IIC_SCL = 0;
    SDA_OUT();
    Max30102_IIC_SDA = 1;
    delay_us(2);
    Max30102_IIC_SCL = 1;
    delay_us(2);
    Max30102_IIC_SCL = 0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    Max30102_IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
    for (t = 0; t < 8; t++)
    {
        Max30102_IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        delay_us(2); //��TEA5767��������ʱ���Ǳ����
        Max30102_IIC_SCL = 1;
        delay_us(2);
        Max30102_IIC_SCL = 0;
        delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN(); //SDA����Ϊ����
    for (i = 0; i < 8; i++)
    {
        Max30102_IIC_SCL = 0;
        delay_us(2);
        Max30102_IIC_SCL = 1;
        receive <<= 1;
        if (Max30102_READ_SDA)
            receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck(); //����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}

void IIC_WriteBytes(u8 WriteAddr, u8 *data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(WriteAddr); //����д����
    IIC_Wait_Ack();

    for (i = 0; i < dataLength; i++)
    {
        IIC_Send_Byte(data[i]);
        IIC_Wait_Ack();
    }
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}

void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr, u8 *data, u8 dataLength)
{
    u8 i;
    IIC_Start();

    IIC_Send_Byte(deviceAddr); //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(writeAddr);
    IIC_Wait_Ack();
    IIC_Send_Byte(deviceAddr | 0X01); //�������ģʽ
    IIC_Wait_Ack();

    for (i = 0; i < dataLength - 1; i++)
    {
        data[i] = IIC_Read_Byte(1);
    }
    data[dataLength - 1] = IIC_Read_Byte(0);
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}

void IIC_Read_One_Byte(u8 daddr, u8 addr, u8 *data)
{
    IIC_Start();

    IIC_Send_Byte(daddr); //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(addr); //���͵�ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(daddr | 0X01); //�������ģʽ
    IIC_Wait_Ack();
    *data = IIC_Read_Byte(0);
    IIC_Stop(); //����һ��ֹͣ����
}

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data)
{
    IIC_Start();

    IIC_Send_Byte(daddr); //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(addr); //���͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(data); //�����ֽ�
    IIC_Wait_Ack();
    IIC_Stop(); //����һ��ֹͣ����
    delay_ms(10);
}

u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{

    /* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

    /* ��1��������I2C���������ź� */
    IIC_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_WR); /* �˴���дָ�� */

    /* ��3��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ */
    IIC_Send_Byte(Register_Address);
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��5������ʼд������ */
    IIC_Send_Byte(Word_Data);

    /* ��6��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();
    return 1; /* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();
    return 0;
}

u8 max30102_Bus_Read(u8 Register_Address)
{
    u8 data;

    /* ��1��������I2C���������ź� */
    IIC_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_WR); /* �˴���дָ�� */

    /* ��3��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ�� */
    IIC_Send_Byte((uint8_t)Register_Address);
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    IIC_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��9������ȡ���� */
    {
        data = IIC_Read_Byte(0); /* ��1���ֽ� */

        IIC_NAck(); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
    }
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();
    return data; /* ִ�гɹ� ����dataֵ */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();
    return 0;
}

void max30102_FIFO_ReadWords(u8 Register_Address, u16 Word_Data[][2], u8 count)
{
    u8 i = 0;
    u8 no = count;
    u8 data1, data2;
    /* ��1��������I2C���������ź� */
    IIC_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_WR); /* �˴���дָ�� */

    /* ��3��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ�� */
    IIC_Send_Byte((uint8_t)Register_Address);
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    IIC_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��9������ȡ���� */
    while (no)
    {
        data1 = IIC_Read_Byte(0);
        IIC_Ack();
        data2 = IIC_Read_Byte(0);
        IIC_Ack();
        Word_Data[i][0] = (((u16)data1 << 8) | data2); //

        data1 = IIC_Read_Byte(0);
        IIC_Ack();
        data2 = IIC_Read_Byte(0);
        if (1 == no)
            IIC_NAck(); /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
        else
            IIC_Ack();
        Word_Data[i][1] = (((u16)data1 << 8) | data2);

        no--;
        i++;
    }
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();
}

void max30102_FIFO_ReadBytes(u8 Register_Address, u8 *Data)
{
    max30102_Bus_Read(REG_INTR_STATUS_1);
    max30102_Bus_Read(REG_INTR_STATUS_2);

    /* ��1��������I2C���������ź� */
    IIC_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_WR); /* �˴���дָ�� */

    /* ��3��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��4���������ֽڵ�ַ�� */
    IIC_Send_Byte((uint8_t)Register_Address);
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    IIC_Start();

    /* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    IIC_Send_Byte(max30102_WR_address | I2C_RD); /* �˴��Ƕ�ָ�� */

    /* ��8��������ACK */
    if (IIC_Wait_Ack() != 0)
    {
        goto cmd_fail; /* EEPROM������Ӧ�� */
    }

    /* ��9������ȡ���� */
    Data[0] = IIC_Read_Byte(1);
    Data[1] = IIC_Read_Byte(1);
    Data[2] = IIC_Read_Byte(1);
    Data[3] = IIC_Read_Byte(1);
    Data[4] = IIC_Read_Byte(1);
    Data[5] = IIC_Read_Byte(0);
    /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /* ����I2C����ֹͣ�ź� */
    IIC_Stop();

    //	u8 i;
    //	u8 fifo_wr_ptr;
    //	u8 firo_rd_ptr;
    //	u8 number_tp_read;
    //	//Get the FIFO_WR_PTR
    //	fifo_wr_ptr = max30102_Bus_Read(REG_FIFO_WR_PTR);
    //	//Get the FIFO_RD_PTR
    //	firo_rd_ptr = max30102_Bus_Read(REG_FIFO_RD_PTR);
    //
    //	number_tp_read = fifo_wr_ptr - firo_rd_ptr;
    //
    //	//for(i=0;i<number_tp_read;i++){
    //	if(number_tp_read>0){
    //		IIC_ReadBytes(max30102_WR_address,REG_FIFO_DATA,Data,6);
    //	}

    //max30102_Bus_Write(REG_FIFO_RD_PTR,fifo_wr_ptr);
}

void max30102_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    IIC_Init();

    max30102_reset();

    //	max30102_Bus_Write(REG_MODE_CONFIG, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
    //	max30102_Bus_Write(REG_INTR_STATUS_2, 0xF0); //open all of interrupt
    //	max30102_Bus_Write(REG_INTR_STATUS_1, 0x00); //all interrupt clear
    //	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x02); //DIE_TEMP_RDY_EN
    //	max30102_Bus_Write(REG_TEMP_CONFIG, 0x01); //SET   TEMP_EN

    //	max30102_Bus_Write(REG_SPO2_CONFIG, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

    //	max30102_Bus_Write(REG_LED1_PA, 0x47);
    //	max30102_Bus_Write(REG_LED2_PA, 0x47);

    max30102_Bus_Write(REG_INTR_ENABLE_1, 0xc0); // INTR setting
    max30102_Bus_Write(REG_INTR_ENABLE_2, 0x00);
    max30102_Bus_Write(REG_FIFO_WR_PTR, 0x00); //FIFO_WR_PTR[4:0]
    max30102_Bus_Write(REG_OVF_COUNTER, 0x00); //OVF_COUNTER[4:0]
    max30102_Bus_Write(REG_FIFO_RD_PTR, 0x00); //FIFO_RD_PTR[4:0]
    max30102_Bus_Write(REG_FIFO_CONFIG, 0x0f); //sample avg = 1, fifo rollover=false, fifo almost full = 17
    max30102_Bus_Write(REG_MODE_CONFIG, 0x03); //0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
    max30102_Bus_Write(REG_SPO2_CONFIG, 0x27); // SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)
    max30102_Bus_Write(REG_LED1_PA, 0x24);     //Choose value for ~ 7mA for LED1
    max30102_Bus_Write(REG_LED2_PA, 0x24);     // Choose value for ~ 7mA for LED2
    max30102_Bus_Write(REG_PILOT_PA, 0x7f);    // Choose value for ~ 25mA for Pilot LED

    //	// Interrupt Enable 1 Register. Set PPG_RDY_EN (data available in FIFO)
    //	max30102_Bus_Write(0x2, 1<<6);

    //	// FIFO configuration register
    //	// SMP_AVE: 16 samples averaged per FIFO sample
    //	// FIFO_ROLLOVER_EN=1
    //	//max30102_Bus_Write(0x8,  1<<4);
    //	max30102_Bus_Write(0x8, (0<<5) | 1<<4);

    //	// Mode Configuration Register
    //	// SPO2 mode
    //	max30102_Bus_Write(0x9, 3);

    //	// SPO2 Configuration Register
    //	max30102_Bus_Write(0xa,
    //			(3<<5)  // SPO2_ADC_RGE 2 = full scale 8192 nA (LSB size 31.25pA); 3 = 16384nA
    //			| (1<<2) // sample rate: 0 = 50sps; 1 = 100sps; 2 = 200sps
    //			| (3<<0) // LED_PW 3 = 411��s, ADC resolution 18 bits
    //	);

    //	// LED1 (red) power (0 = 0mA; 255 = 50mA)
    //	max30102_Bus_Write(0xc, 0xb0);

    //	// LED (IR) power
    //	max30102_Bus_Write(0xd, 0xa0);
}

void max30102_reset(void)
{
    max30102_Bus_Write(REG_MODE_CONFIG, 0x40);
    max30102_Bus_Write(REG_MODE_CONFIG, 0x40);
}

void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
    //  char ach_i2c_data[2];
    //  ach_i2c_data[0]=uch_addr;
    //  ach_i2c_data[1]=uch_data;
    //
    //  IIC_WriteBytes(I2C_WRITE_ADDR, ach_i2c_data, 2);
    IIC_Write_One_Byte(I2C_WRITE_ADDR, uch_addr, uch_data);
}

void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
    //  char ch_i2c_data;
    //  ch_i2c_data=uch_addr;
    //  IIC_WriteBytes(I2C_WRITE_ADDR, &ch_i2c_data, 1);
    //
    //  i2c.read(I2C_READ_ADDR, &ch_i2c_data, 1);
    //
    //   *puch_data=(uint8_t) ch_i2c_data;
    IIC_Read_One_Byte(I2C_WRITE_ADDR, uch_addr, puch_data);
}

void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
    uint32_t un_temp;
    unsigned char uch_temp;
    char ach_i2c_data[6];
    *pun_red_led = 0;
    *pun_ir_led = 0;

    //read and clear status register
    maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
    maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);

    IIC_ReadBytes(I2C_WRITE_ADDR, REG_FIFO_DATA, (u8 *)ach_i2c_data, 6);

    un_temp = (unsigned char)ach_i2c_data[0];
    un_temp <<= 16;
    *pun_red_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[1];
    un_temp <<= 8;
    *pun_red_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[2];
    *pun_red_led += un_temp;

    un_temp = (unsigned char)ach_i2c_data[3];
    un_temp <<= 16;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[4];
    un_temp <<= 8;
    *pun_ir_led += un_temp;
    un_temp = (unsigned char)ach_i2c_data[5];
    *pun_ir_led += un_temp;
    *pun_red_led &= 0x03FFFF; //Mask MSB [23:18]
    *pun_ir_led &= 0x03FFFF;  //Mask MSB [23:18]
}
