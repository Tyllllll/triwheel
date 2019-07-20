#ifndef __CONTROL_H
#define __CONTROL_H

#include "includes.h"
extern float pout;
typedef struct{
  float Reality;                          //速度偏差
  float AngleError;                     //角度偏差
  float Stan;                           //标准速度  
  float AngleStan;                      //标准角度
  float L_ControlIntegral;
  float R_ControlIntegral;
  float lowest;                    //速度设定最小值
  float highest;
  float L_Bigeest;
  float R_Bigeest;
  float speedErr[5];
  float distance;
  float Distance_cnt;
}Speed_struct;
extern Speed_struct speed;

typedef struct
{
  void (*PIDInit)();
  void (*SpeedInit)();
  void (*MotorControl)(float* err);

} CONTROL_CLASS;
extern CONTROL_CLASS control;

typedef struct
{
    float Expect;//期望
    float Reality;//实际
    float NowError;//当前时刻偏差；
    float LastError;//前一时刻偏差； 
    float Intigral;//jifen 
    float dt;
    float Kp;//控制参数Kp
    float Ki;//控制参数Ki
    float Kd;//控制参数Kd	 
}PID_Parameter;//控制器参数 


extern PID_Parameter speedloop; 
extern PID_Parameter direloop;
extern PID_Parameter steerloop; 
extern PID_Parameter steerWloop; 
extern PID_Parameter leftCurrentloop; 
extern PID_Parameter rightCurrentloop; 

extern float  l_setspeed, r_setspeed,setangle,common_out,differential_out,L_SpeedControlOutUpdata,R_SpeedControlOutUpdata,steerK_p;
extern uint16 loseflagCounter;
extern float Diff_speed;


#endif