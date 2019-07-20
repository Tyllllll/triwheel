#include "searchline.h"

/****Definitions**********************************************/

#define MAX(x,y)        ((x)>=(y)?(x):(y))

/****Variables************************************************/
int pro;  
int16 prospect;
float sensorSum;
float Lamp_Odometer;
//uint8 LoopArray[5]= {0x00,0x00,0x00,0x00,0x00};
LOOPARRAY_CLASS loopArray = 
{
  {2,2,1,0,0},2,0,//1-左环 2-右环 
};
TRANSOMEORCROSSMEET_CLASS elementArray = 
{
	{2,2,2,0,0,0},2//1-横断 2-坡道 
};

LAMP_CLASS lamp;
/****Function list********************************************/
void findline(void);
void caculate_err(void);
void TransomegFind(void);
void TransomegFind2(void);
void CrossMeetingFind(void);
void LoopFind(void);
void CrossFind(void);
void LampFind(void);

/****Objects**************************************************/

findline_TypeDef Findline={0};

PICPROCSS_CLASS picprocss =
{
	&findline,
	&caculate_err,
	&crossmeeting,
	&transoming,
	&finalstoping,
};


/****Functions************************************************/
void caculate_err(void)
{
	static int16 i;
	static float errDeltaMax = 35;
        
        float MidGain;
        MidGain = 30/sensor.once_uni_ad[EEEM];
        //        环坡削减
        if(sensor.once_uni_ad[EEEM]>28 && Findline.Process == Normal)
        MidGain = 20/(sensor.once_uni_ad[EEEM]);
        if(MidGain > 1.5)
          MidGain = 1.5;
	//电磁巡线
        sensor.error[0] = 0.8*35*((sensor.once_uni_ad[EEER] - sensor.once_uni_ad[EEEL]))*(3.7/(sensor.once_uni_ad[EEER] + sensor.once_uni_ad[EEEL] + 4.5*sensor.once_uni_ad[EEEM]+0.001))
        + //豎電感
              0.2*  35*((sensor.once_uni_ad[EECR] - sensor.once_uni_ad[EECL]))*(1/(sensor.once_uni_ad[EECR] + sensor.once_uni_ad[EECL] + 1*sensor.once_uni_ad[EEEM] +0.001));
        sensor.error[0] = sensor.error[0]*MidGain;
     
        Findline.err[0] = sensor.error[0] ;
        if(!(Findline.Process == Right_Roundabout_OUT || Findline.Process == Left_Roundabout_OUT))
	for(i=59;i>=1;i--)
		Findline.err[i]=Findline.err[i-1];
	//出環
        if( (Findline.Process == Right_Roundabout_OUT || Findline.Process == Left_Roundabout_OUT )&&fabs(gyro.TurnAngle_Integral)<355 )
	{
          for(i=1;i<41;i+=1)
            Findline.err[0] += Findline.err[i];
          Findline.err[0] = Findline.err[0] /38;
	}
        if( (Findline.Process == Right_Roundabout_OUT || Findline.Process == Left_Roundabout_OUT )&&fabs(gyro.TurnAngle_Integral)>355 )
	{
          if(Findline.err[0] >2)
            Findline.err[0] = 2;
          if(Findline.err[0] <-2)
            Findline.err[0] = -2;
	}
	
	if(fabs(Findline.err[0]-Findline.err[1])>errDeltaMax )
	{
		Findline.err[0] = Findline.err[1]+((Findline.err[0]-Findline.err[1])>0?errDeltaMax:-errDeltaMax);
	}
	//人工入环
	if((Findline.Process == Left_Roundabout_IN||Findline.Process == Right_Roundabout_IN) && fabs(gyro.TurnAngle_Integral) <70)//100
	{
          if(Findline.Process == Left_Roundabout_IN)
            sensor.error[0] = 
              (fabs(gyro.TurnAngle_Integral)/70)
                *1 *35*((sensor.once_uni_ad[EEER] - sensor.once_uni_ad[EEEL]))*(4.7/(sensor.once_uni_ad[EEER] + sensor.once_uni_ad[EEEL] + 4.5*sensor.once_uni_ad[EEEM]+0.001))
                  + ((70 - fabs(gyro.TurnAngle_Integral))/70)
                    *2  *35*((sensor.once_uni_ad[EECR] - sensor.once_uni_ad[EECL]))*(4.7/(sensor.once_uni_ad[EECR] + sensor.once_uni_ad[EECL] + 1*sensor.once_uni_ad[EEEM]+0.001 +0.001));
          if(Findline.Process == Right_Roundabout_IN )
            sensor.error[0] = 
              (fabs(gyro.TurnAngle_Integral)/70)
                *1 *35*((sensor.once_uni_ad[EEER] - sensor.once_uni_ad[EEEL]))*(4.7/(sensor.once_uni_ad[EEER] + sensor.once_uni_ad[EEEL] + 4.5*sensor.once_uni_ad[EEEM]+0.001))
                  + ((70 - fabs(gyro.TurnAngle_Integral))/70)
                    *1.4  *35*((10 - sensor.once_uni_ad[EECL]))*(4.7/(sensor.once_uni_ad[EECR] + sensor.once_uni_ad[EECL] + 1*sensor.once_uni_ad[EEEM]+0.001 +0.001));
                  
          {
            if(sensor.error[0] > 35) sensor.error[0] = 35;
            if(sensor.error[0] <-35) sensor.error[0] = -35;
          }
			Findline.err[0] = sensor.error[0] ;
          
	}
	
        //偏差限幅
        if(Findline.err[0]>35)Findline.err[0] = 35;
        if(Findline.err[0]<-35)Findline.err[0] = -35;    
	

        
	Findline.errBuff =  Findline.err[0];
}

