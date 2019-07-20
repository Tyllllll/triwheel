#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/****Definitions**********************************************/

#define KEY_DELAY		360
#define KEY_BEETIME		3

//// ËÄÂÖ°å
//#define KEY_UP_PTX_n			A0
//#define KEY_DOWN_PTX_n			A6
//#define KEY_LEFT_PTX_n			E28
//#define KEY_RIGHT_PTX_n			A5
//#define KEY_OK_PTX_n			E26
//#define KEY_CANCEL_PTX_n		E27
//#define KEY_CANCEL_PTX_n		E27
//
//#define SWITCH1_PTX_n			A13
//#define SWITCH2_PTX_n			A14
//#define SWITCH3_PTX_n			A15
//#define SWITCH4_PTX_n			A16

// Ö±Á¢°å
//#define KEY_UP_PTX_n			B23
//#define KEY_DOWN_PTX_n			B20
//#define KEY_LEFT_PTX_n			B21
//#define KEY_RIGHT_PTX_n			B22
//#define KEY_OK_PTX_n			C0
//#define KEY_CANCEL_PTX_n		C1
#define KEY_UP_PTX_n			B23
#define KEY_DOWN_PTX_n			B20
#define KEY_LEFT_PTX_n			C1
#define KEY_RIGHT_PTX_n			B22
#define KEY_OK_PTX_n			C0
#define KEY_CANCEL_PTX_n		B21

#define OK_IN			gpio_get(KEY_OK_PTX_n)
#define CANCEL_IN		gpio_get(KEY_CANCEL_PTX_n)
#define UP_IN			gpio_get(KEY_UP_PTX_n)
#define DOWN_IN			gpio_get(KEY_DOWN_PTX_n)
#define LEFT_IN			gpio_get(KEY_LEFT_PTX_n)
#define RIGHT_IN		gpio_get(KEY_RIGHT_PTX_n)

//#define SWITCH1			gpio_get(SWITCH1_PTX_n)
//#define SWITCH2			gpio_get(SWITCH2_PTX_n)
//#define SWITCH3			gpio_get(SWITCH3_PTX_n)
//#define SWITCH4			gpio_get(SWITCH4_PTX_n)

/********************
 *LEFT		OK		*
 *DOWN		UP		*
 *RIGHT		CANCEL	*
 ********************/

/********************
 *	11		12		*
 *	13		14		*
 *	15		16		*
 ********************/

typedef struct
{
	void (*Init)(void);

	int flag;
} KEYS_CLASS;

/****Includes*************************************************/

#include "includes.h"

/****Variables************************************************/

extern KEYS_CLASS keys;

/****Functions************************************************/

void Key_Up_ISR(void);
void Key_Down_ISR(void);
void Key_Left_ISR(void);
void Key_Right_ISR(void);
void Key_Ok_ISR(void);
void Key_Cancel_ISR(void);
void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);

/*************************************************************/

#endif /* __KEYBOARD_H */
