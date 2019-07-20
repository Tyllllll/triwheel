#include "control.h"
/****Definitions**********************************************/

/****Variables************************************************/

//static float  L_PreError[50] = { 0 };
//static float  R_PreError[50] = { 0 };
//static float  A_PreError[50] = { 0 };
float  speedParam,l_setspeed, r_setspeed,
        setangle,lastangle,common_out,differential_out,
        KpGain,FxMax,steerK_p,steerK_d;

float  L_SpeedControlOutUpdata;                 //左电机更新速度
float  R_SpeedControlOutUpdata;                 //右电机更新速度
uint16  loseflagCounter;
uint8 timer;

/****Function list********************************************/
void PIDInit();
void SpeedInit();
void MotorControl(float* err);
void CurrentLoopSimple();

/****Objects**************************************************/
PID_Parameter speedloop;
PID_Parameter direloop;
PID_Parameter steerloop; 
PID_Parameter steerWloop; 
PID_Parameter leftCurrentloop; 
PID_Parameter rightCurrentloop; 
Speed_struct speed;


CONTROL_CLASS control =
{
    &PIDInit,
    &SpeedInit,
    &MotorControl,
};
/****Functions************************************************/
void PIDInit()
{
  //速度环
    speedloop.Kp=1200;//1200
    speedloop.Ki=100;//100
    speedloop.Kd=0;
    speedloop.dt=1;
  //方向环  
    steerloop.Kp=0;
    steerloop.Ki=0;
    steerloop.Kd=0;//30
    steerloop.dt=1;
    steerK_p= 10;//6.6
    steerK_d= 11;//4.5//28
    KpGain = 1.5;//1.4;
    FxMax = 5500;//4800;//2最大向心力限制
  //方向角速度环
    steerWloop.Kp=10.0;//3.9//7//8
    steerWloop.Ki=0;
    steerWloop.Kd=50;//22;//100;0;//
    steerWloop.dt=1;
  //左電機電流环  
    leftCurrentloop.Kp=8;
    leftCurrentloop.Ki=0.6;
    leftCurrentloop.Kd=0;
    leftCurrentloop.dt=0.5;
    //右電機電流环  
    rightCurrentloop.Kp=8;
    rightCurrentloop.Ki=0.6;
    rightCurrentloop.Kd=0;
    rightCurrentloop.dt=0.5;
 
}

void SpeedInit()
{
	speed.Stan= 2.5;//2.7;
	Findline.loseflag = 1;
	
	speed.L_Bigeest=9500;
	speed.R_Bigeest=9500;
    
}

float PID_Controler(PID_Parameter* p)
{
	static float pout=0;
	static float dout=0;
	static float pidout=0; 
	
	p->LastError = p->NowError;
	p->NowError = p->Expect - p->Reality;
	pout = p->Kp * p->NowError;
	p->Intigral += p->Ki * (p->NowError * p->dt);
	
    if(fabs(p->Intigral) > 9000)
		p->Intigral = p->Intigral>0 ? (9000) : -(9000);
    dout = p->Kd * (p->NowError - p->LastError) / p->dt;
    pidout = pout + p->Intigral + dout;//位置输出			
		
    return pidout;
}

void SpeedLoop()
{
	if(timer%10==0)
	{
		// ;//
		switch (Findline.Process)
		{
		case Crossmeeting:
            speedloop.Expect = crossmeet.MeetingSpeed;
            break;
        case Transome:
            speedloop.Expect = transome.TransomeSpeed;
            break;
		default:
			speedloop.Expect = speed.Stan;//0.8*speed.Stan + 0.2*speed.Stan*(35-fabs(Findline.errBuff))/35.0;//
			break;
		}
		speedloop.Reality = speed.Reality;
		//电机输出
                if(speedloop.Intigral > 500 && speedloop.NowError<0)
                speedloop.Intigral = 0;
                if(speedloop.Intigral < -500 && speedloop.NowError>0)
                speedloop.Intigral = 0;
		common_out = PID_Controler(&speedloop);
		if(common_out>250)common_out=250;
	}
}



