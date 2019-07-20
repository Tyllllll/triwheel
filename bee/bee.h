#ifndef __BEE_H
#define __BEE_H

/****Includes*************************************************/

#include "common.h"

/****Definitions**********************************************/

#define BEE_PTX_n		D15

#define BEE_HIGH		1
#define BEE_LOW			0

typedef struct
{
	void (*Init)(void);
	void (*Toggle)(void);
	void (*Set)(int num);

	int time;
} BEE_CLASS;

/****Variables************************************************/

extern BEE_CLASS bee;

/****Functions************************************************/

/*************************************************************/

#endif /* __BEE_H */
