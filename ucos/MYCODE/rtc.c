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


