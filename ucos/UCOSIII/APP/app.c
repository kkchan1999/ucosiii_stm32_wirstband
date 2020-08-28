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



//用户任务1
static  OS_TCB   AppUser1TaskStartTCB;
static  CPU_STK  AppUser1TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     AppUser1TaskStart(void     *p_arg);


//用户任务2
static  OS_TCB   AppUser2TaskStartTCB;
static  CPU_STK  AppUser2TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     AppUser2TaskStart(void     *p_arg);


//用户任务3
static  OS_TCB   AppUser3TaskStartTCB;
static  CPU_STK  AppUser3TaskStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     AppUser3TaskStart(void     *p_arg);

//OLED显示任务
static  OS_TCB   OLEDStartTCB;
static  CPU_STK  OLEDStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     OLEDStart(void     *p_arg);

//MPU6050
static  OS_TCB   MPU6050StartTCB;
static  CPU_STK  MPU6050StartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     MPU6050Start(void     *p_arg);


OS_FLAG_GRP  my_grp;    //事件标志组控制块

u8 Usart_Data;   //值范围：0~255
u8 rx_flag = 0;  //接受数据完成 rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i = 0, count = 0;



void USART1_IRQHandler(void)
{

    OSIntEnter();

    //若是非空，则返回值为1，与RESET（0）判断，不相等则判断为真
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        //判断为真后，为下次中断做准备，则需要对中断的标志清零
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        /* DR读取接受到的数据*/
        buffer[count++] = USART_ReceiveData(USART1);

        if (buffer[count - 1] == ':')
        {
            //去除:
            for (rx_i = 0; rx_i < (count - 1); rx_i++)
            {
                rx_buffer[rx_i] = buffer[rx_i];
            }

            memset(buffer, 0, sizeof(buffer));
            count = 0;  //置为0,下一帧数据从buffer[0]开始存储
            rx_flag = 1; //接受数据完成 rx_flag = 1
        }


    }
    OSIntExit();

}




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




    //任务创建
//    OSTaskCreate((OS_TCB *)&AppUser1TaskStartTCB,               //任务控制块
//                 (CPU_CHAR *)"App User1 Task Start",            //任务名字
//                 (OS_TASK_PTR)AppUser1TaskStart,                //函数名 函数名是地址
//                 (void *)0u,                                    //函数参数
//                 (OS_PRIO)3,                                    //优先级
//                 (CPU_STK *)&AppUser1TaskStartStk[0u],          //堆栈基地址
//                 (CPU_STK_SIZE)AppUser1TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
//                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
//                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
//                 (void *)0u,                                    //不使用补允区
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
//                 (OS_ERR *)&err);


    //任务创建
//    OSTaskCreate((OS_TCB *)&AppUser2TaskStartTCB,               //任务控制块
//                 (CPU_CHAR *)"App User2 Task Start",            //任务名字
//                 (OS_TASK_PTR)AppUser2TaskStart,                //函数名 函数名是地址
//                 (void *)0u,                                    //函数参数
//                 (OS_PRIO)3,                                    //优先级
//                 (CPU_STK *)&AppUser2TaskStartStk[0u],          //堆栈基地址
//                 (CPU_STK_SIZE)AppUser2TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
//                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
//                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
//                 (void *)0u,                                    //不使用补允区
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
//                 (OS_ERR *)&err);


    //任务创建
//    OSTaskCreate((OS_TCB *)&AppUser3TaskStartTCB,               //任务控制块
//                 (CPU_CHAR *)"App User3 Task Start",            //任务名字
//                 (OS_TASK_PTR)AppUser3TaskStart,                //函数名 函数名是地址
//                 (void *)0u,                                    //函数参数
//                 (OS_PRIO)3,                                    //优先级
//                 (CPU_STK *)&AppUser3TaskStartStk[0u],          //堆栈基地址
//                 (CPU_STK_SIZE)AppUser3TaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
//                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
//                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
//                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
//                 (void *)0u,                                    //不使用补允区
//                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
//                 (OS_ERR *)&err);
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




    //事件标志组创建
    OSFlagCreate((OS_FLAG_GRP *)&my_grp,      //事件标志组控制块
                 (CPU_CHAR *)"my_grp",
                 (OS_FLAGS)0x00,             //标志组初始值为0
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
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)  //判断按键是否按下
        {
            OSTimeDlyHMSM(0u, 0u, 0u, 15u,          //15毫秒
                          OS_OPT_TIME_HMSM_STRICT,
                          &err); //去抖动
            if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)  //判断按键是否按下
            {

                while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET); //等待按键松开

                //按键处理事件  发送事件标志
                OSFlagPost((OS_FLAG_GRP *)&my_grp,                  //事件标志组控制块
                           (OS_FLAGS)0x01,                         //发送标志位是0位  值为1
                           (OS_OPT)OS_OPT_POST_FLAG_SET,           //置1操作
                           (OS_ERR *)&err);



            }

        }



        OSTimeDlyHMSM(0u, 0u, 0u, 5u,          //5毫秒
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
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)  //判断按键是否按下
        {
            OSTimeDlyHMSM(0u, 0u, 0u, 15u,          //5毫秒
                          OS_OPT_TIME_HMSM_STRICT,
                          &err); //去抖动
            if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)  //判断按键是否按下
            {
                while (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET); //等待按键松开
                //按键处理事件  发送事件标志
                OSFlagPost((OS_FLAG_GRP *)&my_grp,                  //事件标志组控制块
                           (OS_FLAGS)0x02,                         //发送标志位是1位  值为1  0010
                           (OS_OPT)OS_OPT_POST_FLAG_SET,           //置1操作
                           (OS_ERR *)&err);

            }
        }
        OSTimeDlyHMSM(0u, 0u, 0u, 5u,          //5毫秒
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

static  void  AppUser3TaskStart(void *p_arg)
{
    OS_ERR  err;
    while (1)
    {
        OSFlagPend((OS_FLAG_GRP *)&my_grp,                  //事件标志组控制块
                   (OS_FLAGS)0x3,                          //第0与1位之和  0x01+0x02 = 0x03
                   (OS_TICK)0,                             //阻塞等待
                   (OS_OPT)OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_FLAG_CONSUME,        //与同步，条件满足后标志组清零
                   (CPU_TS *)NULL,
                   (OS_ERR *)&err);

        GPIO_ToggleBits(GPIOE, GPIO_Pin_14);

    }
}


static  void  OLEDStart(void *p_arg)
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
	Check_MPU6050();
	
	while(1)
	{
		OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//1s
		Get_MPU6050_Data();
	}
}


