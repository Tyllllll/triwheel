#include "led.h"

/****Functions************************************************/

void LED_Init(void)
{
	gpio_init(LED1_PTX_n, GPO, 1);
	gpio_init(LED2_PTX_n, GPO, 1);
	gpio_init(LED3_PTX_n, GPO, 1);
}

void LED_Set(int LEDNum, int state)
{
	if (LEDNum & LED1)
	{
		switch (state)
		{
		case LED_ON:
		case LED_OFF:
			gpio_set(LED1_PTX_n, state);
			break;
		case LED_TOGGLE:
			LED1_Toggle();
			break;
        default:
            break;
		}
	}

	if (LEDNum & LED2)
	{
		switch (state)
		{
		case LED_ON:
		case LED_OFF:
			gpio_set(LED2_PTX_n, state);
			break;
		case LED_TOGGLE:
			LED2_Toggle();
			break;
        default:
            break;
		}
	}

	if (LEDNum & LED3)
	{
		switch (state)
		{
		case LED_ON:
		case LED_OFF:
			gpio_set(LED3_PTX_n, state);
			break;
		case LED_TOGGLE:
			LED3_Toggle();
			break;
        default:
            break;
		}
	}

	if (LEDNum & LED4)
	{
		switch (state)
		{
		case LED_ON:
		case LED_OFF:
			gpio_set(LED4_PTX_n, state);
			break;
		case LED_TOGGLE:
			LED4_Toggle();
			break;
        default:
            break;
		}
	}
}

/****Objects**************************************************/

LED_CLASS led = {&LED_Init, &LED_Set};
