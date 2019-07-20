#ifndef __INCLUDES_H
#define __INCLUDES_H

/******************************************************************************/

#include "headfile.h"
#include "math.h"


#include "task.h"
#include "led.h"
#include "oled.h"//#include "IPS_OLED.h"
#include "keys.h"
#include "UI.h"
#include "UIStrings.h"
#include "bee.h"
#include "uart.h"
#include "camera.h"
#include "searchline.h"
#include "sensor.h"
#include "motor.h"
#include "control.h"
#include "gyroscope.h"
#include "APP_Filter.h"
#include "crossmeeting.h"
#include "laser.h"
#include "transome.h"
#include "finalstop.h"
#include "function.h"



/******************************************************************************/
#define DeBugMode       0
#define RunMode         1
#define Road_or_Unroad() !gpio_get(C2)
extern uint8 switch_mode,debugTime;

#endif /* __INCLUDES_H */
