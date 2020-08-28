#include "key.h"
#include "delay.h"

void Key_PA0_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //ʹ��GPIO A��ʱ�ӣ�
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    //ʹ��SYSCFGʱ�ӣ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;       //����0
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //����ģʽ
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //����
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //����IO�����ж��ߵ�ӳ���ϵ��ȷ��ʲô���Ŷ�Ӧ�ĸ��ж��� PA0 -- EXTI0 ���ɱ䣩
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line0;           //�ж���0 ���ɱ䣩
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI0_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

}




void Key_PE2_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //ʹ��GPIO A��ʱ�ӣ�
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //ʹ��SYSCFGʱ�ӣ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_2;       //����2
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //����ģʽ
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //����
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //����IO�����ж��ߵ�ӳ���ϵ��ȷ��ʲô���Ŷ�Ӧ�ĸ��ж��� PE2 -- EXTI0 ���ɱ䣩
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line2;           //�ж���2 ���ɱ䣩
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI2_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

}

void Key_PE3_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //ʹ��GPIO A��ʱ�ӣ�
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //ʹ��SYSCFGʱ�ӣ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_3;       //����3
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //����ģʽ
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //����
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //����IO�����ж��ߵ�ӳ���ϵ��ȷ��ʲô���Ŷ�Ӧ�ĸ��ж��� PE3 -- EXTI0 ���ɱ䣩
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line3;           //�ж���3 ���ɱ䣩
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI3_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

}

void Key_PE4_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStruct;
    EXTI_InitTypeDef  EXTI_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;


    //ʹ��GPIO A��ʱ�ӣ�
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    //ʹ��SYSCFGʱ�ӣ�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_4;       //����4
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;     //����ģʽ
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;     //����
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    //����IO�����ж��ߵ�ӳ���ϵ��ȷ��ʲô���Ŷ�Ӧ�ĸ��ж��� PE4 -- EXTI0 ���ɱ䣩
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);


    EXTI_InitStruct.EXTI_Line   = EXTI_Line4;           //�ж���4 ���ɱ䣩
    EXTI_InitStruct.EXTI_Mode   = EXTI_Mode_Interrupt;  //�ж�ģʽ
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;              //�ж���ʹ��
    //��ʼ�������жϣ����ô��������ȡ�
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel                     = EXTI4_IRQn;   //NVICͨ������stm32f4xx.h�ɲ鿴ͨ�� ���ɱ䣩
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 0x01;         //��ռ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 0x01;         //��Ӧ���ȼ�
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;       //ʹ��
    //�����жϷ��飨NVIC������ʹ���жϡ�
    NVIC_Init(&NVIC_InitStruct);

}



//��д�жϷ������������������Ҫ����Ա�����������ã���������CPU���е��õĺ���
void EXTI0_IRQHandler(void)
{
	OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag)
        {
            //�����״̬
            GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
            //��ձ�־λ
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
	OSIntExit();

}

void EXTI2_IRQHandler(void)
{
	OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line2) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag)
        {
            //�����״̬
            GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
            //��ձ�־λ
        }
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
	OSIntExit();
}

void EXTI3_IRQHandler(void)
{
	OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line3) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag)
        {
            //�����״̬
            GPIO_ToggleBits(GPIOE, GPIO_Pin_13);
            //��ձ�־λ
        }
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
	OSIntExit();
}

void EXTI4_IRQHandler(void)
{
	OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line4) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag)
        {
            //�����״̬
            GPIO_ToggleBits(GPIOE, GPIO_Pin_14);
            //��ձ�־λ
        }
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
	OSIntExit();
}

void Key_Init(void)
{
    Key_PA0_Init();
    Key_PE2_Init();
    Key_PE3_Init();
    Key_PE4_Init();
}






