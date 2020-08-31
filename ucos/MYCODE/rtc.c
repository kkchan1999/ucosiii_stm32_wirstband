#include "rtc.h"
#include "delay.h"

void Rtc_Init(void)
{
    RTC_InitTypeDef  RTC_InitStruct;
//    RTC_TimeTypeDef  RTC_TimeStruct;
//    RTC_DateTypeDef RTC_DateStruct;


    //1��ʹ��PWRʱ�ӣ�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //2��ʹ�ܺ󱸼Ĵ�������: ����Ĵ����ǵ��籣�����ݵ�
    PWR_BackupAccessCmd(ENABLE);
    //3������RTCʱ��Դ��ʹ��RTCʱ�ӣ�
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    //Ҫ��LSE��
    RCC_LSEConfig(RCC_LSE_ON);

    delay_ms(50);  //��ʱ�ȴ�ʱ���ȶ�


    RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;  //ʱ���ʽ
    RTC_InitStruct.RTC_AsynchPrediv = 0x7F;               //�첽��Ƶ��
    RTC_InitStruct.RTC_SynchPrediv  = 0xFF;               //ͬ����Ƶ��
    //4�� ��ʼ��RTC(ͬ��/�첽��Ƶϵ����ʱ�Ӹ�ʽ)��
    RTC_Init(&RTC_InitStruct);

//    RTC_TimeStruct.RTC_H12      = RTC_H12_PM;  //����24Сʱ��ʽ������������Բ���
//    RTC_TimeStruct.RTC_Hours    = 17; //ʱ
//    RTC_TimeStruct.RTC_Minutes  = 01; //��
//    RTC_TimeStruct.RTC_Seconds  = 10; //��
    //5�� ����ʱ�䣺
    //RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);


//    RTC_DateStruct.RTC_Year     = 20;   //20�꣬ǰ��20Ҫ�Լ���
//    RTC_DateStruct.RTC_Month    = 8;    //��
//    RTC_DateStruct.RTC_Date     = 27;   //��
//    RTC_DateStruct.RTC_WeekDay  = 4;    //����
    //6���������ڣ�
    //RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

}