void SteeringLoop()
{
	static float deltaErr;
	if((timer)%5 == 0)
	{
		//方向环
		steerloop.Expect = 0;
		steerloop.Reality =Findline.errBuff ; 
		speedParam = speed.Reality;
		if(speedParam<0.1)
			speedParam = 0.1;
		if(speedParam>4)
			speedParam=4;
//			speedParam=2;
		steerloop.Kp = steerK_p 
			*speedParam
				*KpGain;
	}
    
	//方向角速度环
	deltaErr = Findline.errBuff - Findline.err[4];
	if(fabs(deltaErr) > 20)
		deltaErr = deltaErr > 0 ? 20:-20;
        
//	if(deltaErr * Findline.errBuff > 0 ) 
		steerWloop.Expect = PID_Controler(&steerloop) - steerK_d * deltaErr * speedParam / 100.0;//1500;//
//                if(steerWloop.Expect > 500 ) steerWloop.Expect = 500;
//                if(steerWloop.Expect < -500 )steerWloop.Expect = -500;
//	else
//		steerWloop.Expect = PID_Controler(&steerloop) - steerK_d * deltaErr * speedParam / 100.0/2; 
    
    static float lastWReality[8],WRealErr; 
	for(uint8 i = 7;i > 0; i--)
		lastWReality[i] = lastWReality[i-1];
	lastWReality[0] = steerWloop.Reality ;
	steerWloop.Reality = gyro.Turn_Speed;
	WRealErr = -(steerWloop.Reality - lastWReality[7]);
	//电机输出差速
	differential_out = PID_Controler(&steerWloop);// -1*steerWloop.Reality ;
	
	static float FxMaxOut;
	FxMaxOut = 900; //FxMax / (speed.Reality * speed.Reality + 0.0001);
        
	if(differential_out>FxMaxOut)
        differential_out=FxMaxOut;
	if(differential_out<-FxMaxOut)
        differential_out=-FxMaxOut;
}

void CurrentLoop()
{
	static int currentOffSetLeft = 3135,
	currentOffSetRight = 3138;
	
	CurrentLoopSimple();//电流采样
	if(Findline.Process == Transome && transome.Shuaaaaflag == 1)
	{            
		//左电机速度更新
		leftCurrentloop.Expect =  2*(gyro.TurnAngle_Integral + 45);  // 00;//        
		leftCurrentloop.Reality = -((fabs(sensor.advalue.ad_avr_val[1]-currentOffSetLeft)>5)?(sensor.advalue.ad_avr_val[1]-currentOffSetLeft):0) ;//leftCurrentloop.Intigral ;//             
		L_SpeedControlOutUpdata = PID_Controler(&leftCurrentloop); 
		
		//右电机速度更新
		rightCurrentloop.Expect = 0;  //400;//
		rightCurrentloop.Reality = ((fabs(sensor.advalue.ad_avr_val[0]-currentOffSetRight)>5)?(sensor.advalue.ad_avr_val[0]-currentOffSetRight):0) ; //rightCurrentloop.Intigral ;//       
		R_SpeedControlOutUpdata = PID_Controler(&rightCurrentloop);
		
	}
	else
	{
		//左电机速度更新
		leftCurrentloop.Expect = common_out - 1*differential_out; //-800;//    00;//        
		leftCurrentloop.Reality = ((fabs(sensor.advalue.ad_avr_val[1]-currentOffSetLeft)>5)?(sensor.advalue.ad_avr_val[1]-currentOffSetLeft):0) ;//leftCurrentloop.Intigral ;//             
		L_SpeedControlOutUpdata = PID_Controler(&leftCurrentloop); // 9999;//
		
		//右电机速度更新
		rightCurrentloop.Expect = common_out + 1*differential_out; //0;//400;//
		rightCurrentloop.Reality = -((fabs(sensor.advalue.ad_avr_val[0]-currentOffSetRight)>5)?(sensor.advalue.ad_avr_val[0]-currentOffSetRight):0) ; //rightCurrentloop.Intigral ;//       
		R_SpeedControlOutUpdata = PID_Controler(&rightCurrentloop); //
		
	}
	
}

