#ifndef _UCOSIII_SYSTICK_H_
#define _UCOSIII_SYSTICK_H_

//ͷ�ļ�
#include "stm32f4xx.h"
#include "includes.h"	
//�궨��
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���

//��������
void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);

#endif