void findline(void)
{    
	//障碍判别
	if(Findline.Process != Stop && Findline.Process <= Cross_BothSide 
	    )
    {
        if(elementArray.ElementArray[elementArray.elementFinishNum] == 1)
            TransomegFind();
        if(speed.Distance_cnt > 3)
            TransomegFind2();
    }
		
	
	//断路判别
	if( Findline.Odometer <= 0 && Findline.Process != Stop && Findline.Process <= Cross_BothSide )
    {
        CrossMeetingFind();
	}
    
	//坡道判别
	if( Findline.Odometer <= 0 && Findline.Process != Stop
	   && elementArray.ElementArray[elementArray.elementFinishNum] == 2)
    {
		//if(speed.Distance_cnt > 5)
            LampFind();
	}
	//环识别与补线
	if( Findline.Odometer <= 0 &&  Findline.Process < Lamp 
	   && Findline.Process != Stop)
    {
		LoopFind();
    }
}

/******************************路障判定**************************************/ 
void TransomegFind()
{
	static uint16 straightCounter;

	//正常横断
	if(     
	   sensor.twice_uni_ad[EEEM] < 30
		   && fabs(Findline.errBuff) <= 4
			   && speed.Reality >=1.0
				   && !Findline.loseflag
					   && (Findline.Process == Normal|| Findline.Process == Straight ))
	{
		straightCounter++;
		if(straightCounter>5 && laser.LaserDistanse[0]<=1 && laser.LaserDistanse[0] >= 0.4
		   )
		{
			Findline.Process = Transome;
			transome.OdometerCount = 0.3;
			bee.time = 200;
			transome.Stopflag = 0;
			Findline.elementflag = 0;
            transome.Process = 1;
		}
	}
	else
	{
		straightCounter = 0;
	}
	
}
void TransomegFind2(void)
{
    static uint16 stopCounter;
    //斜入横断
	if(speed.Reality<0.1 && stopCounter<=25
	   && Findline.Process != Transome
		   && !Findline.loseflag)
	{
		stopCounter++;
		if(stopCounter>25 && laser.LaserDistanse[0]<=0.5 && laser.LaserDistanse[10]<=0.5
		   )
		{
			Findline.Process = Transome;
			transome.OdometerCount = 0.25;
			bee.time = 200;
			transome.Stopflag = 1;
			Findline.elementflag = 0;
            transome.Process = 1;
		}
	}
	else
	{
		stopCounter = 0;
	}
}
/******************************断路判定**************************************/ 
void CrossMeetingFind()
{
	static uint16 crossmeetingCounter;
	if(Road_or_Unroad()==0)
	{
		if( !Findline.loseflag && speed.Reality > 1
		   && (Findline.Process == Normal|| Findline.Process == Straight ))
		{
			crossmeetingCounter++;
			if(crossmeetingCounter >15 && Findline.Process != Crossmeeting)
			{
                crossmeet.Process= 0;
				Findline.Process = Crossmeeting;
				bee.time = 1200 ;
				crossmeet.OdometerCount = 1.65;
				Findline.elementflag = 0;
				Lora_SendMessage(CrossMeetChar, 4);
			}
		}
	}
	else
		crossmeetingCounter = 0;
}
float AngleSpeed_Filter;
/******************************坡道判定**************************************/ 
void LampFind()
{
//	static uint16 t;
//	sensorSum = sqrt(sensor.once_uni_ad[EEEL]*sensor.once_uni_ad[EEEL] + sensor.once_uni_ad[EEEM]*sensor.once_uni_ad[EEEM] + sensor.once_uni_ad[EEER]*sensor.once_uni_ad[EEER]);
//	if(     
//	   sensor.once_uni_ad[EEEM] > 29
//		   && laser.LaserDistanse[0] < 0.6
//			   && laser.LaserDistanse[10] < 1.0
//				   && !Findline.loseflag
//					   && (Findline.Process == Normal|| Findline.Process == Straight ))
//	{
//		if(Findline.Process != Lamp &&  Findline.Process != Lamp_D) //逻辑上为0
//		{
//			Findline.Process = Lamp;
//			t=0;
//			bee.time = 240 ;       
//			Findline.Odometer = 0.8;  //不进LampFind函数
//			Findline.elementflag = 0;
//		}
//		
//	}
//	if(Findline.Process == Lamp && t>4
//	   )
//	{
//		
//		Findline.Process = Lamp_D;
//		bee.time = 240 ;
//		Findline.Odometer = 1.2;//0.9
//		t=0;  
//	}
//	
//	if(Findline.Process == Lamp_D || Findline.Process == Lamp )//&& speed.Reality <1.8)
//		t++;
//	if(Findline.Process == Lamp_D && t>4)
//	{
//		Findline.Odometer = 0.8;
//		Findline.Process = Normal;
//		bee.time = 240 ;
//		elementArray.elementFinishNum++;
//		if(elementArray.elementFinishNum>elementArray.elementNum)
//			elementArray.elementFinishNum = 0;
//		
//	}
    static uint16 straightCounter;
    static int16 Lamp_size;
    static float AngleSpeed_Buff[3];
  //  static float AngleSpeed_Filter;
    /*---------------抬头角速度滤波---------------*/
    for(uint8 i = 2; i > 0; i--)
    {
        AngleSpeed_Buff[i] = AngleSpeed_Buff[i-1];
    }
    AngleSpeed_Buff[0] = gyro.Angle_Speed;
    AngleSpeed_Filter = (AngleSpeed_Buff[0] + AngleSpeed_Buff[1] + AngleSpeed_Buff[2]) / 3;
    /*---------------判定直道---------------*/
    if(fabs(Findline.errBuff) <= 4
        && fabs(gyro.Turn_Speed) <= 2 
          && speed.Reality >=1.0
              && !Findline.loseflag
                  && (Findline.Process == Normal|| Findline.Process == Straight ))
    {
        straightCounter++;    
    }
    else
    {
        straightCounter = 0;
    }
    /*-------------- 判坡----------------*/
	if(Findline.Process <= Cross_BothSide )
    {
        if(sensor.once_uni_ad[EEEM] > 20 && sensor.once_uni_ad[EEEM] < 28 
           && straightCounter >= 5
               &&  AngleSpeed_Filter > 100
                    && gyro.Car_Angle > 10
                        && (Findline.Process == Normal|| Findline.Process == Straight ))
            {
                Findline.Process = Lamp;
                bee.time = 240 ;       
                Lamp_Odometer = 1.5;
                Findline.elementflag = 0;
                Lamp_size = gyro.Angle_Speed;
            }
    }
    if(Findline.Process == Lamp)  //判定下坡
    {
        if(gyro.Car_Angle < -6)
        {
            Findline.Process = Lamp_D;
            Lamp_Odometer = 0.8;
            bee.time = 240 ; 
        }
        if(Lamp_Odometer <= 0)
        {
            Findline.Process = Normal;
            bee.time = 240 ;
        }
    }
    if(Findline.Process == Lamp_D)  //清标志
    {
        if(Lamp_Odometer <= 0 || gyro.Car_Angle > -2)
        {
            Findline.Odometer = 1;
            Findline.Process = Normal;
            bee.time = 240 ;
            Elemental_chain();
//            elementArray.elementFinishNum++;
//            if(elementArray.elementFinishNum>elementArray.elementNum)
//                elementArray.elementFinishNum = 0;
        }
    }
	
}
/******************************环岛判定与补线**************************************/   
void LoopFind()
{
//	register int16 i,j,k;
//	
//	static float delta;
//	static uint16 speedcomp;
	
	
	//判定入环位
	if( sensor.once_uni_ad[EEEM]>50 
	   //     && sensor.once_uni_ad[EEEM]<80 
	   //     && sensor.ad_mid_dif[0]<0
	   && sensor.Parallel_sum>60
		   && !Findline.loseflag
			   )
	{
		//    bee.time=50;
          if(
//             sensor.once_uni_ad[EEEM]>0.5*(sensor.once_uni_ad[EEER] +sensor.once_uni_ad[EEEL] )
                fabs(sensor.once_uni_ad[EEEL] - sensor.once_uni_ad[EEER] )<2 
                 && sensor.once_uni_ad[EECL] > sensor.once_uni_ad[EECR]
                   && loopArray.LoopArray[loopArray.loopFinishNum] == 1
                     )
          {
            Findline.Process = Left_Roundabout_IN;
            Findline.elementflag = 0;
          }
          if(
//             sensor.once_uni_ad[EEEM]>0.5*(sensor.once_uni_ad[EEER] +sensor.once_uni_ad[EEEL] )
               fabs(sensor.once_uni_ad[EEEL] - sensor.once_uni_ad[EEER] )<2 
                 && sensor.once_uni_ad[EECL] < sensor.once_uni_ad[EECR]
                   && loopArray.LoopArray[loopArray.loopFinishNum] == 2
                     )
            
          {      
            Findline.Process = Right_Roundabout_IN;
            Findline.elementflag = 0;
          }
		
	}
	
	//判定出环位
	if(fabs(gyro.TurnAngle_Integral) > 250 && Findline.Process == Right_Roundabout_IN )
	{
		//右出环
		if(Findline.Process != Right_Roundabout_OUT ) 
		{
			Findline.Process = Right_Roundabout_OUT;
		} 
		
	}
	if(fabs(gyro.TurnAngle_Integral) > 250 && Findline.Process == Left_Roundabout_IN )
	{
		//左出环
		if(Findline.Process != Left_Roundabout_OUT ) 
		{
			Findline.Process = Left_Roundabout_OUT;
		} 
	}
	
	//清除环位
	if(fabs(gyro.TurnAngle_Integral)>345 && fabs(gyro.TurnAngle_Integral)<350 )
	{
		Findline.Odometer = 1.0;
		Findline.loopOdometerflag = 0;
		loopArray.loopFinishNum++;
		bee.time = 200;
		
	}
	if((Findline.Process == Left_Roundabout_OUT || Findline.Process == Right_Roundabout_OUT ) 
	   && fabs(gyro.TurnAngle_Integral)>355 
		   && Findline.Odometer <= 0 )
	{
		bee.time = 200;
		Findline.Process = Normal;
		if(loopArray.loopFinishNum>loopArray.loopNum)
			loopArray.loopFinishNum = 0;
	} 
}
void Elemental_chain(void)
{
    if(crossmeet.reverse == 0) // 正向发车
    {
        if(crossmeet.HasCrossMeet == 0)
        {
            elementArray.elementFinishNum++;
            if(elementArray.elementFinishNum>=elementArray.elementNum)
                elementArray.elementFinishNum = 0;
        }
        else if(crossmeet.Mode == 1) //掉头方式会车
        {
            elementArray.elementFinishNum--;
            if(elementArray.elementFinishNum<0)
                elementArray.elementFinishNum = elementArray.elementNum-1;
        }
    }
    else
    {
        if(crossmeet.HasCrossMeet == 0) 
        {
            elementArray.elementFinishNum--;
            if(elementArray.elementFinishNum<0)
                elementArray.elementFinishNum = elementArray.elementNum-1;
        }
        else if(crossmeet.Mode == 1) //掉头方式会车
        {
            elementArray.elementFinishNum++;
            if(elementArray.elementFinishNum>=elementArray.elementNum)
                elementArray.elementFinishNum = 0;
        }
    }
    
}
