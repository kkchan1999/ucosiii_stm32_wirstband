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
static  void     AppTaskStart(void     *p_arg);

//时间显示任务
static  OS_TCB   TimeStartTCB;
static  CPU_STK  TimeStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     TimeStart(void     *p_arg);

//MPU6050
static  OS_TCB   MPU6050StartTCB;
static  CPU_STK  MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     MPU6050Start(void     *p_arg);

//max30102
static  OS_TCB   Max30102StartTCB;
static  CPU_STK  Max30102StartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     Max30102Start(void     *p_arg);

//Menu，菜单启动函数
static  OS_TCB   MenuStartTCB;
static  CPU_STK  MenuStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     MenuStart(void     *p_arg);

OS_FLAG_GRP     my_grp;    //事件标志组控制块，好像没啥用，可以先留着
OS_SEM          HR_sem;     //心率二值信号量控制块
OS_SEM          OLED_sem;   //OLED信号量
OS_SEM          Menu_sem;   //目录信号量

//这两个都是目录用的
u8 Menu_time;   //弄个时间限制，太久没反应的话直接返回时间显示界面
u8 sleek;       //记录目前是啥功能
u8 Menu_flag = 0;//当这个flag是0的时候，menu没打开，当flag为1的时候，进入了menu
u8 Menu_enter = 0;
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


    //心率检测信号量创建
    OSSemCreate((OS_SEM *)&HR_sem,           //控制块
                (CPU_CHAR *)"HR_sem",       //信号量名字
                (OS_SEM_CTR)0,              //一开始的时候是0，需要的时候才置1
                (OS_ERR *)&err);
    //OLED信号量创建
    OSSemCreate((OS_SEM *)&OLED_sem,          //控制块s
                (CPU_CHAR *)"OLED_sem",       //信号量名字
                (OS_SEM_CTR)1,                //一开始的时候是1,一开始就要用了
                (OS_ERR *)&err);
    //目录信号量创建
    OSSemCreate((OS_SEM *)&Menu_sem,          //控制块s
                (CPU_CHAR *)"Menu_sem",       //信号量名字
                (OS_SEM_CTR)0,                //一开始的时候是0，需要的时候才置1
                (OS_ERR *)&err);


    //时间显示
    OSTaskCreate((OS_TCB *)&TimeStartTCB,               //任务控制块
                 (CPU_CHAR *)"Time Start",          //任务名字
                 (OS_TASK_PTR)TimeStart,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,                                    //优先级
                 (CPU_STK *)&TimeStartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)TimeStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
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



    OSTaskCreate((OS_TCB *)&MenuStartTCB,               //任务控制块
                 (CPU_CHAR *)"Menu Start",          //任务名字
                 (OS_TASK_PTR)MenuStart,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,                                    //优先级
                 (CPU_STK *)&MenuStartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)MenuStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
                 (void *)0u,                                    //不使用补允区
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
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


static  void  TimeStart(void *p_arg)//界面显示，需要很多flag
{
    //准确点来说这个是时间的显示，后面要配合按键改一改这个东西，要用到信号量的
    OS_ERR  err;
    OLED_Fill(0xFF);//全屏点亮
    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);//1s

    OLED_Fill(0x00);//全屏灭//弄成了反显，不能变成全黑，不然背景不均匀
    while (1)
    {
        OSSemPend((OS_SEM *)&OLED_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);

        PFout(9) = !PFout(9);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
        ShowDate(0, 0);
        ShowTime(0, 4);
        OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                  (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                  (OS_ERR *)&err);
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
                      &err);//0.5s
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

        OSSemPend((OS_SEM *)&OLED_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);

        //变更灯状态
        HR = Show_HR();//初始化心率
        OLED_CLS();
        OLED_ShowBigNum(0, 0, HR / 100, 0);
        OLED_ShowBigNum(16, 0, (HR % 100) / 10, 0);
        OLED_ShowBigNum(32, 0, (HR % 10), 0);
        OLED_ShowBigNum(48, 0, 10, 0);

        OSTimeDlyHMSM(0u, 0u, 5u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//5s

        OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                  (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                  (OS_ERR *)&err);

    }

}

static  void  MenuStart(void *p_arg)//这个任务用于目录的显示
{
    OS_ERR  err;

    while (1)
    {
        u8 temp = 100;//弄成一个不会重复的值，不然会有bug（按下去第一下之刷新一部分屏幕）
        sleek = 0;
        Menu_time = 0;
		Menu_enter = 0;
        //阻塞等待按键的按下，获得menu的信号
        OSSemPend((OS_SEM *)&Menu_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);

        //拿到OLED的控制权
        OSSemPend((OS_SEM *)&OLED_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);
        Menu_flag = 1;//进入菜单状态

        while (Menu_time < 5) //选择的功能在这里面干
        {
            if (temp != sleek) //检测是不是没变，没变就不刷新了
            {
                OLED_CLS();
            }

            temp = sleek;
            OLED_SetPos(0, 0); //起始点坐标
            switch (sleek)
            {
            case 0:
                OLED_ShowStr(0, 0, "menu 0", 2);
                if (Menu_enter != 0)
                {
                    //证明按了enter
                    Menu_enter = 0;
                }

				

                break;
            case 1:
                OLED_ShowStr(0, 0, "HR", 2);

                if (Menu_enter != 0)//证明按了enter
                {
					//进入心率检测功能
                    OSSemPost((OS_SEM *)&HR_sem,              //信号量控制块,
                              (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                              (OS_ERR *)&err);
					
					Menu_time =100;//整个数退出循环
					
                    
                    Menu_enter = 0;
                }
                break;
            case 2:
                OLED_ShowStr(0, 0, "menu 2", 2);
                if (Menu_enter != 0)
                {
                    //证明按了enter
                    Menu_enter = 0;
                }
                break;
            case 3:
                OLED_ShowStr(0, 0, "menu 3", 2);
                if (Menu_enter != 0)
                {
                    //证明按了enter
                    Menu_enter = 0;
                }
                break;
            default:
                OLED_ShowStr(0, 0, "error", 2);
                break;
            }

            Menu_time++;//时间控制
            OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);//1s
        }

        OLED_CLS();//退出的时候清一下屏

        Menu_flag = 0;//退出菜单状态


        //这下面是防止按键按得太多，sem加得太多导致退不出去
        OSSemSet((OS_SEM *)&Menu_sem,
                 (OS_SEM_CTR)0,
                 (OS_ERR *)&err);

        OSSemSet((OS_SEM *)&OLED_sem,
                 (OS_SEM_CTR)0,
                 (OS_ERR *)&err);


        OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                  (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                  (OS_ERR *)&err);
    }
}





