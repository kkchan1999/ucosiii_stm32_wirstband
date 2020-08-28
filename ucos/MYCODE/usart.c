#include "usart.h"
#include "stdio.h"

#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
int _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����   printf ��һ����
int fputc(int ch, FILE *f)
{
//    USART_SendData(USART3, ch); //ͨ�����ڷ�������
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

    // �����ǹ����� APB2 ��������裬����ʹ�ܺ���Ϊ
    //ʹ�� USART1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //ʹ�õ��Ǵ��� 1������ 1 ��Ӧ��оƬ���� PA9,PA10 ��Ҫʹ��PA��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //�������Ÿ�����ӳ��
    //���Ÿ�����ӳ�����ã���Ҫ����PA9��PA10 �����ţ����ú���Ϊ��
    //PA9 ����Ϊ USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    //PA10 ����Ϊ USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 �� GPIOA10
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;             //����IO�ڸ��ù���
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;         //�ٶ� 50MHz
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;            //���츴�����
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;             //����
    //��ʼ�� PA9��PA10
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStruct.USART_BaudRate     = 115200;               //һ������Ϊ 115200;
    USART_InitStruct.USART_WordLength   = USART_WordLength_8b;  //�ֳ�Ϊ 8 λ���ݸ�ʽ
    USART_InitStruct.USART_StopBits     = USART_StopBits_1;     //һ��ֹͣλ
    USART_InitStruct.USART_Parity       = USART_Parity_No;      //����żУ��λ
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������
    USART_InitStruct.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ  ˫ȫ��
    //��ʼ������
    USART_Init(USART1, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;      //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

    //���ô��ڽ����ж�
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void Usart3_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    // �����ǹ����� APB1 ��������裬����ʹ�ܺ���Ϊ
    //ʹ�� USART3 ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //ʹ�õ��Ǵ��� 3������3 ��Ӧ��оƬ���� PB10 11 ��Ҫʹ��PB��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //�������Ÿ�����ӳ��
    //���Ÿ�����ӳ�����ã���Ҫ����PB10 11 �����ţ����ú���Ϊ��
    //PB10 ����Ϊ USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    //PB11 ����Ϊ USART1
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10 �� GPIOB11
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;             //����IO�ڸ��ù���
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;         //�ٶ� 50MHz
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;            //���츴�����
    GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;             //����
    //��ʼ�� PB10,PB11
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStruct.USART_BaudRate     = 115200;               //һ������Ϊ 115200;
    USART_InitStruct.USART_WordLength   = USART_WordLength_8b;  //�ֳ�Ϊ 8 λ���ݸ�ʽ
    USART_InitStruct.USART_StopBits     = USART_StopBits_1;     //һ��ֹͣλ
    USART_InitStruct.USART_Parity       = USART_Parity_No;      //����żУ��λ
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������
    USART_InitStruct.USART_Mode         = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ  ˫ȫ��
    //��ʼ������
    USART_Init(USART3, &USART_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = USART3_IRQn;      //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

    //���ô��ڽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART3, ENABLE);
}


