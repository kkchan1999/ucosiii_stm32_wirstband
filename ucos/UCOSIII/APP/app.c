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

//OLED��ʾ����
static  OS_TCB   OLEDStartTCB;
static  CPU_STK  OLEDStartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     OLEDStart(void     *p_arg);

//MPU6050
static  OS_TCB   MPU6050StartTCB;
static  CPU_STK  MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     MPU6050Start(void     *p_arg);

//max30102
static  OS_TCB   Max30102StartTCB;
static  CPU_STK  Max30102StartStk[APP_CFG_TASK_START_STK_SIZE];  //����128*4 = 512�ֽ�
static  void     Max30102Start(void     *p_arg);



OS_FLAG_GRP  my_grp;    //�¼���־����ƿ�
OS_SEM      HR_sem;     //���ʶ�ֵ�ź������ƿ�

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

    OSStart(&err); /* Start multitasking (i.e. give control to uC/OS-III). */
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


    OSTaskCreate((OS_TCB *)&Max30102StartTCB,               //������ƿ�
                 (CPU_CHAR *)"Max30102 Start",          //��������
                 (OS_TASK_PTR)Max30102Start,                //������ �������ǵ�ַ
                 (void *)0u,                                    //��������
                 (OS_PRIO)3,                                    //���ȼ�
                 (CPU_STK *)&Max30102StartStk[0u],          //��ջ����ַ
                 (CPU_STK_SIZE)Max30102StartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //��ջ���
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //��ջ��С
                 (OS_MSG_QTY)0u,                                //��ֹ�ڲ���Ϣ����
                 (OS_TICK)0u,                                   //ʹ��SYSTick�ж�ʱ����Ϊ����ʱ��Ƭ��
                 (void *)0u,                                    //��ʹ�ò�����
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //��ն�ջ��
                 (OS_ERR *)&err);

    //���ʼ���ź�������
    OSSemCreate((OS_SEM *)&HR_sem,           //���ƿ�
                (CPU_CHAR *)"HR_sem",       //�ź�������
                (OS_SEM_CTR)0,              //һ��ʼ��ʱ����0����Ҫ��ʱ�����1
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


static  void  OLEDStart(void *p_arg)//������ʾ����Ҫ�ܶ�flag
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
    Check_MPU6050();//У׼

    while (1)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
        Get_MPU6050_Data();//Ŀǰֻ�Ǵ�ӡ����
    }
}

static  void  Max30102Start(void *p_arg)
{
    OS_ERR  err;
    int16_t HR;
    while (1)
    {
        OSSemPend((OS_SEM *)&HR_sem,      //�ź������ƿ�,
                  (OS_TICK)0,             //�����ȴ�
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //����ģʽ
                  (CPU_TS *)NULL,         //����¼���ܵ�ʱ��
                  (OS_ERR *)&err);

        OSSchedLock(&err);
        //�����״̬
        HR = Show_HR();//��ʼ������
        OLED_CLS();
        OLED_ShowBigNum(0, 0, HR / 100, 0);
        OLED_ShowBigNum(16, 0, (HR % 100) / 10, 0);
        OLED_ShowBigNum(32, 0, (HR % 10), 0);
        OLED_ShowBigNum(48, 0, 10, 0);
//        OSTimeDlyHMSM(0u, 0u, 10u, 0u,
//                      OS_OPT_TIME_HMSM_STRICT|OS_OPT_TIME_PERIODIC,
//                      &err);//10s
        //OSSchedUnlock(&err);//����������ɳ���

    }


}







