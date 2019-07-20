#include "transome.h"
/****Definitions**********************************************/

/****Variables************************************************/


/****Function list********************************************/


/****Objects**************************************************/
TRANSOME_CLASS transome ;

/****Functions************************************************/

void transoming()
{
  if(  transome.Stopflag == 1)
  {
    switch (transome.Process)
    {
    case 1:
        transome.TransomeSpeed = -2.4;
        transome.PostureIntegral = 1;
        Findline.errBuff = sensor.error[0];
        if(transome.OdometerCount > 0.3)
        {
            transome.Process = 2;
            bee.time=200;
        }
        break;
    case 2 :
      transome.Shuaaaaflag = 1;
      transome.TransomeSpeed = 2.4;
      transome.PostureIntegral = 1;    //角度积分关闭         
      Findline.errBuff = -10*8.5/steerK_p;
      if( gyro.TurnAngle_Integral<-40 )
      {
        transome.Shuaaaaflag = 0;
        transome.Process = 3;
        bee.time=200;
        transome.OdometerCount = 0.3;//.40
      }            
      break;
    case 3 :
      transome.TransomeSpeed = 2.4;
      transome.PostureIntegral = 1;    //角度积分开启          
      Findline.errBuff = -10*8.5/steerK_p;
      if(transome.OdometerCount<=0 && gyro.TurnAngle_Integral > 0)
      {
        bee.time=200;
        transome.Process = 4;
        transome.OdometerCount = 0.2;
      }
      break;
      
    case 4 :
      transome.TransomeSpeed = 2.4;
      transome.PostureIntegral = 1;    //角度积分开启          
      Findline.errBuff = -18*8.5/steerK_p;
      if(transome.OdometerCount<=0 && gyro.TurnAngle_Integral>75 )
      {
        transome.Process = 5;
        bee.time=200;
        transome.OdometerCount=0.2;
      }
      break;
    case 5 :
      transome.TransomeSpeed = 2.4;
      transome.PostureIntegral = 1;    //角度积分开启          
      Findline.errBuff =18*8.5/steerK_p;//gyro.TurnAngle_Integral<5?(35*8.5/steerK_p):0;
      if( sensor.Parallel_sum>10 && Road_or_Unroad() == 1 && transome.OdometerCount<=0 && gyro.TurnAngle_Integral<5 )
      {                  
        Findline.Process=Normal, transome.Process = 1;
        Findline.Odometer = 0.4;
        transome.Finishflag = 1;
        
        Elemental_chain();
//        elementArray.elementFinishNum++;
//        if(elementArray.elementFinishNum>elementArray.elementNum)
//          elementArray.elementFinishNum = 0;
      }
      break;
    }
  }
  else
  {
    switch (transome.Process)
    {
    case 1 : //拐出赛道
      transome.TransomeSpeed = speed.Stan;
      transome.PostureIntegral = 1;        
      Findline.errBuff = 17*8.5/steerK_p;
      //Findline.err[0] =4*(20 - gyro.TurnAngle_Integral);
      if(transome.OdometerCount<=0 && gyro.TurnAngle_Integral <- 60)
      {
        bee.time=200;
        transome.Process = 2;
        transome.OdometerCount = 0.3;
      }
      break;
    case 2 :  // 回正到平行赛道
      transome.TransomeSpeed = speed.Stan;
      transome.PostureIntegral = 1;       
      Findline.errBuff = -17*8.5/steerK_p;
      //Findline.err[0] =4*(20 - gyro.TurnAngle_Integral);
      if(transome.OdometerCount<=0 && gyro.TurnAngle_Integral > 0)
      {
        bee.time=200;
        transome.Process = 3;
        transome.OdometerCount = 0.2;
      }
      break;
      
    case 3 :  //平行赛道
      transome.TransomeSpeed = speed.Stan;
      transome.PostureIntegral = 1;     
      if(fabs(gyro.TurnAngle_Integral)>35)
        Findline.errBuff = (gyro.TurnAngle_Integral)>0?35:-35;          
      else
        Findline.errBuff = (gyro.TurnAngle_Integral);//0;
      if(transome.OdometerCount<=0)
      {
        transome.Process = 4;
        bee.time=200;
        transome.OdometerCount=0.2;//0.40
      }
      break;
    case 4 :  //拐回赛道
      transome.TransomeSpeed = speed.Stan;
      transome.PostureIntegral = 1;           
      Findline.errBuff = -10*8.5/steerK_p;//21
      //Findline.err[0] =4*(20 - gyro.TurnAngle_Integral);
      if(transome.OdometerCount<=0 && gyro.TurnAngle_Integral>58 )//70//50
      {
        transome.Process = 5;
        bee.time=200;
        transome.OdometerCount=0.3;
      }
      break;
    case 5 :
      transome.TransomeSpeed = speed.Stan;
      transome.PostureIntegral = 1;    //角度积分开启          
      Findline.errBuff = gyro.TurnAngle_Integral>5?(21*8.5/steerK_p):0;//>(-gyro.TurnAngle_Integral)?15:0;
      if( (sensor.Parallel_sum>10 && Road_or_Unroad() == 1 )&& gyro.TurnAngle_Integral<15)
      {          
        //            transome.Process = 7;
        //          bee.time=200;
        //          transome.OdometerCount=0.2;
        
        Findline.Process=Normal, transome.Process = 0;
        Findline.Odometer = 0.4;
        transome.Finishflag = 1;
        
        Elemental_chain();
//        elementArray.elementFinishNum++;
//        if(elementArray.elementFinishNum>elementArray.elementNum)
//          elementArray.elementFinishNum = 0;
      }
      break;
    }
  }
}