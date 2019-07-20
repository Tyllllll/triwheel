#ifndef __UI_H
#define __UI_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/

#define NUMBERPOS	48
#define FLOATEND	NUMBERPOS + 42
#define UINTEND		NUMBERPOS + 24
#define FLOATBLANK	"     "
#define UINTBLANK	"        "

#define FLOAT		0x01
#define UINT		0x02
#define NONE		0x00

#define ADD			1
#define SUB			-1

#define FLASH_TEST_SECTOR   (250)
#define FLASH_TEST_ADDR     (FLASH_TEST_SECTOR*2048)
#define DATA_SIZE           (64)

typedef struct
{
	void (*Disp)(void);
	void (*DispImg)(void);
	void (*DispAD)(void);
	void (*DispGyro)(void);
	void (*DispOther)(void);
	void (*DispParameter)(void);

	int8 cursor[3], level, flag_cursor, curMode, flag_adjust;
	int16 fps_timer_counter[3];
    int8 startbutton_flag, areYouReady, startLine, hasADMax;
} UI_CLASS;

typedef struct
{
	int8 page_num;
	int8 string[8][8][30];
	float32 data[8][8];
	int8 course;
	int8 Bit;
	int8 BitFlag;
}Setting_Class;

/****Variables************************************************/

extern UI_CLASS ui;

/****Functions************************************************/

extern void Parameter_Setting_Init(void);

#endif /* __UI_H */
