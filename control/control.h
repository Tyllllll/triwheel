#ifndef __CONTROL_H
#define __CONTROL_H

#include "includes.h"
extern float pout;
typedef struct{
  float Reality;                          //�ٶ�ƫ��
  float AngleError;                     //�Ƕ�ƫ��
  float Stan;                           //��׼�ٶ�  
  float AngleStan;                      //��׼�Ƕ�
  float L_ControlIntegral;
  float R_ControlIntegral;
  float lowest;                    //�ٶ��趨��Сֵ
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
    float Expect;//����
    float Reality;//ʵ��
    float NowError;//��ǰʱ��ƫ�
    float LastError;//ǰһʱ��ƫ� 
    float Intigral;//jifen 
    float dt;
    float Kp;//���Ʋ���Kp
    float Ki;//���Ʋ���Ki
    float Kd;//���Ʋ���Kd	 
}PID_Parameter;//���������� 


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