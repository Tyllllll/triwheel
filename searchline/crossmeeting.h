#ifndef __CROSSMEETING_H
#define __CROSSMEETING_H

#include "includes.h"
typedef struct
{
	float OdometerCount;
	float PostureIntegral;
	float MeetingSpeed;
	float DriftErr;
   // float AddWaitTime;                 //�ȴ���ʱ  ��λ����
	uint8 FinishflagMy;
	uint8 FinishflagAnother;            //1-��һ���������· �� �군��
	uint8 Mode;
	uint8 dejavuflag;
	int16 RunTime;
    int16 SubWaitTime;
    uint8 Process;
    uint8 HasCrossMeet;                 //1-�ᳵ����                         
    uint8 reverse;                      //0-���򷢳�  1-���򷢳�
}CROSSMEETING_CLASS;
extern CROSSMEETING_CLASS crossmeet;
extern char CrossMeetChar[4];
extern void crossmeeting();

#endif