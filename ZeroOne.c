#include "includes.h"

/****Definitions**********************************************/

#define TASK_ENABLE 0

/****Function list********************************************/

/****Variables************************************************/
uint8 image_switch;
uint8 switch_mode,debugTime;


/****Objects**************************************************/
/*

三轮(√)


*/
/****main*****************************************************/

void main (void)
{
	get_clk();
	
	DisableInterrupts;
	UART_Init();	
	spi.SPI_Init();
	oled.Init();
	keys.Init();
	bee.Init();
	led.Init();
	control.PIDInit();
	control.SpeedInit();
	motor.MotorInit(MotorPwm);
	motor.QdInit();
	sensor.SensorInit();
	FLASH_Init();
	Parameter_Setting_Init();
	if (!ui.hasADMax)
	{
		DisableInterrupts;
		Magnet_Init();
	}

	ui.startbutton_flag = 0;
	
	task.Init();
         
    Findline.Process = Normal;
	 switch_mode = 1;
        if(switch_mode==DeBugMode)
        {
          debugTime = 6;
        }
	EnableInterrupts;
	
	enable_irq(PORTC_IRQn);
	enable_irq(PORTB_IRQn);
	enable_irq(PIT0_IRQn);
	enable_irq(PIT1_IRQn);
	enable_irq(PIT2_IRQn);
	enable_irq(PIT3_IRQn);
	
	while(1)
	{
		if (task.taskNum[0] == TASK_ENABLE) // 杂项周期
		{
			task.taskNum[0] = 500; // 周期毫秒数
			led.Set(LED1, LED_TOGGLE);
			 
			ui.flag_cursor = !ui.flag_cursor;
		}
		
		/*--------------------------------------------------------------------*/
		
		if (task.taskNum[1] == TASK_ENABLE)
		{
			task.taskNum[1] = 40; // 40毫秒周期，用作屏幕显示，25FPS
			ui.Disp();
		}
		
		/*--------------------------------------------------------------------*/
		
		if (task.taskNum[2] == TASK_ENABLE)//9ms
		{
			task.taskNum[2] = 20;
		}
		
		/*--------------------------------------------------------------------*/
		
		if (task.taskNum[3] == TASK_ENABLE)
		{
			task.taskNum[3] = 5;
			
		}
	}
}

/****Functions************************************************/

