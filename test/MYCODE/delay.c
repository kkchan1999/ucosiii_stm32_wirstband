#include "delay.h"


u32 my_us = 21;     //计21个数，用时1us
u32 my_ms = 21000;  //计21000个数，用时1ms

void Delay_Init(void)
{
    //systick定时器频率：168/8 = 21MHZ
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

}


//参数u32 nus值范围：0~798915
void delay_us(u32 nus)
{
    u32 temp;

    //设置重装值
    SysTick->LOAD = my_us * nus - 1;
    //设置VAL为0
    SysTick->VAL = 0x00;

    //开定时器
    SysTick->CTRL |= (0x01 << 0);


    do
    {
        temp = SysTick->CTRL;  //temp 第十六为1,则说时计数到0
    }
    while (temp & (0x01 << 0) && !(temp & (0x01 << 16)));
    //判断定时器是否开启   判断计数是否到0

    //关定时器
    SysTick->CTRL &= ~(0x01 << 0);

}

//参数u32 nms值范围：0~798
void delay_ms(u32 nms)
{

    u32 temp;

    //设置重装值
    SysTick->LOAD = my_ms * nms - 1;
    //设置VAL为0
    SysTick->VAL = 0x00;

    //开定时器
    SysTick->CTRL |= (0x01 << 0);


    do
    {
        temp = SysTick->CTRL;  //temp 第十六为1,则说时计数到0
    }
    while (temp & (0x01 << 0) && !(temp & (0x01 << 16)));
    //判断定时器是否开启   判断计数是否到0

    //关定时器
    SysTick->CTRL &= ~(0x01 << 0);

}


void delay_s(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        delay_ms(500);
        delay_ms(500);
    }
}
