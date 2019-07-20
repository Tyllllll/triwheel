#include "crossmeeting.h"
/****Definitions**********************************************/

/****Variables************************************************/

char CrossMeetChar[4] = {'R', ':', 'D', 'L'};

/****Function list********************************************/


/****Objects**************************************************/
CROSSMEETING_CLASS crossmeet;

/****Functions************************************************/

int8 turn180;
void crossmeeting()
{
	uint8 i;
	float MidGain;    
    
	crossmeet.Mode = 1;

    if(crossmeet.Process < 1)
    {
        crossmeet.Process = 1;
        crossmeet.dejavuflag = 0;
		if (crossmeet.FinishflagAnother == 0)
			crossmeet.RunTime = -200;
//        crossmeet.FinishflagAnother = 1;
    }
    MidGain = 30/sensor.once_uni_ad[EEEM];
	if(MidGain > 1.5)
		MidGain = 1.5;
	//电磁巡线
  sensor.error[0] = 0.8*35*((sensor.once_uni_ad[EEER] - sensor.once_uni_ad[EEEL]))*(3.7/(sensor.once_uni_ad[EEER] + sensor.once_uni_ad[EEEL] + 4.5*sensor.once_uni_ad[EEEM]+0.001))
        + //豎電感
              0.2*  35*((sensor.once_uni_ad[EECR] - sensor.once_uni_ad[EECL]))*(1/(sensor.once_uni_ad[EECR] + sensor.once_uni_ad[EECL] + 1*sensor.once_uni_ad[EEEM] +0.001));
        sensor.error[0] = sensor.error[0]*MidGain;

	if(crossmeet.Mode == 1) //crossmeet.Mode == 1 掉头
	{
		if (crossmeet.FinishflagAnother == 0)
		{
			crossmeet.RunTime++;
			if (crossmeet.RunTime == 32767)
				crossmeet.RunTime--;
		}
		else if(crossmeet.RunTime >= 0)
		{
			crossmeet.RunTime--;
			if (crossmeet.RunTime == -32768)
				crossmeet.RunTime++;
		}
		switch (crossmeet.Process)
		{
			//姿态调整段
		case 1:
			crossmeet.PostureIntegral = 1;    //角度积分关闭
			crossmeet.MeetingSpeed = 0;
			if (speed.Reality < 0.001)
			{
				crossmeet.Process = 2,bee.time = 120;
			}
			break;
		case 2:
			crossmeet.PostureIntegral = 1;    //角度积分关闭
			crossmeet.MeetingSpeed = speed.Stan ;
			Findline.errBuff = sensor.error[0];
			crossmeet.dejavuflag = 1;
			
			if(fabs(gyro.TurnAngle_Integral) > 170 && fabs(gyro.TurnAngle_Integral) < 190 && fabs(gyro.Turn_Speed) < 5)
				crossmeet.Process = 3,bee.time = 120;
			break;
		case 3:
			crossmeet.PostureIntegral = 1;    //角度积分关闭
			crossmeet.MeetingSpeed = speed.Stan ;
			if(sensorSum<9.5)
				sensor.error[0] = 0; 
			Findline.errBuff = sensor.error[0] ;
			crossmeet.dejavuflag = 2;
			// 計時時間減到0
			if (crossmeet.FinishflagAnother == 1 && crossmeet.RunTime < 0 && fabs(Findline.errBuff) < 5 && fabs(gyro.Turn_Speed) < 2)
			{
				crossmeet.Process = 4;
				bee.time = 120;
				crossmeet.dejavuflag = 0;
			}
			break;
			
		case 4:
			crossmeet.MeetingSpeed = speed.Stan;
			crossmeet.PostureIntegral = 0;    //角度积分开启     
			if( Road_or_Unroad() == 1)
			{
				crossmeet.Process = 0  ,Findline.Process = Normal, crossmeet.HasCrossMeet = 1, bee.time = 120;
			}
			
			break;
		default :
			break;
		}
	}
	else
	{
		switch (crossmeet.Process)
		{
			//姿态调整段
		case 1 :
			crossmeet.PostureIntegral = 0;    //角度积分关闭
			crossmeet.MeetingSpeed = speed.Stan;
			Findline.errBuff = sensor.error[0] ;
			
			if( crossmeet.OdometerCount <=0 )
				crossmeet.Process = 2,bee.time = 120;
			break;
		case 2 :
			crossmeet.PostureIntegral = 0;    //角度积分开启  
			crossmeet.MeetingSpeed = speed.Stan;
			Findline.errBuff = sensor.error[0] ;   
			if( Road_or_Unroad() == 1)
			{
				crossmeet.Process = 0  ,Findline.Process = Normal,bee.time = 120;
			}
			
			break;
		default :
			break;
		}
	}
	
	if(Findline.errBuff>35) Findline.errBuff = 35;
	if(Findline.errBuff<-35) Findline.errBuff = -35;  
	Findline.err[0] = Findline.errBuff;
	for(i=59;i>=1;i--)
		Findline.err[i]=Findline.err[i-1];    
	
	
}
