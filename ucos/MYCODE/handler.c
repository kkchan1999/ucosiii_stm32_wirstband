#include "handler.h"

//串口相关操作
u8 Usart_Data;   //值范围：0~255
u8 rx_flag = 0;  //接受数据完成 rx_flag = 1
u8 buffer[64] = {0};
u8 rx_buffer[64] = {0};
u8 rx_i = 0, count = 0;

OS_ERR  err;
extern OS_SEM      HR_sem;
extern OS_SEM      Menu_sem;


//menu用的
extern u8 Menu_time;	//弄个时间限制，太久没反应的话直接返回时间显示界面
extern u8 sleek;		//记录目前是啥功能
extern u8 Menu_flag;	//进入menu的标识
extern u8 Menu_enter;

//编写中断服务函数。这个函数不需要程序员在主函数调用，满足条件CPU自行调用的函数
void EXTI0_IRQHandler(void)//按键1 prev
{
    OSIntEnter();
    u8 flag;//弄个flag用来消抖
    //判断中断标志是否为1
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);//读取一下按键的电平
        if (RESET == flag) //在这下面干活
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);
            //变更灯状态
            GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
			
			//时间重置
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
        EXTI_ClearITPendingBit(EXTI_Line0);//清空标志位
    }
    OSIntExit();

}

void EXTI2_IRQHandler(void)//按键2 next
{
    OSIntEnter();
    u8 flag;//弄个flag用来消抖
    //判断中断标志是否为1
    if (EXTI_GetITStatus(EXTI_Line2) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);//读取一下按键的电平
        if (RESET == flag) //在这下面干活
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);

            //变更灯状态
            GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
			
			//时间重置
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
        EXTI_ClearITPendingBit(EXTI_Line2);//清空标志位
    }
    OSIntExit();
}

void EXTI3_IRQHandler(void)//按键3 enter
{
    OSIntEnter();
    u8 flag;//弄个flag用来消抖
    //判断中断标志是否为1
    if (EXTI_GetITStatus(EXTI_Line3) == SET)
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);//读取一下按键的电平
        if (RESET == flag) //在这下面干活
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);
            	
			if(Menu_flag!=0)//必须确定在目录里面才能用
			{
				//在这里写功能
				Menu_enter = 1;
				GPIO_ToggleBits(GPIOE, GPIO_Pin_13);//变更灯状态
			}
			
			Menu_flag = 1;
			

        }
        EXTI_ClearITPendingBit(EXTI_Line3);//清空标志位
    }
    OSIntExit();
}

void EXTI4_IRQHandler(void)//按键4 exit
{
    OSIntEnter();
    u8 flag;//弄个flag用来消抖
    
    if (EXTI_GetITStatus(EXTI_Line4) == SET)//判断中断标志是否为1
    {
        delay_ms(10);
        flag = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);//读取一下按键的电平
        if (RESET == flag) //在这下面干活
        {
            OSSemPost((OS_SEM *)&Menu_sem,            //信号量控制块,
                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
                      (OS_ERR *)&err);
//            OSSemPost((OS_SEM *)&HR_sem,              //信号量控制块,
//                      (OS_OPT)OS_OPT_POST_ALL,        //向等待该信号量的所有任务发送信号量
//                      (OS_ERR *)&err);
			
			if(Menu_flag != 0)//必须确定在目录里面才能用
			{
				//在这里写功能
				GPIO_ToggleBits(GPIOE, GPIO_Pin_14);//变更灯状态
			}
			
			Menu_flag = 1;

        }
        EXTI_ClearITPendingBit(EXTI_Line4);            //清空标志位
    }
    OSIntExit();
}

void USART1_IRQHandler(void)//接受串口的东西，调试蓝牙的时候才用得上
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






