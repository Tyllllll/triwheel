#include "finalstop.h"
/****Definitions**********************************************/

/****Variables************************************************/


/****Function list********************************************/


/****Objects**************************************************/
FINALSTOP_CLASS finalstop;

/****Functions************************************************/

void finalstoping()
{
	static uint8 stopProcess;
	
	
	if( stopProcess < 1)
		stopProcess = 1;
	
	switch (stopProcess)
	{
	case 1: 
		finalstop.FinalSpeed = 0;
		finalstop.PostureIntegral = 0;    //角度积分开启          
		Findline.err[0] = 0; 
		
		break;
	}
	
}