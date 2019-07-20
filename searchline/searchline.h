#ifndef __SEARCHLINE_H
#define __SEARCHLINE_H

#include "common.h"
#include "includes.h"

#define Right_Angle_Threshold 12//13

#define Stop 0
#define Straight 1
#define Normal 2
#define Cross_OnlyLeft 3
#define Cross_OnlyRight 4
#define Cross_BothSide 5   //以上为常规态
#define Left_Roundabout_IN 6
#define Right_Roundabout_IN 7
#define Left_Roundabout_OUT 8
#define Right_Roundabout_OUT 9
#define Lamp 10
#define Lamp_D 11
#define Transome_or_Crossmeeting 12
#define Crossmeeting 13
#define Transome 14


typedef struct
{
  uint16 leftcrossflag[2],
         rightcrossflag[2],
         leftloopflag[2],
         rightloopflag[2];
  uint16 Process;
  
  int16 roadwide[90];
  
  int16 midline[90];
  
  int16 leftline[90];
  int16 rightline[90];
  int16 leftlineflag[90];
  int16 rightlineflag[90];
  
  int16 leftstartpoint;
  int16 rightstartpoint;
  
  int16 endline;
  float Odometer;
  float leftLoopOdometer;
  float rightLoopOdometer;
  int8 elementflag;
  int8 loopOdometerflag;
  int8 drawlineflag;
  int8 loseflag;
  int8 overcrossflag;
  int8 parallelendlineflag;
  float err[60];
  float errBuff;
  
 
}findline_TypeDef;

typedef struct
{
  int8 LoopArray[6];
  int8 loopNum;
  int8 loopFinishNum;
  
} LOOPARRAY_CLASS;

typedef struct
{
  int8 ElementArray[6];
  int8 elementNum;
  int8 elementFinishNum;
  
} TRANSOMEORCROSSMEET_CLASS;

typedef struct
{
	void (*findline)(void);
	void (*caculate_err)(void);
	void (*crossmeeting)(void);        
	void (*transoming)(void);       
	void (*finalstoping)(void);
	
} PICPROCSS_CLASS;

typedef struct
{
	float OdometerCount;
	float PostureIntegral;
	uint8 sendMessage;
	
}LAMP_CLASS;

extern findline_TypeDef Findline;
extern PICPROCSS_CLASS picprocss;
extern LOOPARRAY_CLASS loopArray;
extern TRANSOMEORCROSSMEET_CLASS elementArray;
extern int pro;
extern float sensorSum;
extern float Lamp_Odometer;


void Elemental_chain(void);

#endif