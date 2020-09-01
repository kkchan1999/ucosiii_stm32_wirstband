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

#define SIT_TIME 30


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

//串口时间设置任务
static  OS_TCB   SetTimeStartTCB;
static  CPU_STK  SetTimeStartStk[APP_CFG_TASK_START_STK_SIZE];  //长度128*4 = 512字节
static  void     SetTimeStart(void     *p_arg);


OS_FLAG_GRP     my_grp;    //事件标志组控制块，好像没啥用，可以先留着
OS_SEM          HR_sem;     //心率二值信号量控制块
OS_SEM          OLED_sem;   //OLED信号量
OS_SEM          Menu_sem;   //目录信号量
OS_SEM          Sleep_sem;  //睡大觉，什么都不显示了
//这两个都是目录用的
u8 Menu_time;   //弄个时间限制，太久没反应的话直接返回时间显示界面
u8 sleek;       //记录目前是啥功能
u8 Menu_flag = 0;//当这个flag是0的时候，menu没打开，当flag为1的时候，进入了menu
u8 Menu_enter = 0;

extern int16_t step_cnt;//计步的

//下面是串口用的
u8 Usart_Data;   //值范围：0~255
u8 rx_flag = 0;  //接受数据完成 rx_flag = 1
u8 rx_bluetouth_flag = 0;
u8 rx_bluetouth_buffer[64] = {0};
u8 rx_buffer[64] = {0};

u8 alarm_flag = 0;
u8 sit_flag = 0;
extern unsigned char HEART[];
extern unsigned char RUN[];
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


    OSSemCreate((OS_SEM *)&Sleep_sem,          //控制块
                (CPU_CHAR *)"Sleep_sem",       //信号量名字
                (OS_SEM_CTR)1,                //一开始的时候是0，需要的时候才置1
                (OS_ERR *)&err);


    //时间显示，这个的优先级要比其他的程序低一些，不然的话会直接抢掉OLED的显示
    OSTaskCreate((OS_TCB *)&TimeStartTCB,               //任务控制块
                 (CPU_CHAR *)"Time Start",          //任务名字
                 (OS_TASK_PTR)TimeStart,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)4,    /*就是这里的优先级！*/                 //优先级
                 (CPU_STK *)&TimeStartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)TimeStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,     //堆栈大小
                 (OS_MSG_QTY)0u,                                //禁止内部消息队列
                 (OS_TICK)0u,                                   //使用SYSTick中断时间做为任务时间片。
                 (void *)0u,                                    //不使用补允区
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),        //清空堆栈区
                 (OS_ERR *)&err);

    OSTaskCreate((OS_TCB *)&SetTimeStartTCB,               //任务控制块
                 (CPU_CHAR *)"SetTime Start",          //任务名字
                 (OS_TASK_PTR)SetTimeStart,                //函数名 函数名是地址
                 (void *)0u,                                    //函数参数
                 (OS_PRIO)3,    /*就是这里的优先级！*/                 //优先级
                 (CPU_STK *)&SetTimeStartStk[0u],          //堆栈基地址
                 (CPU_STK_SIZE)SetTimeStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],   //堆栈深度
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
	u8 mode = 0;
    u8 count = 0;
    OLED_Fill(0xFF);//全屏点亮
    OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);//1s

    OLED_Fill(0x00);//全屏灭//弄成了反显，不能变成全黑，不然背景不均匀
    while (1)
    {
        OSSemPend((OS_SEM *)&Sleep_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);
        OSSemPend((OS_SEM *)&OLED_sem,      //信号量控制块,
                  (OS_TICK)0,             //阻塞等待
                  (OS_OPT)OS_OPT_PEND_BLOCKING,    //阻塞模式
                  (CPU_TS *)NULL,         //不记录接受的时间
                  (OS_ERR *)&err);
		
        PFout(9) = !PFout(9);
        OSTimeDlyHMSM(0u, 0u, 0u, 500u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//0.5s
		
        ShowDate(0, 0, mode);
        ShowTime(0, 4, mode);

        count++;
        if (count >= 20)
        {
            //10s，自动息屏
            OLED_CLS();

            OSSemSet((OS_SEM *)&OLED_sem,
                     (OS_SEM_CTR)0,
                     (OS_ERR *)&err);

            OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);

            OSSemSet((OS_SEM *)&Sleep_sem,
                     (OS_SEM_CTR)0,
                     (OS_ERR *)&err);

            count = 0;
        }
        else
        {
            //这个信号量用来唤醒时间显示
            OSSemPost((OS_SEM *)&Sleep_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);
        }

        OSSemSet((OS_SEM *)&OLED_sem,
                 (OS_SEM_CTR)0,
                 (OS_ERR *)&err);

        OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                  (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                  (OS_ERR *)&err);
    }
}

