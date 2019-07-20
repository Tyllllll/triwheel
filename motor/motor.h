#ifndef __MOTOR_H
#define __MOTOR_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/

#define SPEED_F 200.0f	//�ٶȻ�ȡƵ��
#define L_QD_UNIT 5837.0f//������һ�׵ļ���
#define R_QD_UNIT 5837.0f

typedef struct
{
	void (*MotorInit)(int16* PWM);
	void (*QdInit)(void);
	void (*GetSpeed)(void);
	void (*MotorPwmFlash)(int16* PWM);
	uint16 crazyRunCounter;        
	float Odometer;
	
} MOTOR_CLASS;

/****Variables************************************************/

extern int16 MotorPwm[4];
extern float L_CarSpeed;
extern float R_CarSpeed;


extern MOTOR_CLASS motor;

/****Functions************************************************/


#endif