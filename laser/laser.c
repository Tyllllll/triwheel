#include "includes.h"

LASER_CLASS laser =
{
  &VL53L0X_SimpleMea,
};


void VL53L0X_SimpleMea()
{
  float temp;
  
//  if(((float)sensor.advalue.ad_avr_val[7]-83.99) > 0)
//	  temp= (834.4/((float)sensor.advalue.ad_avr_val[7]-83.99))+0.02308 ;
//  else
//      temp=5;

//  temp = 910/((float)sensor.advalue.ad_avr_val[7]);
  temp = (0.00002 * sensor.advalue.ad_avr_val[7] * sensor.advalue.ad_avr_val[7] - 0.1191 * sensor.advalue.ad_avr_val[7] + 184.06) / 100;
  
  if(gyro.Car_Angle<speed.AngleStan+30) //ÆÂµÀ²â¾à²»×¼
	  laser.LaserDistanse[0] = temp;
  else
      laser.LaserDistanse[0] = 5;
  
  for(uint8 i=19;i>0;i--)
  {
	  laser.LaserDistanse[i] = laser.LaserDistanse[i-1];    
  }
  
}

