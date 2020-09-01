#include "rtc.h"
#include "delay.h"

void Rtc_Init(void)
{
    RTC_InitTypeDef  RTC_InitStruct;
//    RTC_TimeTypeDef  RTC_TimeStruct;
//    RTC_DateTypeDef RTC_DateStruct;


    //1、使能PWR时钟：
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //2、使能后备寄存器访问: 这类寄存器是掉电保存数据的
    PWR_BackupAccessCmd(ENABLE);
    //3、配置RTC时钟源，使能RTC时钟：
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    //要打开LSE：
    RCC_LSEConfig(RCC_LSE_ON);

    delay_ms(50);  //延时等待时钟稳定


    RTC_InitStruct.RTC_HourFormat   = RTC_HourFormat_24;  //时间格式
    RTC_InitStruct.RTC_AsynchPrediv = 0x7F;               //异步分频器
    RTC_InitStruct.RTC_SynchPrediv  = 0xFF;               //同步分频器
    //4、 初始化RTC(同步/异步分频系数和时钟格式)：
    RTC_Init(&RTC_InitStruct);

//    RTC_TimeStruct.RTC_H12      = RTC_H12_PM;  //对于24小时格式，这个参数可以不用
//    RTC_TimeStruct.RTC_Hours    = 17; //时
//    RTC_TimeStruct.RTC_Minutes  = 01; //分
//    RTC_TimeStruct.RTC_Seconds  = 10; //秒
    //5、 设置时间：
    //RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);


//    RTC_DateStruct.RTC_Year     = 20;   //20年，前年20要自己补
//    RTC_DateStruct.RTC_Month    = 8;    //月
//    RTC_DateStruct.RTC_Date     = 27;   //日
//    RTC_DateStruct.RTC_WeekDay  = 4;    //星期
    //6、设置日期：
    //RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

}

void RTC_Alarm_AInit(void)
{
    RTC_TimeTypeDef RTC_AlarmTime;
    RTC_AlarmTypeDef RTC_AlarmStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    //2、关闭闹钟：
    RTC_AlarmCmd(RTC_Alarm_A, DISABLE);


    //闹钟时间设置
    RTC_AlarmTime.RTC_H12       = RTC_H12_PM;  //对于24小时格式，这个参数可以不用
    RTC_AlarmTime.RTC_Hours     = 16; //时
    RTC_AlarmTime.RTC_Minutes   = 36; //分
    RTC_AlarmTime.RTC_Seconds   = 30; //秒

    RTC_AlarmStruct.RTC_AlarmTime           = RTC_AlarmTime;        //时间设置
    RTC_AlarmStruct.RTC_AlarmMask           = RTC_AlarmMask_None;   //无掩码位 按实际时间来响应闹钟
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay; //按星期来闹钟
    RTC_AlarmStruct.RTC_AlarmDateWeekDay    = RTC_Weekday_Wednesday;  //星期3


    //3、配置闹钟参数：
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);

    //4、开启闹钟：
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    //5、开启配置闹钟中断：

    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    EXTI_InitStruct.EXTI_Line   = EXTI_Line17;          //中断线17
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = RTC_Alarm_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);


}

void RTC_Alarm_BInit(void)
{
    RTC_TimeTypeDef     RTC_AlarmTime;
    RTC_AlarmTypeDef    RTC_AlarmStruct;
    EXTI_InitTypeDef    EXTI_InitStruct;
    NVIC_InitTypeDef    NVIC_InitStruct;

    //2、关闭闹钟：
    RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

    //闹钟时间设置
    RTC_AlarmTime.RTC_H12       = RTC_H12_PM;  //对于24小时格式，这个参数可以不用
    RTC_AlarmTime.RTC_Hours     = 1; //时
    RTC_AlarmTime.RTC_Minutes   = 1; //分
    RTC_AlarmTime.RTC_Seconds   = 1; //秒

    RTC_AlarmStruct.RTC_AlarmTime           = RTC_AlarmTime;        //时间设置
    RTC_AlarmStruct.RTC_AlarmMask           = RTC_AlarmMask_None;   //无掩码位 按实际时间来响应闹钟
    RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;  //按星期来闹钟
    RTC_AlarmStruct.RTC_AlarmDateWeekDay    = 1;                    //星期3


    //3、配置闹钟参数：
    RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_B, &RTC_AlarmStruct);

    //4、开启闹钟：
    //RTC_AlarmCmd(RTC_Alarm_B,ENABLE);

    //5、开启配置闹钟中断：

    RTC_ITConfig(RTC_IT_ALRB, ENABLE);

    EXTI_InitStruct.EXTI_Line   = EXTI_Line17;          //中断线17
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = RTC_Alarm_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);
}
