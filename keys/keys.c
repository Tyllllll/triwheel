#include "keys.h"

/****Functions************************************************/

void Keys_Init(void);
void KEY_isr(void);

/****Objects**************************************************/

KEYS_CLASS keys = {&Keys_Init, 0};

/****Functions************************************************/

void Key_Up_ISR(void)
{
	if (ui.level == 0)
		ui.cursor[ui.level]--;
	if(ui.cursor[ui.level] < 0)
		ui.cursor[ui.level] = 5;
}

void Key_Down_ISR(void)
{
	if (ui.level == 0 || ui.level == 1 && ui.cursor[0] == 5)
	{
		ui.cursor[ui.level]++;
		if (ui.level == 0)
		{
			if(ui.cursor[ui.level] > 5)
				ui.cursor[ui.level] = 0;
		}
		else
		{
			if(ui.cursor[ui.level] > 7)
				ui.cursor[ui.level] = 0;
		}
	}
}

void Key_Left_ISR(void)
{
	if (ui.level == 0 || ui.cursor[0] == 0)
		ui.cursor[ui.level]--;
	if(ui.cursor[ui.level] < 0)
		ui.cursor[ui.level] = 5;
}

void Key_Right_ISR(void)
{
	if (ui.level == 0 || ui.cursor[0] == 0)
		ui.cursor[ui.level]++;
	if(ui.cursor[ui.level] > 5)
		ui.cursor[ui.level] = 0;
}

void Key_Ok_ISR(void)
{
	if (ui.level < 3)
	{
		if (ui.level == 0 && ui.cursor[0] == 4)
			ui.level += 2;
		else
			ui.level++;
	}
	ui.cursor[ui.level] = 0;
}

void Key_Cancel_ISR(void)
{
	if (ui.level > 0)
		ui.level--;
	if (ui.level == 0)
	{
		Findline.Process = Stop;
	}
}

void Keys_Init(void)
{
	port_init(KEY_UP_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
	port_init(KEY_DOWN_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
	port_init(KEY_LEFT_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
	port_init(KEY_RIGHT_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
	port_init(KEY_OK_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
	port_init(KEY_CANCEL_PTX_n, IRQ_FALLING | PF | ALT1 | PULLUP );
        
	port_init(C2,  PF | ALT1 | PULLUP );
}
