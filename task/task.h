#ifndef __TASK_H
#define __TASK_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/

#define NumOfTask 4

typedef struct
{
	void (*Init)(void);
	void (*Delay)(int xms); // 延时函数
	void (*Time_Start)(void); // 开始计时
	void (*Time_Stop)(void); // 停止计时，时间存放在testTime里面，单位为uS

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
