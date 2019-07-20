#ifndef __LED_H
#define __LED_H

/****Includes*************************************************/

#include "common.h"

/****Definitions**********************************************/

#define LED1_PTX_n		E9
#define LED2_PTX_n		E10
#define LED3_PTX_n		E11
#define LED4_PTX_n		E12

#define LED1_Toggle()	gpio_turn(LED1_PTX_n)
#define LED2_Toggle()	gpio_turn(LED2_PTX_n)
#define LED3_Toggle()	gpio_turn(LED3_PTX_n)
#define LED4_Toggle()	gpio_turn(LED4_PTX_n)

#define LED1			1
#define LED2			(1 << 1)
#define LED3			(1 << 2)
#define LED4			(1 << 3)

#define LED_OFF			1
#define LED_ON			0
#define LED_TOGGLE		2

typedef struct
{
	void (*Init)(void);
	void (*Set)(int LEDNum, int state);
} LED_CLASS;

/****Variables************************************************/

extern LED_CLASS led;

/****Function list********************************************/

/*************************************************************/

#endif /* __LED_H*/
