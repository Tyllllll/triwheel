#ifndef __SENSOR_H
#define __SENSOR_H

/****Includes*************************************************/

#include "includes.h"

/****GPIOs****************************************************/

/****Definitions**********************************************/
//竖电感
#define  EECL  2       //ADC0_DP0
#define  EECR  5        //ADC0_DM1
//横电感
#define  EEEL  6       //ADC0_DP0
#define  EEEM  4        //ADC1_DM0
#define  EEER  3        //ADC1_DM1

#define	 GanHuangGuanPT_x	B4

/****Variables************************************************/
//ADC
typedef struct
{
	uint32 ad_add_val[8];		//AD采样中值和
	uint16 ad_avr_val[8];		//AD采样平均值
	uint16 ad_max_val[8];		//AD最大值
	uint16 ad_max_temp[8];
	uint16 ad_int_val[8][3];
	uint16 ad_avr_temp[8][10];
	uint16 ad_offset_val[8];
	uint16 ad_offset_val_temp[8];
	
}ADC_val_struct;


//电感
typedef struct
{
	void (*SensorInit)(void);
	void (*GetSensor)(void);
	float	once_uni_ad[8];  //一次归一化处理
	float	twice_uni_ad[8]; //二次归一化处理
	float	error[4];
    float   err;
    float   d_err[2];
	float   Gain[4];                   //Gain[1]竖电感增益 Gain[2]横电感增益
	float  MidEEE[60];
	float  ad_mid_dif[8];
	float Parallel_sum;
	
	ADC_val_struct advalue;
	
}SENSOR_CLASS;

void Magnet_Init(void);

extern float Position_transit_short[3];
extern SENSOR_CLASS  sensor;
extern int8 Position;
/****Functions************************************************/

/*************************************************************/

#endif