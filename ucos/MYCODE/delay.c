#include "delay.h"

static u8  fac_us = 0;                          //us延时倍乘数
static u16 fac_ms = 0;


//us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void)
{
    OS_ERR err;
    OSSchedLock(&err);                      //UCOSIII的方式,禁止调度，防止打断us延时
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{
    OS_ERR err;
    OSSchedUnlock(&err);                    //UCOSIII的方式,恢复调度
}


//调用OS自带的延时函数延时
//ticks:延时的节拍数
void delay_ostimedly(u32 ticks)
{
    OS_ERR err;
    OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err); //UCOSIII延时采用周期模式
}

void delay_init(u8 SYSCLK)
{
    u32 reload;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SYSTICK使用外部时钟源
    fac_us = SYSCLK / 8;                    //不论是否使用OS,fac_us都需要使用
    reload = SYSCLK / 8;                    //每秒钟的计数次数 单位为K
    reload *= 1000000 / delay_ostickspersec; //根据delay_ostickspersec设定溢出时间
    //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右
    fac_ms = 1000 / delay_ostickspersec;    //代表OS可以延时的最少单位
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //开启SYSTICK中断
    SysTick->LOAD = reload;                 //每1/OS_TICKS_PER_SEC秒中断一次
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
}

void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD;             //LOAD的值
    ticks = nus * fac_us;                   //需要的节拍数
    delay_osschedlock();                    //阻止OS调度，防止打断us延时
    told = SysTick->VAL;                    //刚进入时的计数器值

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;        //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;

            told = tnow;

            if (tcnt >= ticks)
                break;                      //时间超过/等于要延迟的时间,则退出.
        }
    }

    delay_osschedunlock();                  //恢复OS调度
}
//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(u16 nms)
{
    if (delay_osrunning && delay_osintnesting == 0) //如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)
    {
        if (nms >= fac_ms)                  //延时的时间大于OS的最少时间周期
        {
            delay_ostimedly(nms / fac_ms);  //OS延时
        }
        nms %= fac_ms;                      //OS已经无法提供这么小的延时了,采用普通方式延时
    }
    delay_us((u32)(nms * 1000));            //普通方式延时
}
