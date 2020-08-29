#include "heartrate.h"


uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
#define MAX_BRIGHTNESS 255

//variables to calculate the on-board LED brightness that reflects the heartbeats
uint32_t un_min, un_max, un_prev_data;
int i;
int32_t n_brightness;
float f_temp;
u8 temp_num = 0;
u8 temp[6];
u8 str[100];
u8 dis_hr = 0, dis_spo2 = 0;




void HR_init(void)//检测心率之前先弄一段这样的，相当于初始化
{
    printf("\r\n MAX30102  init  \r\n");

    un_min = 0x3FFFF;
    un_max = 0;

    n_ir_buffer_length = 500; //buffer length of 100 stores 5 seconds of samples running at 100sps
    //read the first 500 samples, and determine the signal range
    for (i = 0; i < n_ir_buffer_length; i++)
    {
        while (MAX30102_INT == 1); //wait until the interrupt pin asserts

        max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);
        aun_red_buffer[i] = (long)((long)((long)temp[0] & 0x03) << 16) | (long)temp[1] << 8 | (long)temp[2]; // Combine values to get the actual number
        aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03) << 16) | (long)temp[4] << 8 | (long)temp[5]; // Combine values to get the actual number

        if (un_min > aun_red_buffer[i])
            un_min = aun_red_buffer[i];  //update signal min
        if (un_max < aun_red_buffer[i])
            un_max = aun_red_buffer[i];  //update signal max
    }
    un_prev_data = aun_red_buffer[i];
    //前500个样本（样本的前5秒）后计算心率和SpO2
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    if (dis_hr == 0) //**dis_hr == 0 && dis_spo2 == 0
    {
        sprintf((char *)str, "HR:---        "); //**HR:--- SpO2:---
    }
    else
    {
        sprintf((char *)str, "HR:%3d        ", dis_hr); //**HR:%3d SpO2:%3d
    }
//    OLED_CLS();
//    OLED_ShowStr(0, 0, str, 2);
}



u8 check_heart_rate(void)//初始化完之后在进入这个函数，检测个5遍取平均，最多整30次，30次都不行直接显示检测失败
{
    i = 0;
    un_min = 0x3FFFF;
    un_max = 0;

    //dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
    for (i = 100; i < 500; i++)
    {
        aun_red_buffer[i - 100] = aun_red_buffer[i];
        aun_ir_buffer[i - 100] = aun_ir_buffer[i];

        //update the signal min and max
        if (un_min > aun_red_buffer[i])
            un_min = aun_red_buffer[i];
        if (un_max < aun_red_buffer[i])
            un_max = aun_red_buffer[i];
    }
    //在计算心率之前采集100组样本。
    for (i = 400; i < 500; i++)
    {
        un_prev_data = aun_red_buffer[i - 1];
        while (MAX30102_INT == 1);
        max30102_FIFO_ReadBytes(REG_FIFO_DATA, temp);
        aun_red_buffer[i] = (long)((long)((long)temp[0] & 0x03) << 16) | (long)temp[1] << 8 | (long)temp[2]; // Combine values to get the actual number
        aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03) << 16) | (long)temp[4] << 8 | (long)temp[5]; // Combine values to get the actual number

        if (aun_red_buffer[i] > un_prev_data)
        {
            f_temp = aun_red_buffer[i] - un_prev_data;
            f_temp /= (un_max - un_min);
            f_temp *= MAX_BRIGHTNESS;
            n_brightness -= (int)f_temp;
            if (n_brightness < 0)
                n_brightness = 0;
        }
        else
        {
            f_temp = un_prev_data - aun_red_buffer[i];
            f_temp /= (un_max - un_min);
            f_temp *= MAX_BRIGHTNESS;
            n_brightness += (int)f_temp;
            if (n_brightness > MAX_BRIGHTNESS)
                n_brightness = MAX_BRIGHTNESS;
        }
    }
    //计算心率和SpO2水平
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    //send samples and calculation result to terminal program through UART
    if (ch_hr_valid == 1 && n_heart_rate < 130) //**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
    {
        dis_hr = n_heart_rate;
        dis_spo2 = n_sp02;
    }
    else
    {
        dis_hr = 0;
        dis_spo2 = 0;
    }

    printf("HR=%i, ", n_heart_rate);
    printf("HRvalid=%i, ", ch_hr_valid);
    printf("SpO2=%i, ", n_sp02);
    printf("SPO2Valid=%i\r\n", ch_spo2_valid);

    if (dis_hr == 0) //**dis_hr == 0 && dis_spo2 == 0
    {
        sprintf((char *)str, "HR:---        "); //**HR:--- SpO2:---
//        OLED_CLS();
//        OLED_ShowStr(0, 0, str, 2);
        return 0;
    }
    else
    {
        sprintf((char *)str, "HR:%3d        ", dis_hr); //**HR:%3d SpO2:%3d
//        OLED_CLS();
//        OLED_ShowStr(0, 0, str, 2);
        return dis_hr;
    }

}


int16_t Show_HR(void)
{
    u8 count, HR, i;
    int16_t ret = 0;
    count = 0;
    HR_init();//初始化

    while (!check_heart_rate());
    for (i = 0; i < 40; i++)
    {
        HR = check_heart_rate();
        printf("RATE : %d\n", HR);
        if (HR != 0)
        {
            ret += HR;
            count++;
            OLED_CLS();
            OLED_ShowBigNum(0, 0, HR / 100,0);
            OLED_ShowBigNum(16, 0, (HR % 100) / 10,0);
            OLED_ShowBigNum(32, 0, (HR % 10),0);
        }
        if (count >= 5)
        {
            printf("ret : %d\n", ret);
            printf("count : %d\n", count);
            printf("RET : %d\n", ret / count);
            return (ret / count);
        }
    }

    return 0;

}

