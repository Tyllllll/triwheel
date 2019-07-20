#ifndef __FINALSTOP_H
#define __FINALSTOP_H

#include "includes.h"
typedef struct
{
	float OdometerCount;
	float PostureIntegral;
	float FinalSpeed;
	float DriftErr;
	uint8 Finishflag;
	uint8 sendMessage;
	
}FINALSTOP_CLASS;
extern FINALSTOP_CLASS finalstop;

extern void finalstoping();


#endif