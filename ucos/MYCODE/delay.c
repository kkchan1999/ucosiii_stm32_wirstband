#include "delay.h"

static u8  fac_us = 0;                          //us��ʱ������
static u16 fac_ms = 0;


//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{
    OS_ERR err;
    OSSchedLock(&err);                      //UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{
    OS_ERR err;
    OSSchedUnlock(&err);                    //UCOSIII�ķ�ʽ,�ָ�����
}


//����OS�Դ�����ʱ������ʱ
//ticks:��ʱ�Ľ�����
void delay_ostimedly(u32 ticks)
{
    OS_ERR err;
    OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err); //UCOSIII��ʱ��������ģʽ
}

void delay_init(u8 SYSCLK)
{
    u32 reload;
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//SYSTICKʹ���ⲿʱ��Դ
    fac_us = SYSCLK / 8;                    //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
    reload = SYSCLK / 8;                    //ÿ���ӵļ������� ��λΪK
    reload *= 1000000 / delay_ostickspersec; //����delay_ostickspersec�趨���ʱ��
    //reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����
    fac_ms = 1000 / delay_ostickspersec;    //����OS������ʱ�����ٵ�λ
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //����SYSTICK�ж�
    SysTick->LOAD = reload;                 //ÿ1/OS_TICKS_PER_SEC���ж�һ��
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //����SYSTICK
}

void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD;             //LOAD��ֵ
    ticks = nus * fac_us;                   //��Ҫ�Ľ�����
    delay_osschedlock();                    //��ֹOS���ȣ���ֹ���us��ʱ
    told = SysTick->VAL;                    //�ս���ʱ�ļ�����ֵ

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;        //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else
                tcnt += reload - tnow + told;

            told = tnow;

            if (tcnt >= ticks)
                break;                      //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }
    }

    delay_osschedunlock();                  //�ָ�OS����
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u16 nms)
{
    if (delay_osrunning && delay_osintnesting == 0) //���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)
    {
        if (nms >= fac_ms)                  //��ʱ��ʱ�����OS������ʱ������
        {
            delay_ostimedly(nms / fac_ms);  //OS��ʱ
        }
        nms %= fac_ms;                      //OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ
    }
    delay_us((u32)(nms * 1000));            //��ͨ��ʽ��ʱ
}
