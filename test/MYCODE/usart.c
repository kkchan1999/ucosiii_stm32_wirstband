#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
int _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数   printf 是一个宏
int fputc(int ch, FILE *f)
{
//    USART_SendData(USART3, ch); //通过串口发送数据
//    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return ch;
}


void Usart1_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    // 串口是挂载在 APB2 下面的外设，所以使能函数为
    //使能 USART1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //使用的是串口 1，串口 1 对应着芯片引脚 PA9,PA10 需要使能PA的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //设置引脚复用器映射
    //引脚复用器映射配置，需要配置PA9，PA10 的引脚，调用函数为：
    //PA9 复用为 USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    //PA10 复用为 USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 与 GPIOA10
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;             //配置IO口复用功能
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;         //速度 50MHz
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;             //上拉
    //初始化 PA9，PA10
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStruct.USART_BaudRate     = 115200;               //一般设置为 115200;
    USART_InitStruct.USART_WordLength   = USART_WordLength_8b;  //字长为 8 位数据格式
    USART_InitStruct.USART_StopBits     = USART_StopBits_1;     //一个停止位
    USART_InitStruct.USART_Parity       = USART_Parity_No;      //无奇偶校验位
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件控制流
    USART_InitStruct.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;    //收发模式  双全工
    //初始化串口
    USART_Init(USART1, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;      //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

    //配置串口接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void Usart3_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    // 串口是挂载在 APB1 下面的外设，所以使能函数为
    //使能 USART3 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //使用的是串口 3，串口3 对应着芯片引脚 PB10 11 需要使能PB的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //设置引脚复用器映射
    //引脚复用器映射配置，需要配置PB10 11 的引脚，调用函数为：
    //PB10 复用为 USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    //PB11 复用为 USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10 与 GPIOB11
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;             //配置IO口复用功能
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;         //速度 50MHz
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;            //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;             //上拉
    //初始化 PB10,PB11
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStruct.USART_BaudRate     = 115200;               //一般设置为 115200;
    USART_InitStruct.USART_WordLength   = USART_WordLength_8b;  //字长为 8 位数据格式
    USART_InitStruct.USART_StopBits     = USART_StopBits_1;     //一个停止位
    USART_InitStruct.USART_Parity       = USART_Parity_No;      //无奇偶校验位
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件控制流
    USART_InitStruct.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;    //收发模式  双全工
    //初始化串口
    USART_Init(USART3, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = USART3_IRQn;      //NVIC通道，在stm32f4xx.h可查看通道 （可变）
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //响应优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //使能
    //配置中断分组（NVIC），并使能中断。
    NVIC_Init(&NVIC_InitStruct);

    //配置串口接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART3, ENABLE);
}


