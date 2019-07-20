#ifndef __TRANSOME_H
#define __TRANSOME_H

#include "includes.h"
typedef struct
{
	float OdometerCount;
	float PostureIntegral;
	float TransomeSpeed;
	float DriftErr;
	uint8 Stopflag;
	uint8 Shuaaaaflag;
	uint8 Finishflag;
    uint8 Process;
}TRANSOME_CLASS;
extern TRANSOME_CLASS transome;

extern void transoming();

#endif