float DerictionControl(float* err)
{
	static float dif_speed[4];
    float pout, dout;
    const float  d_max = 170;
    float Diff_speed;
    float p, d;
	sensor.d_err[1] = sensor.d_err[0];
	sensor.d_err[0] = (err[0] - err[5]) * 5;
	Constrain_32(&sensor.d_err[0], d_max, -d_max);//微分项限幅 
     
//	p = direloop.Kp;
//	d = direloop.Kd;
	p = direloop.Kp * (18 + 4 * (float)(Fuzzy_L(err[0], sensor.d_err[0])) / 100.0) / 22;
	d = direloop.Kd * (10 + 1 * (float)(Fuzzy_L(err[0], sensor.d_err[0])) / 100.0) / 11;
//    d = 10;
//	if(Sensor_v al.cir_flag)
//	{
//		p = 9.0f;
//		d = 7.0f;             
//	}
//	err[0] = err[0] * 0.4 + err[1] * 0.3 + err[2] * 0.1 + err[3] * 0.1 + err[4] * 0.1;

    pout = err[0] / 20.0 * p *0.1;



	dout = sensor.d_err[0] / d_max * d * 0.1;

    for(uint8 i=3;i>0;i--)
      dif_speed[i]=dif_speed[i-1];
	dif_speed[0] = pout + dout;

    Diff_speed = dif_speed[0];

	return Diff_speed;
}

float Diff_speed;
void MotorControl(float* err)
{
	static uint16  Counter;
	
	
	Counter++;
	if(Counter>=2) //1ms
	{
		
		Counter=0;
		//速度环
		SpeedLoop();
	    //方向环计算
		SteeringLoop();  
		timer = ++timer>=20?0:timer;
		
	}
	//电流环计算
	CurrentLoop();
	

	if(
	   (sensor.advalue.ad_avr_val[EEEL] + sensor.advalue.ad_avr_val[EEEM] + sensor.advalue.ad_avr_val[EEER]  <= 350
		//             || (Findline.Process == Lamp_D && fabs(gyro.Car_Angle)>44)
		|| !ui.startbutton_flag || motor.crazyRunCounter>=100 )
		   && Findline.Process <= Transome_or_Crossmeeting
			   )
	{
		loseflagCounter++;
		if(loseflagCounter>250)
			Findline.loseflag = 1 ;
	}
	else 
	{
		if(loseflagCounter>250)
			Findline.loseflag = 1 ;
		Findline.loseflag =0;
		loseflagCounter=0;
	}
	
	if(Findline.Process == Crossmeeting && crossmeet.dejavuflag == 1)
	{            
		//左电机速度更新
		L_SpeedControlOutUpdata = -40*4*(-gyro.TurnAngle_Integral + 180); //    00;//
		if(L_SpeedControlOutUpdata <-4000) L_SpeedControlOutUpdata = -4000;  
		if(L_SpeedControlOutUpdata > 4000) L_SpeedControlOutUpdata = 4000;
		
		//右电机速度更新
		R_SpeedControlOutUpdata = 40*4*(-gyro.TurnAngle_Integral + 180);
		if(R_SpeedControlOutUpdata <-4000) R_SpeedControlOutUpdata = -4000;
		if(R_SpeedControlOutUpdata > 4000) R_SpeedControlOutUpdata = 4000;
	}
//	else if(Findline.Process == Crossmeeting && crossmeet.dejavuflag == 2)
//	{
//		//左电机速度更新
//		L_SpeedControlOutUpdata = -60*4*(-gyro.TurnAngle_Integral + 180); //    00;//
//		if(L_SpeedControlOutUpdata <-4000) L_SpeedControlOutUpdata = -4000;  
//		if(L_SpeedControlOutUpdata > 4000) L_SpeedControlOutUpdata = 4000;
//		
//		//右电机速度更新
//		R_SpeedControlOutUpdata = 60*4*(-gyro.TurnAngle_Integral + 180);
//		if(R_SpeedControlOutUpdata <-4000) R_SpeedControlOutUpdata = -4000;
//		if(R_SpeedControlOutUpdata > 4000) R_SpeedControlOutUpdata = 4000;  
//	}
//	if (Findline.Process == Crossmeeting && crossmeet.dejavuflag == 1)
//	{
//		L_SpeedControlOutUpdata =  5500 * (1  - fabs(gyro.TurnAngle_Integral) / 180);
//		R_SpeedControlOutUpdata =  -5500 * (1 - fabs(gyro.TurnAngle_Integral) / 180);
//	}
	else if (Findline.Process == Crossmeeting && crossmeet.dejavuflag == 2)
	{
		L_SpeedControlOutUpdata =  sensor.error[0] * 500;
		R_SpeedControlOutUpdata =  -sensor.error[0] * 500;
	}
	
	if (crossmeet.dejavuflag == 0)
		if(Findline.loseflag || motor.crazyRunCounter>=100 )
		{
			L_SpeedControlOutUpdata =  0;
			R_SpeedControlOutUpdata =  0;        
		}
	
	if(R_SpeedControlOutUpdata>=0)  
	{ 
		MotorPwm[0] =(int16)(R_SpeedControlOutUpdata>speed.R_Bigeest?speed.R_Bigeest:R_SpeedControlOutUpdata);
		MotorPwm[1] = 0;
	}
	if(R_SpeedControlOutUpdata<0)
	{
		MotorPwm[0] =0;
		MotorPwm[1] =(int16)(-R_SpeedControlOutUpdata>speed.R_Bigeest?speed.R_Bigeest:-R_SpeedControlOutUpdata);
	}
	if(L_SpeedControlOutUpdata>=0)
	{
		MotorPwm[2] =(int16)(L_SpeedControlOutUpdata>speed.L_Bigeest?speed.L_Bigeest:L_SpeedControlOutUpdata); 
		MotorPwm[3] =0;
	}
	if(L_SpeedControlOutUpdata<0)
	{ 
		MotorPwm[2] = 0;
		MotorPwm[3] =(int16)(-L_SpeedControlOutUpdata>speed.L_Bigeest?speed.L_Bigeest:-L_SpeedControlOutUpdata);
	}
	
	motor.MotorPwmFlash(MotorPwm);
}

