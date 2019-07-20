#ifndef __TASK_H
#define __TASK_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/

#define NumOfTask 4

typedef struct
{
	void (*Init)(void);
	void (*Delay)(int xms); // ��ʱ����
	void (*Time_Start)(void); // ��ʼ��ʱ
	void (*Time_Stop)(void); // ֹͣ��ʱ��ʱ������testTime���棬��λΪuS

	unsigned int taskNum[NumOfTask];
	float time, testTime;
} TASK_CLASS;

/****Variables************************************************/

extern TASK_CLASS task;

/****Function list********************************************/

void pit0_isr(void);
void pit2_isr(void);
void pit3_isr(void);

/*************************************************************/

#endif /* __TASK_H */
