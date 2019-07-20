#include "motor.h"

/****Definitions**********************************************/

#define speedMaxErr 0.04

/****Variables************************************************/

int16 Left_count;
int16 Right_count;
float speed_avr;
float L_CarSpeed,R_CarSpeed;
float L_lastCarSpeed,R_lastCarSpeed;
int16 MotorPwm[4];

/****Function list********************************************/
void MotorInit(int16* PWM);
void QdInit(void);
void GetSpeed(void);
void MotorPwmFlash(int16* PWM);

/****Objects**************************************************/

MOTOR_CLASS motor =
{
    &MotorInit,
    &QdInit,
    &GetSpeed,
    &MotorPwmFlash,
};

/****Functions************************************************/
/****************���*******************/
void MotorInit(int16* PWM)
{
	ftm_pwm_init(ftm0, ftm_ch4, 15000, 0);
	ftm_pwm_init(ftm0, ftm_ch5, 15000, 0);
	ftm_pwm_init(ftm0, ftm_ch6, 15000, 0);
	ftm_pwm_init(ftm0, ftm_ch7, 15000, 0);
	
	PWM[0] = 0000;//����
	PWM[1] = 0000;//��
	PWM[2] = 0000;//�Ҹ�
	PWM[3] = 0000;//����
	
}

void QdInit(void)
{
	ftm_quad_init(ftm1);
	ftm_quad_init(ftm2);	
}



void GetSpeed(void)//����������ٶ�
{          
	static int16 l_filter[4],r_filter[4];
	Right_count = -ftm_quad_get(ftm2);
	ftm_quad_clean(ftm2);
	for (int i = 3; i > 0; i--)
		r_filter[i] = r_filter[i - 1];
	r_filter[0]=Right_count;
	
	Left_count = ftm_quad_get(ftm1);
	ftm_quad_clean(ftm1);
	for (int i = 3; i > 0; i--)
		l_filter[i] = l_filter[i - 1];
	l_filter[0]=Left_count;
	
	
	//	//�޸�Ƶ�ʣ���������λ
	L_CarSpeed = L_lastCarSpeed*0.15 + 0.85*(0.4*l_filter[0]+0.3*l_filter[1]+0.2*l_filter[2]+0.1*l_filter[3]) * SPEED_F / L_QD_UNIT;    //������ֵ*����Ƶ��/һ�׼���ֵ���������ת��ΪM/S 
	R_CarSpeed = R_lastCarSpeed*0.15 + 0.85*(0.4*r_filter[0]+0.3*r_filter[1]+0.2*r_filter[2]+0.1*r_filter[3]) * SPEED_F / R_QD_UNIT;    //�������ת��ΪM/S 
	
	if (L_CarSpeed > 8)L_CarSpeed = 8,motor.crazyRunCounter++;//����
	if (R_CarSpeed > 8)R_CarSpeed = 8,motor.crazyRunCounter++;
	if (L_CarSpeed < -8)L_CarSpeed = -8,motor.crazyRunCounter++;//����
	if (R_CarSpeed < -8)R_CarSpeed = -8,motor.crazyRunCounter++;
	
	if(fabs(L_CarSpeed-L_lastCarSpeed)>speedMaxErr)
	{
		L_CarSpeed = L_lastCarSpeed + (L_CarSpeed-L_lastCarSpeed>0?speedMaxErr:-speedMaxErr);
	}
	if(fabs(R_CarSpeed-R_lastCarSpeed)>speedMaxErr)
	{
		R_CarSpeed = R_lastCarSpeed + (R_CarSpeed-R_lastCarSpeed>0?speedMaxErr:-speedMaxErr);
	}
	
	L_lastCarSpeed = L_CarSpeed;
	R_lastCarSpeed = R_CarSpeed;
	
	if(fabs(L_CarSpeed)>=4||fabs(R_CarSpeed)>=4)
	{
		if(fabs(R_CarSpeed) <= fabs(L_CarSpeed))
			speed_avr=R_CarSpeed;
		else
			speed_avr=L_CarSpeed;
	}
	else          
		speed_avr=(L_CarSpeed+R_CarSpeed)/2;
	speed.Reality = speed_avr;
	
	
	for(uint8 i = 4; i>0 ; i-- )
		speed.speedErr[i] = speed.speedErr[i-1];
	//         speed.speedErr[0] = speed_avr;
	LPF_1_db(1,0.005,speed_avr,speed.speedErr);
	
	
	motor.Odometer += speed_avr * 0.005;
	if( Findline.Odometer > 0)
		Findline.Odometer -= speed_avr * 0.005 ;
	else
		Findline.Odometer = 0 ;
    
	if( crossmeet.OdometerCount > 0)
		crossmeet.OdometerCount -= speed_avr * 0.005 ;
	else
		crossmeet.OdometerCount = 0 ;
    
	if( transome.OdometerCount > 0)
		transome.OdometerCount -= speed_avr * 0.005 ;
	else
		transome.OdometerCount = 0 ;
    
	if( finalstop.OdometerCount > 0)
		finalstop.OdometerCount -= speed_avr * 0.005 ;
	else
		finalstop.OdometerCount = 0 ;
    
    if (Lamp_Odometer > 0)
        Lamp_Odometer -= speed_avr * 0.005 ;
    else
        Lamp_Odometer = 0;
    
    if(Findline.Process == Stop)
    {
        speed.Distance_cnt = 0;
    }
    else
    {
        speed.Distance_cnt += speed_avr * 0.005;
    }
    
	if (speed.distance > 0)
	{
		speed.distance -= speed_avr * 0.005;
	}
	else if (speed.distance < 0)
	{
		ui.startbutton_flag = 0;
	}
	
}

void MotorPwmFlash(int16* PWM)
{
	ftm_pwm_duty(ftm0, ftm_ch4, PWM[0]);// ����
	ftm_pwm_duty(ftm0, ftm_ch5, PWM[1]);// �ҷ�
	ftm_pwm_duty(ftm0, ftm_ch6, PWM[2]);// ����
	ftm_pwm_duty(ftm0, ftm_ch7, PWM[3]);// ��
//	ftm_pwm_duty(ftm0, ftm_ch4, 2000);
//	ftm_pwm_duty(ftm0, ftm_ch5, 0);
//	ftm_pwm_duty(ftm0, ftm_ch6, 2000);
//	ftm_pwm_duty(ftm0, ftm_ch7, 0);
}