static  void  MPU6050Start(void *p_arg)
{
//    int16_t i = 0;
    int16_t temp = step_cnt;
    OS_ERR  err;
    filter_avg_t filter;
    peak_value_t peak;
    slid_reg_t slid;
    axis_info_t sample;
    Check_MPU6050();//校准


    while (1)
    {
//        i++;
//        if (i == 5)
//        {
//            Get_MPU6050_Data();
//            i = 0;
//        }
        OSTimeDlyHMSM(0u, 0u, 0u, 40u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//40ms

        //下面是用来计步的
        get_4_gyr_data(&filter);
        filter_calculate(&filter, &sample);
        if (sample.x > 5000 || sample.y > 5000 || sample.z > 5000)
        {
            OSSemPost((OS_SEM *)&Sleep_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);
            printf("wake up time\n");
        }
        peak_update(&peak, &sample);
        slid_update(&slid, &sample);
        detect_step(&peak, &slid, &sample);
        if (temp != step_cnt)
        {
            printf("%d\n", step_cnt);
            temp = step_cnt;
        }
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
		
        //OLED_ShowStr(0, 0, "Heart Rate Start", 2);
		OLED_DrawBMP(0,0,128,8,HEART);
		
        HR = Show_HR();//初始化心率
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//5s
        
        OLED_ShowBigNum(0, 0, HR / 100, 0);
        OLED_ShowBigNum(16, 0, (HR % 100) / 10, 0);
        OLED_ShowBigNum(32, 0, (HR % 10), 0);
        OLED_ShowBigNum(48, 0, 10, 0);

        OSTimeDlyHMSM(0u, 0u, 5u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//5s
		OLED_CLS();

        OSSemSet((OS_SEM *)&OLED_sem,  //释放信号量之前弄成0，防止信号量太多阻塞不了
                 (OS_SEM_CTR)0,
                 (OS_ERR *)&err);


        OSSemPost((OS_SEM *)&OLED_sem,            //信号量控制块,
                  (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                  (OS_ERR *)&err);
    }

}

static  void  SetTimeStart(void *p_arg)
{
	int16_t time = 0;
	int16_t step_temp = step_cnt;
    OS_ERR  err;
	RTC_TimeTypeDef RTC_TimeStruct;
    
    while (1)
    {
		OSTimeDlyHMSM(0u, 0u, 0u, 200u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);//200ms
		time++;
		
		if(time >= SIT_TIME*60*5)//分钟
		{
			RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
			if(step_cnt - step_temp <= 50&&(RTC_TimeStruct.RTC_Hours>=8&&RTC_TimeStruct.RTC_Hours<=22))
			{
				printf("sit too long\n");
				//整个喇叭响一下
				PFout(8) = 1;
				//顺便把标志位置成非零
				sit_flag = 1;
			}
			time =0;//时间复位
			step_temp = step_cnt;//同步步数
		}
		
        if (rx_bluetouth_flag == 1)//一直检测是否收到了有效信息,这个是瞬间发生的，基本上可以忽略时间
        {
            PFout(10) = 1;
            //"HST095530:" 9时55分30秒（设置时间）
            if (strncmp("HST", rx_bluetouth_buffer, 3) == 0)
            {
                RTC_TimeTypeDef  RTC_TimeStruct;

                RTC_TimeStruct.RTC_H12      = RTC_H12_PM;               //对于24小时格式，这个参数可以不用
                RTC_TimeStruct.RTC_Hours    = (rx_bluetouth_buffer[3] - 48) * 10 + (rx_bluetouth_buffer[4] - 48); //时
                RTC_TimeStruct.RTC_Minutes  = (rx_bluetouth_buffer[5] - 48) * 10 + (rx_bluetouth_buffer[6] - 48); //分
                RTC_TimeStruct.RTC_Seconds  = (rx_bluetouth_buffer[7] - 48) * 10 + (rx_bluetouth_buffer[8] - 48); //秒
                // 设置时间：
                RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);
            }

            //"HSD2008204" 20年8月20日 	
            else if (strncmp("HSD", rx_bluetouth_buffer, 3) == 0)
            {
                RTC_DateTypeDef  RTC_DateStruct;

                RTC_DateStruct.RTC_Year     = (rx_bluetouth_buffer[3] - 48) * 10 + (rx_bluetouth_buffer[4] - 48); //20年，前年20要自己补
                RTC_DateStruct.RTC_Month    = (rx_bluetouth_buffer[5] - 48) * 10 + (rx_bluetouth_buffer[6] - 48); //月
                RTC_DateStruct.RTC_Date     = (rx_bluetouth_buffer[7] - 48) * 10 + (rx_bluetouth_buffer[8] - 48); //日
                RTC_DateStruct.RTC_WeekDay  = (rx_bluetouth_buffer[9] - 48);                                    //星期
                // 设置日期：
                RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
            }
			else if (strncmp("HHR", rx_bluetouth_buffer, 3) == 0)
            {
				printf("step is : %d\n",step_cnt);
            }
					//"HSAD09560020:" 9时56分00秒 20日（按日期设置闹钟）
		else if(strncmp("HSAD", rx_bluetouth_buffer, 4) == 0)
		{
			RTC_AlarmTypeDef	RTC_AlarmStruct;
			RTC_TimeTypeDef		RTC_AlarmTime;

			u8 Hours	= (rx_bluetouth_buffer[4]-48)*10 + (rx_bluetouth_buffer[5]-48);	//时
			u8 Minutes	= (rx_bluetouth_buffer[6]-48)*10 + (rx_bluetouth_buffer[7]-48);	//分
			u8 Seconds	= (rx_bluetouth_buffer[8]-48)*10 + (rx_bluetouth_buffer[9]-48);	//秒
			u8 Date	= (rx_bluetouth_buffer[10]-48)*10 + (rx_bluetouth_buffer[11]-48);	//日 
			
			// 关闭闹钟
			RTC_AlarmCmd(RTC_Alarm_B,DISABLE); 
	
			//闹钟时间设置
			RTC_AlarmTime.RTC_H12		= RTC_H12_PM;  //对于24小时格式，这个参数可以不用
			RTC_AlarmTime.RTC_Hours		= Hours; //时
			RTC_AlarmTime.RTC_Minutes	= Minutes; //分
			RTC_AlarmTime.RTC_Seconds	= Seconds; //秒
			
			RTC_AlarmStruct.RTC_AlarmTime 			= RTC_AlarmTime;  		//时间设置
			RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;	//无掩码位 按实际时间来响应闹钟
			RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_Date; 			//按星期来闹钟
			RTC_AlarmStruct.RTC_AlarmDateWeekDay	= Date;  				//星期3
				

			// 配置闹钟参数：
			RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_B, &RTC_AlarmStruct);
				
			// 开启闹钟：
			RTC_AlarmCmd(RTC_Alarm_B,ENABLE);
			
			printf("set alarm date ok!\n");
		}
		
		//"HSAW0956004:" 9时56分00秒 星期4（按星期设置闹钟）
		else if(strncmp("HSAW", rx_bluetouth_buffer, 4) == 0)
		{
			RTC_AlarmTypeDef	RTC_AlarmStruct;
			RTC_TimeTypeDef		RTC_AlarmTime;
			
			u8 Hours	= (rx_bluetouth_buffer[4]-48)*10 + (rx_bluetouth_buffer[5]-48);	//时
			u8 Minutes	= (rx_bluetouth_buffer[6]-48)*10 + (rx_bluetouth_buffer[7]-48);	//分
			u8 Seconds	= (rx_bluetouth_buffer[8]-48)*10 + (rx_bluetouth_buffer[9]-48);	//秒
			u8 WeekDay	= rx_bluetouth_buffer[10]-48;		//星期 	
			
			// 关闭闹钟
			RTC_AlarmCmd(RTC_Alarm_B,DISABLE); 
	
			// 闹钟时间设置
			RTC_AlarmTime.RTC_H12		= RTC_H12_PM;  //对于24小时格式，这个参数可以不用
			RTC_AlarmTime.RTC_Hours		= Hours; //时
			RTC_AlarmTime.RTC_Minutes	= Minutes; //分
			RTC_AlarmTime.RTC_Seconds	= Seconds; //秒
			
			RTC_AlarmStruct.RTC_AlarmTime 			= RTC_AlarmTime;  		//时间设置
			RTC_AlarmStruct.RTC_AlarmMask			= RTC_AlarmMask_None;	//无掩码位 按实际时间来响应闹钟
			RTC_AlarmStruct.RTC_AlarmDateWeekDaySel	= RTC_AlarmDateWeekDaySel_WeekDay; 			//按星期来闹钟
			RTC_AlarmStruct.RTC_AlarmDateWeekDay	= WeekDay;  			//星期3
				

			// 配置闹钟参数：
			RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_B, &RTC_AlarmStruct);
				
			// 开启闹钟：
			RTC_AlarmCmd(RTC_Alarm_B,ENABLE);
			
			printf("set alarm week ok!\n");
		}
				
            rx_bluetouth_flag = 0;  //取消数据处理标志位
        }

		
		
	}


}


