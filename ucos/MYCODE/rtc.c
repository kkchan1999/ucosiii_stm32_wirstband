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

void RTC_Alarm_AInit(void)
{
    RTC_TimeTypeDef RTC_AlarmTime;
    RTC_AlarmTypeDef RTC_AlarmStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    //2���ر����ӣ�
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);


    //����ʱ������
    RTC_AlarmTime.RTC_H12       = RTC_H12_PM;  //����24Сʱ��ʽ������������Բ���
    RTC_AlarmTime.RTC_Hours     = 16; //ʱ
    RTC_AlarmTime.RTC_Minutes   = 36; //��
    RTC_AlarmTime.RTC_Seconds   = 30; //��

    RTC_AlarmStruct.RTC_AlarmTime           = RTC_AlarmTime;        //ʱ������
    RTC_AlarmStruct.RTC_AlarmMask           = RTC_AlarmMask_None;   //������λ ��ʵ��ʱ������Ӧ����
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; //������������
    RTC_AlarmStruct.RTC_AlarmDateWeekDay    = RTC_Weekday_Wednesday;  //����3


    //3���������Ӳ�����
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);

    //4���������ӣ�
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    //5���������������жϣ�

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    EXTI_InitStruct.EXTI_Line   = EXTI_Line17;          //�ж���17
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = RTC_Alarm_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);


}

void RTC_Alarm_BInit(void)
{
    RTC_TimeTypeDef     RTC_AlarmTime;
    RTC_AlarmTypeDef    RTC_AlarmStruct;
    EXTI_InitTypeDef    EXTI_InitStruct;
    NVIC_InitTypeDef    NVIC_InitStruct;

    //2���ر����ӣ�
    RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

    //����ʱ������
    RTC_AlarmTime.RTC_H12       = RTC_H12_PM;  //����24Сʱ��ʽ������������Բ���
    RTC_AlarmTime.RTC_Hours     = 1; //ʱ
    RTC_AlarmTime.RTC_Minutes   = 1; //��
    RTC_AlarmTime.RTC_Seconds   = 1; //��

    RTC_AlarmStruct.RTC_AlarmTime           = RTC_AlarmTime;        //ʱ������
    RTC_AlarmStruct.RTC_AlarmMask           = RTC_AlarmMask_None;   //������λ ��ʵ��ʱ������Ӧ����
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;  //������������
    RTC_AlarmStruct.RTC_AlarmDateWeekDay    = 1;                    //����3


    //3���������Ӳ�����
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_B, &RTC_AlarmStruct);

    //4���������ӣ�
    //RTC_AlarmCmd(RTC_Alarm_B,ENABLE);

    //5���������������жϣ�

    RTC_ITConfig(RTC_IT_ALRB, ENABLE);

    EXTI_InitStruct.EXTI_Line   = EXTI_Line17;          //�ж���17
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = RTC_Alarm_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);
}
