#include "handler.h"

//������ز���
u8 Usart_Data;   //ֵ��Χ��0~255
u8 rx_flag = 0;  //����������� rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i = 0, count = 0;

OS_ERR  err;
extern OS_SEM      HR_sem;


//��д�жϷ������������������Ҫ����Ա�����������ã���������CPU���е��õĺ���
void EXTI0_IRQHandler(void)//����1
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

void EXTI2_IRQHandler(void)//����2
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

void EXTI3_IRQHandler(void)//����3
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

void EXTI4_IRQHandler(void)//����4
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

            OSSemPost((OS_SEM *)&HR_sem,              //�ź������ƿ�,
                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
                      (OS_ERR *)&err);            
            //��ձ�־λ
        }
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    OSIntExit();
}

void USART1_IRQHandler(void)
{
    OSIntEnter();
    //���Ƿǿգ��򷵻�ֵΪ1����RESET��0���жϣ���������ж�Ϊ��
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //�ж�Ϊ���Ϊ�´��ж���׼��������Ҫ���жϵı�־����
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        /* DR��ȡ���ܵ�������*/
        buffer[count++] = USART_ReceiveData(USART1);

        if (buffer[count - 1] == ':')
        {
            //ȥ��:
            for (rx_i = 0; rx_i < (count - 1); rx_i++)
            {
                rx_buffer[rx_i] = buffer[rx_i];
            }
            memset(buffer, 0, sizeof(buffer));
            count = 0;  //��Ϊ0,��һ֡���ݴ�buffer[0]��ʼ�洢
            rx_flag = 1; //����������� rx_flag = 1
        }
    }
    OSIntExit();
}






