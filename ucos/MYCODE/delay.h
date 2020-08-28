#ifndef _UCOSIII_SYSTICK_H_
#define _UCOSIII_SYSTICK_H_

//头文件
#include "stm32f4xx.h"
#include "includes.h"	
//宏定义
#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数

//函数声明
void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);

#endif

