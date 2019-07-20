#include "sensor.h"



/****Definitions**********************************************/

/****Variables************************************************/
/*

    all stand value 1030

*/
/****Function list********************************************/

void Sensor_AdcInit(void) ;
void Sensor_dealSensor(void) ;
void Sensor_getAdc(void) ;

/****Objects**************************************************/

SENSOR_CLASS  sensor =
{
  &Sensor_AdcInit,
  &Sensor_dealSensor,
	
};
/****Functions************************************************/

void Sensor_AdcInit(void)
{
	adc_init(ADC1_SE8);
	adc_init(ADC1_SE9);
//	adc_init(ADC1_SE10);
	adc_init(ADC1_SE11);
	adc_init(ADC1_SE12);
	adc_init(ADC1_SE13);
	adc_init(ADC0_SE13);
	adc_init(ADC1_DP0);
	adc_init(ADC1_DM0);
	
	port_init(GanHuangGuanPT_x, IRQ_RISING | PF | ALT1 | PULLDOWN );
    
	//sensor.distance_err_max_val=1000;
    
	for(int i=0;i <= 7;i++)   //MagneticInit() 将最大值改为侧到的最大值
	{
		sensor.advalue.ad_max_val[i]=4095;
	}
    
}

void Sensor_dealSensor(void)
{
	uint8 i;       
	Sensor_getAdc(); //滤波后AD值
    
	for(i = 2; i <= 6; i++)
	{
		sensor.once_uni_ad[i] = 100.0f * (sensor.advalue.ad_avr_val[i] ) / 4095.0;//sensor.advalue.ad_max_val[i];
		
	}
	//激光一次归一化
	//sensor.once_uni_ad[0] = ((float)(sensor.advalue.ad_avr_val[5] * 100.0f) / (float)sensor.advalue.ad_max_val[5]);
	
	sensor.Parallel_sum = sensor.once_uni_ad[EEEL] + sensor.once_uni_ad[EEEM] + sensor.once_uni_ad[EEER];
	
	for(uint8 i=59;i>0;i--)
		sensor.MidEEE[i] = sensor.MidEEE[i-1];
	sensor.MidEEE[0] = sensor.once_uni_ad[EEEM];
	
	for(uint8 i=0;i<8;i++)
		sensor.ad_mid_dif[i] = sensor.MidEEE[2*i]-sensor.MidEEE[2*(i+1)];
	
    /*   -------------------------------       计算电感偏差 -----------------------------       */
    for(i = 3; i > 0; i--)
        sensor.Gain[i] = sensor.Gain[i-1];
    for(i = 3; i > 0; i--)
        sensor.error[i] = sensor.error[i-1];
    
	// 计算增益
    sensor.Gain[0] = 95 / sensor.once_uni_ad[EEEM];
    sensor.Gain[0] = sensor.Gain[0]*0.4 + sensor.Gain[1]*0.3 + sensor.Gain[2]*0.2 + sensor.Gain[3]*0.1;
    Constrain_32(&sensor.Gain[0], 2.0, 0.6);
	// 计算偏差
    sensor.error[0] =  35 * (sensor.once_uni_ad[EEER] - sensor.once_uni_ad[EEEL]) / (sensor.once_uni_ad[EEER] + sensor.once_uni_ad[EEEL] + 6.5*sensor.once_uni_ad[EEEM] + 0.00001);
//    sensor.error[0] *= sensor.Gain[0];
    
    sensor.err = sensor.error[0];
}

