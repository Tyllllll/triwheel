#ifndef __OLED_H
#define __OLED_H

/****Includes*************************************************/

#include "common.h"
#include "includes.h"

/****GPIOs****************************************************/

//#define OLED_GPIO_PTx 	(PTA | PTE)
#define OLED_GPIO_SCLK 	D12
#define OLED_GPIO_SDA 	D13
#define OLED_GPIO_RST 	D14
#define OLED_GPIO_DC	D10
#define OLED_GPIO_CS	D11

#define OLED_SCLK(x) gpio_set(OLED_GPIO_SCLK, x)
#define OLED_SDA(x)  gpio_set(OLED_GPIO_SDA, x)
#define OLED_RST(x)  gpio_set(OLED_GPIO_RST, x)
#define OLED_DC(x)   gpio_set(OLED_GPIO_DC, x)

/****Definitions**********************************************/

typedef struct
{
	void (*Init)();
	void (*Putchar)(unsigned char x, unsigned char y, unsigned char ch);
	void (*Printf)(unsigned char x, unsigned char y, unsigned char ch[]);
	void (*PrintCharValue)(unsigned char x, unsigned char y, unsigned char data);
	void (*PrintIntValue)(unsigned char x, unsigned char y, int data);
	void (*PrintUintValue)(unsigned char x, unsigned char y, unsigned int data);
	void (*PrintFloatValue)(unsigned char x, unsigned char y, float data);
	void (*PrintReverseFloatValue)(unsigned char x, unsigned char y, float data, int8 Bit);
	void (*Showimage)();
	void (*SetNumDisplay)(unsigned char x, unsigned char y, unsigned int num);
} OLED_CLASS;

/****Variables************************************************/

extern const unsigned char F6x8[][6];
extern OLED_CLASS oled;

/****Functions************************************************/

void oled_clear();

/*************************************************************/

#endif /* __OLED_H */
