#ifndef __LAZER_H
#define __TAZER_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/


/****Variables************************************************/
typedef struct
{
  void (*VL53L0X_SimpleMea)(void);
  float LaserDistanse[20];
}LASER_CLASS;

extern LASER_CLASS laser;
/****Function list********************************************/
void VL53L0X_SimpleMea();
/*************************************************************/

#endif 