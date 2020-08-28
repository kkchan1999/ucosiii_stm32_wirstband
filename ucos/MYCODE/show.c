#include "show.h"


void ShowTime(unsigned char x, unsigned char y)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    u8 t[6];
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
    t[0] = RTC_TimeStruct.RTC_Hours / 10;
    t[1] = RTC_TimeStruct.RTC_Hours % 10;
    t[2] = RTC_TimeStruct.RTC_Minutes / 10;
    t[3] = RTC_TimeStruct.RTC_Minutes % 10;
    t[4] = RTC_TimeStruct.RTC_Seconds / 10;
    t[5] = RTC_TimeStruct.RTC_Seconds % 10;
    for (int i = 0, j = 0; i < 8; i++, j++)
    {
        if (i == 2 || i == 5)
        {
            OLED_ShowBigNum(i * 16 + x, y, 10);
            j--;
            continue;
        }
        OLED_ShowBigNum(i * 16 + x, y, t[j]);
    }
}

void ShowDate(unsigned char x, unsigned char y)
{
    RTC_DateTypeDef RTC_DateStruct;
    u8 d[6];
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

    d[0] = RTC_DateStruct.RTC_Year / 10;
    d[1] = RTC_DateStruct.RTC_Year % 10;
    d[2] = RTC_DateStruct.RTC_Month / 10;
    d[3] = RTC_DateStruct.RTC_Month % 10;
    d[4] = RTC_DateStruct.RTC_Date / 10;
    d[5] = RTC_DateStruct.RTC_Date % 10;
    for (int i = 0, j = 0; i < 8; i++, j++)
    {
        if (i == 2 || i == 5)
        {
            OLED_ShowBigNum(i * 16 + x, y, 11);
            j--;
            continue;
        }
        OLED_ShowBigNum(i * 16 + x, y, d[j]);
    }

}
