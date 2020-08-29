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

//OLED显示任务
static  OS_TCB   OLEDStartTCB;
static  CPU_STK  OLEDStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     OLEDStart(void     *p_arg);

//MPU6050
static  OS_TCB   MPU6050StartTCB;
static  CPU_STK  MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     MPU6050Start(void     *p_arg);

//max30102
static  OS_TCB   Max30102StartTCB;
static  CPU_STK  Max30102StartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     Max30102Start(void     *p_arg);



OS_FLAG_GRP  my_grp;    //事件标志组控制块
OS_SEM      HR_sem;     //心率二值信号量控制块

int main(void)
{
    OS_ERR  err;
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    //任务创建
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
    OSTaskCreate((OS_TCB *)&OLEDStartTCB,               //任务控制块
                 (CPU_CHAR *)"OLED Start",          //任务名字
                 (OS_TASK_PTR)OLEDStart,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,                                    //优先级
                 (CPU_STK *)&OLEDStartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)OLEDStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
                 (void *)0u,                                    //不使用补允区
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
                 (OS_ERR *)&err);


    OSTaskCreate((OS_TCB *)&MPU6050StartTCB,               //任务控制块
                 (CPU_CHAR *)"MPU6050 Start",          //任务名字
                 (OS_TASK_PTR)MPU6050Start,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,                                    //优先级
                 (CPU_STK *)&MPU6050StartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
                 (void *)0u,                                    //不使用补允区
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
                 (OS_ERR *)&err);


    OSTaskCreate((OS_TCB *)&Max30102StartTCB,               //任务控制块
                 (CPU_CHAR *)"Max30102 Start",          //任务名字
                 (OS_TASK_PTR)Max30102Start,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,                                    //优先级
                 (CPU_STK *)&Max30102StartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)Max30102StartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
                 (void *)0u,                                    //不使用补允区
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
                 (OS_ERR *)&err);

    //心率检测信号量创建
    OSSemCreate((OS_SEM *)&HR_sem,           //控制块
                (CPU_CHAR *)"HR_sem",       //信号量名字
                (OS_SEM_CTR)0,              //一开始的时候是0，需要的时候才置1
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


static  void  OLEDStart(void *p_arg)//界面显示，需要很多flag
{
    //准确点来说这个是时间的显示，后面要配合按键改一改这个东西，要用到信号量的
    OS_ERR  err;
    OLED_Fill(0xFF);//全屏点亮
    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);//1s

    //OLED_Fill(0x00);//全屏灭//弄成了反显，不能变成全黑，不然背景不均匀
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
    Check_MPU6050();//校准

    while (1)
    {
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
        Get_MPU6050_Data();//目前只是打印出来
    }
}

static  void  Max30102Start(void *p_arg)
{
    OS_ERR  err;
    int16_t HR;
    while (1)
    {
        OSSemPend((OS_SEM *)&HR_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);

        OSSchedLock(&err);
        //变更灯状态
        HR = Show_HR();//初始化心率
        OLED_CLS();
        OLED_ShowBigNum(0, 0, HR / 100, 0);
        OLED_ShowBigNum(16, 0, (HR % 100) / 10, 0);
        OLED_ShowBigNum(32, 0, (HR % 10), 0);
        OLED_ShowBigNum(48, 0, 10, 0);
//        OSTimeDlyHMSM(0u, 0u, 10u, 0u,
//                      OS_OPT_TIME_HMSM_STRICT|OS_OPT_TIME_PERIODIC,
//                      &err);//10s
        //OSSchedUnlock(&err);//这个方法不可常用

    }


}







