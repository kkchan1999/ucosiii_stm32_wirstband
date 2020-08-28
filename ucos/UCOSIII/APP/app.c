/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <includes.h>





#define  APP_TASK_EQ_0_ITERATION_NBR              16u




static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  void  AppTaskStart(void     *p_arg);



//�û�����1
static  OS_TCB   AppUser1TaskStartTCB;
static  CPU_STK  AppUser1TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     AppUser1TaskStart(void     *p_arg);


//�û�����2
static  OS_TCB   AppUser2TaskStartTCB;
static  CPU_STK  AppUser2TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     AppUser2TaskStart(void     *p_arg);


//�û�����3
static  OS_TCB   AppUser3TaskStartTCB;
static  CPU_STK  AppUser3TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     AppUser3TaskStart(void     *p_arg);

//OLED��ʾ����
static  OS_TCB   OLEDStartTCB;
static  CPU_STK  OLEDStartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     OLEDStart(void     *p_arg);

//MPU6050
static  OS_TCB   MPU6050StartTCB;
static  CPU_STK  MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     MPU6050Start(void     *p_arg);


OS_FLAG_GRP  my_grp;    //�¼���־����ƿ�

u8 Usart_Data;   //ֵ��Χ��0~255
u8 rx_flag = 0;  //����������� rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i = 0, count = 0;



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




int main(void)
{
    OS_ERR  err;



    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    //���񴴽�
    OSTaskCreate((OS_TCB *)&AppTaskStartTCB,                    /* Create the start task                                */
                 (CPU_CHAR *)"App Task Start",
                 (OS_TASK_PTR)AppTaskStart,
                 (void *)0u,
                 (OS_PRIO)APP_CFG_TASK_START_PRIO,
                 (CPU_STK *)&AppTaskStartStk[0u],
                 (CPU_STK_SIZE)AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY)0u,
                 (OS_TICK)0u,
                 (void *)0u,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart(void *p_arg)
{
    OS_ERR  err;


    (void)p_arg;

    /* Initialize BSP functions                             */
    //  BSP_Tick_Init();                                            /* Initialize Tick Services.                            */
    BSP_Init();




    //���񴴽�
//    OSTaskCreate((OS_TCB *)&AppUser1TaskStartTCB,               //������ƿ�
//                 (CPU_CHAR *)"App User1 Task Start",            //��������
//                 (OS_TASK_PTR)AppUser1TaskStart,                //������ �������ǵ�ַ
//                 (void *)0u,                                    //��������
//                 (OS_PRIO)3,                                    //���ȼ�
//                 (CPU_STK *)&AppUser1TaskStartStk[0u],          //��ջ����ַ
//                 (CPU_STK_SIZE)AppUser1TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
//                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
//                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
//                 (void *)0u,                                    //��ʹ�ò�����
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
//                 (OS_ERR *)&err);


    //���񴴽�
//    OSTaskCreate((OS_TCB *)&AppUser2TaskStartTCB,               //������ƿ�
//                 (CPU_CHAR *)"App User2 Task Start",            //��������
//                 (OS_TASK_PTR)AppUser2TaskStart,                //������ �������ǵ�ַ
//                 (void *)0u,                                    //��������
//                 (OS_PRIO)3,                                    //���ȼ�
//                 (CPU_STK *)&AppUser2TaskStartStk[0u],          //��ջ����ַ
//                 (CPU_STK_SIZE)AppUser2TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
//                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
//                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
//                 (void *)0u,                                    //��ʹ�ò�����
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
//                 (OS_ERR *)&err);


    //���񴴽�
//    OSTaskCreate((OS_TCB *)&AppUser3TaskStartTCB,               //������ƿ�
//                 (CPU_CHAR *)"App User3 Task Start",            //��������
//                 (OS_TASK_PTR)AppUser3TaskStart,                //������ �������ǵ�ַ
//                 (void *)0u,                                    //��������
//                 (OS_PRIO)3,                                    //���ȼ�
//                 (CPU_STK *)&AppUser3TaskStartStk[0u],          //��ջ����ַ
//                 (CPU_STK_SIZE)AppUser3TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
//                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
//                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
//                 (void *)0u,                                    //��ʹ�ò�����
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
//                 (OS_ERR *)&err);
    //OLED
    OSTaskCreate((OS_TCB *)&OLEDStartTCB,               //������ƿ�
                 (CPU_CHAR *)"OLED Start",          //��������
                 (OS_TASK_PTR)OLEDStart,                //������ �������ǵ�ַ
                 (void *)0u,                                    //��������
                 (OS_PRIO)3,                                    //���ȼ�
                 (CPU_STK *)&OLEDStartStk[0u],          //��ջ����ַ
                 (CPU_STK_SIZE)OLEDStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
                 (void *)0u,                                    //��ʹ�ò�����
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
                 (OS_ERR *)&err);
	
	
	OSTaskCreate((OS_TCB *)&MPU6050StartTCB,               //������ƿ�
                 (CPU_CHAR *)"MPU6050 Start",          //��������
                 (OS_TASK_PTR)MPU6050Start,                //������ �������ǵ�ַ
                 (void *)0u,                                    //��������
                 (OS_PRIO)3,                                    //���ȼ�
                 (CPU_STK *)&MPU6050StartStk[0u],          //��ջ����ַ
                 (CPU_STK_SIZE)MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
                 (void *)0u,                                    //��ʹ�ò�����
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
                 (OS_ERR *)&err);




    //�¼���־�鴴��
    OSFlagCreate((OS_FLAG_GRP *)&my_grp,      //�¼���־����ƿ�
                 (CPU_CHAR *)"my_grp",
                 (OS_FLAGS)0x00,             //��־���ʼֵΪ0
                 (OS_ERR *)&err);

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    while (DEF_TRUE)                                            /* Task body, always written as an infinite loop.       */
    {
        //GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

static  void  AppUser1TaskStart(void *p_arg)
{
    OS_ERR  err;
    while (1)
    {
        //KEY0
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)  //�жϰ����Ƿ���
        {
            OSTimeDlyHMSM(0u, 0u, 0u, 15u,          //15����
                          OS_OPT_TIME_HMSM_STRICT,
                          &err); //ȥ����
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)  //�жϰ����Ƿ���
            {

                while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET); //�ȴ������ɿ�

                //���������¼�  �����¼���־
                OSFlagPost((OS_FLAG_GRP *)&my_grp,                  //�¼���־����ƿ�
                           (OS_FLAGS)0x01,                         //���ͱ�־λ��0λ  ֵΪ1
                           (OS_OPT)OS_OPT_POST_FLAG_SET,           //��1����
                           (OS_ERR *)&err);



            }

        }



        OSTimeDlyHMSM(0u, 0u, 0u, 5u,          //5����
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);



    }

}