void Sensor_getAdc(void)
{
	uint8 i = 0, j = 0;
	//先对其和进行清零
	for (i = 0; i < 10; i++)
	{
		sensor.advalue.ad_add_val[i] = 0;
	}
        
	for (i = 0; i < 3; i++)
	{
		sensor.advalue.ad_avr_temp[i][2] = adc_once(ADC1_SE9, ADC_12bit);//左竖 ADC0_DP1
		sensor.advalue.ad_avr_temp[i][3] = adc_once(ADC1_SE12, ADC_12bit);//右竖 ADC0_DM0
		
		sensor.advalue.ad_avr_temp[i][4] = adc_once(ADC1_SE13, ADC_12bit);//左 ADC0_DM1
		sensor.advalue.ad_avr_temp[i][5] = adc_once(ADC1_SE11, ADC_12bit);//中 ADC1_DM0
		sensor.advalue.ad_avr_temp[i][6] = adc_once(ADC1_SE8, ADC_12bit);//右 ADC1_DP0
		sensor.advalue.ad_avr_temp[i][7] = adc_once(ADC0_SE13, ADC_12bit);//激光 ADC1_DP1
	}
        
	for (i = 0; i <= 7; i++)
	{
		for (j = 0; j < 3; j++)
		{
			sensor.advalue.ad_add_val[i] += sensor.advalue.ad_avr_temp[j][i];
		}
		sensor.advalue.ad_avr_val[i] = sensor.advalue.ad_add_val[i] / 3;
	}
 
}

void Magnet_Init(void)
{
	while(1) //测定零漂
	{
		Sensor_getAdc();
		sensor.advalue.ad_offset_val[EECL] = sensor.advalue.ad_avr_val[EECL];
		sensor.advalue.ad_offset_val[EECR] = sensor.advalue.ad_avr_val[EECR];
		sensor.advalue.ad_offset_val[EEEL] = sensor.advalue.ad_avr_val[EEEL];
		sensor.advalue.ad_offset_val[EEEM] = sensor.advalue.ad_avr_val[EEEM];
		sensor.advalue.ad_offset_val[EEER] = sensor.advalue.ad_avr_val[EEER];
		if(sensor.advalue.ad_offset_val[EEEL] < 250 && sensor.advalue.ad_offset_val[EEEM] < 250 && sensor.advalue.ad_offset_val[EEER] < 250 && sensor.advalue.ad_offset_val[EECL] < 250 && sensor.advalue.ad_offset_val[EECR] < 250)
		{
			break;
		}
		else
		{
			oled.Printf(20, 4, "Error MagInit");
		}
	}
	oled_clear();
	oled.Printf(30, 2, "If Over");
    oled.Printf(30, 4, "Press KEY1");
    for(int i=2;i <= 6;i++)  
	{
		sensor.advalue.ad_max_val[i]=0;
	}
	while(1)
	{
		Sensor_getAdc();
		if(sensor.advalue.ad_avr_val[EEEL] > sensor.advalue.ad_max_val[EEEL])    //当左电感为最大值 记录中间电感值
		{
			sensor.advalue.ad_max_val[EEEL] = sensor.advalue.ad_avr_val[EEEL];
		}
		if(sensor.advalue.ad_avr_val[EEEM] > sensor.advalue.ad_max_val[EEEM])
		{
			sensor.advalue.ad_max_val[EEEM] = sensor.advalue.ad_avr_val[EEEM];
		}
		if(sensor.advalue.ad_avr_val[EEER] > sensor.advalue.ad_max_val[EEER])
		{
			sensor.advalue.ad_max_val[EEER] = sensor.advalue.ad_avr_val[EEER];
		}
		if(sensor.advalue.ad_avr_val[EECL] > sensor.advalue.ad_max_val[EECL])
		{
			sensor.advalue.ad_max_val[EECL] = sensor.advalue.ad_avr_val[EECL];
		}
		if(sensor.advalue.ad_avr_val[EECR] > sensor.advalue.ad_max_val[EECR])
		{
			sensor.advalue.ad_max_val[EECR] = sensor.advalue.ad_avr_val[EECR];
		}
		if (OK_IN == 0)
		{
			for(int16 i = 0; i < 19000; i++)
			{
				for(int16 j = 0; j < 100; j++)
				{
					asm("nop");
				}
			}
			if(OK_IN == 0)
			{
				break;
			}
		}
	}
}