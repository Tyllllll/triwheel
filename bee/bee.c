#include "bee.h"

/****Definitions**********************************************/

/****Variables************************************************/

/****Function list********************************************/

void bee_Init(void);
void bee_Toggle(void);
void bee_Set(int num);

/****Objects**************************************************/

BEE_CLASS bee =
{
	&bee_Init,
	&bee_Toggle,
	&bee_Set
};

/****Functions************************************************/

void bee_Init(void)
{
	gpio_init(BEE_PTX_n, GPO, 0);
}

void bee_Toggle(void)
{
	gpio_turn(BEE_PTX_n);
}

void bee_Set(int num)
{
	gpio_set(BEE_PTX_n, num);
}