static  void  AppUser2TaskStart(void *p_arg)
{
    OS_ERR  err;
    while (1)
    {
        //KEY1
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)  //�жϰ����Ƿ���
        {
            OSTimeDlyHMSM(0u, 0u, 0u, 15u,          //5����
                          OS_OPT_TIME_HMSM_STRICT,
                          &err); //ȥ����
            if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)  //�жϰ����Ƿ���
            {
                while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET); //�ȴ������ɿ�
                //���������¼�  �����¼���־
                OSFlagPost((OS_FLAG_GRP *)&my_grp,                  //�¼���־����ƿ�
                           (OS_FLAGS)0x02,                         //���ͱ�־λ��1λ  ֵΪ1  0010
                           (OS_OPT)OS_OPT_POST_FLAG_SET,           //��1����
                           (OS_ERR *)&err);

            }
        }
        OSTimeDlyHMSM(0u, 0u, 0u, 5u,          //5����
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

static  void  AppUser3TaskStart(void *p_arg)
{
    OS_ERR  err;
    while (1)
    {
        OSFlagPend((OS_FLAG_GRP *)&my_grp,                  //�¼���־����ƿ�
                   (OS_FLAGS)0x3,                          //��0��1λ֮��  0x01+0x02 = 0x03
                   (OS_TICK)0,                             //�����ȴ�
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_FLAG_CONSUME,        //��ͬ��������������־������
                   (CPU_TS *)NULL,
                   (OS_ERR *)&err);

        GPIO_ToggleBits(GPIOE, GPIO_Pin_14);

    }
}


static  void  OLEDStart(void *p_arg)
{
	//׼ȷ����˵�����ʱ�����ʾ������Ҫ��ϰ�����һ�����������Ҫ�õ��ź�����
    OS_ERR  err;
    OLED_Fill(0xFF);//ȫ������
    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);//1s
	
    //OLED_Fill(0x00);//ȫ����//Ū���˷��ԣ����ܱ��ȫ�ڣ���Ȼ����������
    while (1)
    {
        PFout(9) = !PFout(9);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
        ShowDate(0, 0);
        ShowTime(0, 4);
    }
}

static  void  MPU6050Start(void *p_arg)
{
	OS_ERR  err;
	Check_MPU6050();
	
	while(1)
	{
		OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
		Get_MPU6050_Data();
	}
}