static  void  MenuStart(void *p_arg)//这个任务用于目录的显示
{
    OS_ERR  err;
    char info[64] = {0};
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
            switch (sleek)//一直查询任务的状态，检测到enter之后进入任务
            {
            case 0:
//                OLED_ShowStr(0, 0, "HR", 2);
				OLED_DrawBMP(0,0,128,8,HEART);
                if (Menu_enter != 0)//证明按了enter
                {
                    //进入心率检测功能
                    OSSemPost((OS_SEM *)&HR_sem,              //信号量控制块,
                              (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                              (OS_ERR *)&err);

                    Menu_time = 100; //整个数退出循环


                    Menu_enter = 0;
                }
                break;
            case 1:
                OLED_DrawBMP(0,0,128,8,RUN);
                if (Menu_enter != 0)
                {
                    sprintf(info, "%d", step_cnt);
                    //证明按了enter
                    //OLED_CLS();
					OLED_DrawBMP(0,0,128,8,RUN);
                    OLED_ShowStr(0, 0, info, 2);
                    OSTimeDlyHMSM(0u, 0u, 3u, 0u,
                                  OS_OPT_TIME_HMSM_STRICT,
                                  &err);//1s
                    Menu_time = 100; //整个数退出循环
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
                  (OS_OPT)OS_OPT_POST_1,        //这里不能给所有的任务都发，只发给等待优先级最高的任务
                  (OS_ERR *)&err);
    }
}





