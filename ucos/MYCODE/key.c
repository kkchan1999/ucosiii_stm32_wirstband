#include "key.h"
#include "delay.h"

void Key_PA0_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //使能GPIO A组时钟，
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //使能SYSCFG时钟：
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;       //引脚0
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //输入模式
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线 PA0 -- EXTI0 （可变）
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line0;           //中断线0 （可变）
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI0_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

}




void Key_PE2_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //使能GPIO A组时钟，
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //使能SYSCFG时钟：
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2;       //引脚2
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //输入模式
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线 PE2 -- EXTI0 （可变）
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line2;           //中断线2 （可变）
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI2_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

}

void Key_PE3_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //使能GPIO A组时钟，
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //使能SYSCFG时钟：
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;       //引脚3
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //输入模式
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线 PE3 -- EXTI0 （可变）
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line3;           //中断线3 （可变）
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI3_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

}

void Key_PE4_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //使能GPIO A组时钟，
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //使能SYSCFG时钟：
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_4;       //引脚4
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //输入模式
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //上拉
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线 PE4 -- EXTI0 （可变）
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line4;           //中断线4 （可变）
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //中断模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //中断线使能
    //初始化线上中断，设置触发条件等。
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI4_IRQn;   //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

}

void Key_Init(void)
{
    Key_PA0_Init();
    Key_PE2_Init();
    Key_PE3_Init();
    Key_PE4_Init();
}






