#ifndef __CROSSMEETING_H
#define __CROSSMEETING_H

#include "includes.h"
typedef struct
{
	float OdometerCount;
	float PostureIntegral;
	float MeetingSpeed;
	float DriftErr;
   // float AddWaitTime;                 //等待加时  单位：秒
	uint8 FinishflagMy;
	uint8 FinishflagAnother;            //1-另一辆车到达断路 或 完蛋了
	uint8 Mode;
	uint8 dejavuflag;
	int16 RunTime;
    int16 SubWaitTime;
    uint8 Process;
    uint8 HasCrossMeet;                 //1-会车结束                         
    uint8 reverse;                      //0-正向发车  1-反向发车
}CROSSMEETING_CLASS;
extern CROSSMEETING_CLASS crossmeet;
extern char CrossMeetChar[4];
extern void crossmeeting();

#endif