void CurrentLoopSimple()
{
	uint8 i,j;
	static float filterBuff1,filterBuff0;
	for (i = 0; i < 2; i++)
	{
		sensor.advalue.ad_add_val[i] = 0;
	}
	for (i = 0; i < 3; i++)
	{
		
		sensor.advalue.ad_avr_temp[i][0] = adc_once(ADC1_DP0, ADC_12bit);//左电流
		sensor.advalue.ad_avr_temp[i][1] = adc_once(ADC1_DM0, ADC_12bit);//右电流
	}
	
	for (i = 0; i <= 1; i++)
	{
		for (j = 0; j < 3; j++)
		{
			sensor.advalue.ad_add_val[i] += sensor.advalue.ad_avr_temp[j][i];
		}
		sensor.advalue.ad_avr_val[i] = sensor.advalue.ad_add_val[i] / 3;
		//滤波
		if(i==1)
		{
			LPF_1_db(30,0.0005,(float)(sensor.advalue.ad_avr_val[i]),&filterBuff0);
			sensor.advalue.ad_avr_val[i] = (int16)filterBuff0;
		}
		else                  
		{
			LPF_1_db(30,0.0005,(float)(sensor.advalue.ad_avr_val[i]),&filterBuff1);
			sensor.advalue.ad_avr_val[i] = (int16)filterBuff1;
		}
	}
}
//
////************************************
//// Method:    Fuzzy
//// FullName:  Fuzzy
//// Access:    public 
//// Returns:   float
//// Qualifier: 模糊控制
//// Parameter: void
////************************************
//
//#define FMAX    100 //语言值得满赋值
//
//int pFF[7] = { -200,-100,-50,0,50,100,200};
//int DFF[5] = { -100, -40,0,40,100};
//int UFF[7] = {  0,  0,  5,  10,   25,   50,  100};
////int UFF[7] = { 000, 111,  321,  725, 800, 840, 880};
////              0   1   2   3    4     5    6
////int pFF[7] = { -500,-325,-150,0,150,325,500};
////int DFF[5] = { -375, -100,0,100,375};
////int UFF[7] = { 39, 219,  385,  405, 705, 1405, 1675};
//////              0    1     2     3    4     5    6
////       //p =   1.2 1.57 1.90 2.19 2.49 2.69 2.99
///*采用了调整因子的规则表,大误差时偏重误差,小误差时偏重误差变化*/  
//int rule[7][5]={
//	//  ec小于-8为-2  ec大于-8小于0为-1  ec大于0小于8为0    ec>8为1 
//	
//	{ 6,  5,  3,  0,  0}, //   -24   
//	{ 5,  4,  2,  0,  0}, //   -12   
//	{ 4,  3,  2,  0,  0}, //   -8 
//	{ 3,  1,  0,  1,  3}, //   0   
//	{ 0,  0,  2,  2,  4}, //   8    
//	{ 0,  0,  2,  4,  5}, //   12    
//	{ 0,  0,  3,  5,  6}  //   24
//	//  ec小于-8为-2  ec大于-8小于0为-1  ec大于0小于8为0    ec>8为1 
//	//  /*2.8m*/
//	//  { 6,  6,  5,  4,  3}, //   -50   
//	//  { 5,  4,  4,  4,  3}, //   -32   
//	//  { 4,  4,  3,  3,  1}, //   -15 
//	//  { 0,  1,  6,  1,  0}, //   0   
//	//  { 1,  3,  3,  4,  4}, //   15    
//	//  { 3,  4,  4,  4,  5}, //   32    
//	//  { 4,  4,  5,  6,  6}  //   50
//	//int pFF[7] = { -750,-650,-300,0,300,650,750};
//	//int DFF[5] = { -450, -150,0,150,450};
//	//int UFF[7] = { 289, 325,  339,  369, 465, 539, 589};
//	//  { 6,  6,  5,  3,  1}, //   -50   
//	//  { 5,  5,  5,  5,  3}, //   -25   
//	//  { 3,  4,  4,  3,  2}, //   -14 
//	//  { 0,  2,  3,  2,  0}, //   0   
//	//  { 2,  3,  4,  4,  3}, //   14    
//	//  { 3,  5,  5,  5,  5}, //   25    
//	//  { 1,  3,  5,  6,  6}  //   50
//};
//
//int    U;           /*偏差,偏差微分以及输出值的精确量*/
//unsigned int   pF[2]={0}, DF[2]={0}, UF[4]={0};   /*偏差,偏差微分以及输出值的隶属度*/
//int    Pn, Dn, Un[4];
//float   temp1, temp2;
//float  a1=0,a2=0,a3=0,a4=0;
///**********************************************************/
//int   Fuzzy(int P, int D)   /*模糊运算引擎*/
//{
//	/*隶属度的确定*/
//	/*根据PD的指定语言值获得有效隶属度*/
//	if (P>pFF[0] && P<pFF[6])
//	{
//		if (P <= pFF[1])
//		{
//			Pn = -2;
//			pF[0] = (int)(FMAX*((float)(pFF[1] - P) / (pFF[1] - pFF[0])));//求p值占该论域的比例即为在该论域的概率
//		}
//		else if (P <=pFF[2])
//		{
//			Pn = -1;
//			pF[0] = (int)(FMAX*((float)(pFF[2] - P) / (pFF[2] - pFF[1])));
//		}
//		else if (P <= pFF[3])
//		{
//			Pn = 0;
//			pF[0] = (int)(FMAX*((float)(pFF[3] - P) / (pFF[3] - pFF[2])));
//		}
//		else if (P <= pFF[4])
//		{
//			Pn = 1; 
//			pF[0] = (int)(FMAX*((float)(pFF[4] - P) / (pFF[4] - pFF[3])));
//		}
//		else if (P <= pFF[5])
//		{
//			Pn = 2; 
//			pF[0] = (int)(FMAX*((float)(pFF[5] - P) / (pFF[5] - pFF[4])));
//		}
//		else if (P <= pFF[6])
//		{
//			Pn = 3; 
//			pF[0] =(int)( FMAX*((float)(pFF[6] - P) / (pFF[6] - pFF[5])));
//		}
//	}
//	else if (P <= pFF[0])
//	{
//		Pn = -2;  
//		pF[0] = FMAX;
//	}
//	else if (P >= pFF[6])
//	{
//		Pn = 3;   pF[0] = 0;
//	}
//	pF[1] = FMAX - pF[0];//求p值不在所在论域的概率
//	
//	
//	if (D > DFF[0] && D < DFF[4])//-10~10
//	{
//		if (D <=DFF[1])//-10~-5
//		{
//			Dn = -2; 
//			DF[0] = (int)(FMAX*((float)(DFF[1] - D) / (DFF[1] - DFF[0])));
//		}
//		else if (D <= DFF[2])//-5~-0
//		{
//			Dn = -1;
//			DF[0] =(int)( FMAX*((float)(DFF[2] - D) / (DFF[2] - DFF[1])));
//		}
//		else if (D <= DFF[3])//0~5
//		{
//			Dn = 0;
//			DF[0] =(int)( FMAX*((float)(DFF[3] - D) / (DFF[3] - DFF[2])));
//		}
//		else if (D <= DFF[4])//5~10
//		{
//			Dn = 1;
//			DF[0] =(int)( FMAX*((float)(DFF[4] - D) / (DFF[4] - DFF[3])));
//		}
//	}
//	else if (D <= DFF[0])
//	{
//		Dn = -2;
//		DF[0] = FMAX;
//	}
//	else if (D >= DFF[4])
//	{
//		Dn = 1;
//		DF[0] = 0;
//	}
//	DF[1] = FMAX - DF[0];
//	/*使用误差范围优化后的规则表rule[7][7]*/
//	/*输出值使用13个隶属函数,中心值由UFF[7]指定*/
//	/*一般都是四个规则有效*/
//	Un[0] = rule[Pn - 1 + 3][Dn - 1 + 3];
//	Un[1] = rule[Pn + 3][Dn - 1 + 3];
//	Un[2] = rule[Pn - 1 + 3][Dn + 3];
//	Un[3] = rule[Pn + 3][Dn + 3];
//	
//	if (pF[0] <= DF[0])
//		UF[0] = pF[0];//Un[0]取值的概率应该为p所在论域的概率和d所在的论域概率的最小值
//	else
//		UF[0] = DF[0];
//	
//	if (pF[1] <= DF[0])
//		UF[1] = pF[1];//Un[1]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d所在的论域概率的最小值
//	else
//		UF[1] = DF[0];
//	
//	if (pF[0] <= DF[1])
//		UF[2] = pF[0];//Un[2]取值的概率应该为p所在论域的概率和d不在所在的论域（在相邻右边论域）概率的最小值
//	else
//		UF[2] = DF[1];
//	
//	if (pF[1] <= DF[1])  //Un[3]取值的概率应该为p不在所在论域（在相邻右边论域）的概率和d不在所在的论域（在相邻右边论域）概率的最小值
//		UF[3] = pF[1];
//	else
//		UF[3] = DF[1];
//	/*同隶属函数输出语言值求大*/
//	
//	if (Un[0] == Un[1])
//	{
//		if (UF[0] > UF[1])//UF[0] > UF[1]即为在Un[0]的概率大于Un[1]的概率
//			UF[1] = 0;//概率小的清零
//		else
//			UF[0] = 0;
//	}
//	if (Un[0] == Un[2])
//	{
//		if (UF[0] > UF[2])
//			UF[2] = 0;
//		else
//			UF[0] = 0;
//	}
//	if (Un[0] == Un[3])
//	{
//		if (UF[0] > UF[3])
//			UF[3] = 0;
//		else
//			UF[0] = 0;
//	}
//	if (Un[1] == Un[2])
//	{
//		if (UF[1] > UF[2])
//			UF[2] = 0;
//		else
//			UF[1] = 0;
//	}
//	if (Un[1] == Un[3])
//	{
//		if (UF[1] > UF[3])
//			UF[3] = 0;
//		else
//			UF[1] = 0;
//	}
//	if (Un[2] == Un[3])
//	{
//		if (UF[2] > UF[3])
//			UF[3] = 0;
//		else
//			UF[2] = 0;
//	}
//	
//	a1=UF[0]*UFF[Un[0]];  
//	a2=UF[1]*UFF[Un[1]];
//	a3=UF[2]*UFF[Un[2]];
//	a4=UF[3]*UFF[Un[3]];
//	
//	temp1 = a1+a2+a3+a4;
//	temp2 = UF[0] + UF[1] + UF[2] + UF[3];
//	U = (int)(temp1 / temp2);//加权平均
//	return U;
//}
///****************************************
//*
//*               速度係數模糊
//*
//****************************************/
//int SFF[7] = { 10 , 100, 150, 200, 250, 300, 400,};
//int OFF[7] = { 5 , 50 , 75 , 100, 112, 125, 150,};
////int OFF[7] = { 25 , 50 , 75 , 100, 125, 150, 175,};
//int speedOut;
//unsigned int   SF[2]={0};
//int    Sn, On[2];
//float   temp3, temp4;
//float  s1=0, s2=0;
//int speedRule[7]={
//	0, //0m
//	1, //1m
//	2, //1.5m
//	3, //2m
//	4, //2.5m
//	5, //3m
//	6, //3.5m
//};
//int   SpeedFuzzy(int S)   /*模糊运算引擎*/
//{
//	/*隶属度的确定*/
//	/*根据S的指定语言值获得有效隶属度*/
//	if (S>SFF[0] && S<SFF[6])
//	{
//		if (S <= SFF[1])
//		{
//			Sn = 0;
//			SF[0] = (int)(FMAX*((float)(SFF[1] - S) / (SFF[1] - SFF[0])));
//		}
//		else if (S <=SFF[2])
//		{
//			Sn = 1;
//			SF[0] = (int)(FMAX*((float)(SFF[2] - S) / (SFF[2] - SFF[1])));
//		}
//		else if (S <= SFF[3])
//		{
//			Sn = 2;
//			SF[0] = (int)(FMAX*((float)(SFF[3] - S) / (SFF[3] - SFF[2])));
//		}
//		else if (S <= SFF[4])
//		{
//			Sn = 3; 
//			SF[0] = (int)(FMAX*((float)(SFF[4] - S) / (SFF[4] - SFF[3])));
//		}
//		else if (S <= SFF[5])
//		{
//			Sn = 4; 
//			SF[0] = (int)(FMAX*((float)(SFF[5] - S) / (SFF[5] - SFF[4])));
//		}
//		else if (S <= SFF[6])
//		{
//			Sn = 5; 
//			SF[0] =(int)( FMAX*((float)(SFF[6] - S) / (SFF[6] - SFF[5])));
//		}
//	}
//	else if (S <= SFF[0])
//	{
//		Sn = 0;  
//		SF[0] = FMAX;
//	}
//	else if (S >= SFF[6])
//	{
//		Sn = 5;   SF[0] = 0;
//	}
//	SF[1] = FMAX - SF[0];//求p值不在所在论域的概率
//	/*使用误差范围优化后的规则表rule[7]*/
//	/*输出值使用7个隶属函数,中心值由SFF[3]指定*/
//	/*一般都是兩个规则有效*/
//	On[0] = speedRule[Sn];
//	On[1] = speedRule[Sn+1];
//	
//	//        if (On[0] == On[1])
//	//	{
//	//		if (SF[0] > SF[1])
//	//			UF[1] = 0;
//	//		else
//	//			UF[0] = 0;
//	//	}
//	
//	s1=SF[0]*OFF[On[0]];  
//	s2=SF[1]*OFF[On[1]];
//	
//	temp3 = s1+s2;
//	temp4 = SF[0] + SF[1];
//	speedOut = (int)(temp3 / temp4);//加权平均
//	return speedOut;
//}


