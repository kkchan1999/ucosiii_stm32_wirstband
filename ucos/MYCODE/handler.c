#include "handler.h"

//������ز���
u8 Usart_Data;   //ֵ��Χ��0~255
u8 rx_flag = 0;  //����������� rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i = 0, count = 0;

OS_ERR  err;
extern OS_SEM      HR_sem;
extern OS_SEM      Menu_sem;


//menu�õ�
extern u8 Menu_time;	//Ū��ʱ�����ƣ�̫��û��Ӧ�Ļ�ֱ�ӷ���ʱ����ʾ����
extern u8 sleek;		//��¼Ŀǰ��ɶ����
extern u8 Menu_flag;	//����menu�ı�ʶ
extern u8 Menu_enter;

//��д�жϷ������������������Ҫ����Ա�����������ã���������CPU���е��õĺ���
void EXTI0_IRQHandler(void)//����1 prev
{
    OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag) //��������ɻ�
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //�ź������ƿ�,
                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
                      (OS_ERR *)&err);
            //�����״̬
            GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
			
			//ʱ������
			Menu_time = 0;
			if(sleek-1<0)
			{
				sleek = 3;
			}
			else
			{
				sleek--;
			}
        }
        EXTI_ClearITPendingBit(EXTI_Line0);//��ձ�־λ
    }
    OSIntExit();

}

void EXTI2_IRQHandler(void)//����2 next
{
    OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line2) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag) //��������ɻ�
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //�ź������ƿ�,
                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
                      (OS_ERR *)&err);

            //�����״̬
            GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
			
			//ʱ������
			Menu_time = 0;
			if(sleek+1>3)
			{
				sleek = 0;
			}
			else
			{
				sleek++;
			}

        }
        EXTI_ClearITPendingBit(EXTI_Line2);//��ձ�־λ
    }
    OSIntExit();
}

void EXTI3_IRQHandler(void)//����3 enter
{
    OSIntEnter();
    u8 flag;//Ū��flag��������
    //�ж��жϱ�־�Ƿ�Ϊ1
    if (EXTI_GetITStatus(EXTI_Line3) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag) //��������ɻ�
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //�ź������ƿ�,
                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
                      (OS_ERR *)&err);
            	
			if(Menu_flag!=0)//����ȷ����Ŀ¼���������
			{
				//������д����
				Menu_enter = 1;
				GPIO_ToggleBits(GPIOE, GPIO_Pin_13);//�����״̬
			}
			
			Menu_flag = 1;
			

        }
        EXTI_ClearITPendingBit(EXTI_Line3);//��ձ�־λ
    }
    OSIntExit();
}

void EXTI4_IRQHandler(void)//����4 exit
{
    OSIntEnter();
    u8 flag;//Ū��flag��������
    
    if (EXTI_GetITStatus(EXTI_Line4) == SET)//�ж��жϱ�־�Ƿ�Ϊ1
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);//��ȡһ�°����ĵ�ƽ
        if (RESET == flag) //��������ɻ�
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //�ź������ƿ�,
                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
                      (OS_ERR *)&err);
//            OSSemPost((OS_SEM *)&HR_sem,              //�ź������ƿ�,
//                      (OS_OPT)OS_OPT_POST_ALL,        //��ȴ����ź����������������ź���
//                      (OS_ERR *)&err);
			
			if(Menu_flag != 0)//����ȷ����Ŀ¼���������
			{
				//������д����
				GPIO_ToggleBits(GPIOE, GPIO_Pin_14);//�����״̬
			}
			
			Menu_flag = 1;

        }
        EXTI_ClearITPendingBit(EXTI_Line4);            //��ձ�־λ
    }
    OSIntExit();
}

void USART1_IRQHandler(void)//���ܴ��ڵĶ���������������ʱ����õ���
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






