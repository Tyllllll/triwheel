#include "task.h"
#include "includes.h"

/****Variables************************************************/

int delayMs;
int turnerTime = 0;

/****Function list********************************************/

void TASK_PIT_Init(void);
void TASK_PIT3_Start(void);
void TASK_PIT3_Stop(void);
void TASK_Delay(int xms);

/****Objects**************************************************/

TASK_CLASS task = {&TASK_PIT_Init, &TASK_Delay, &TASK_PIT3_Start, &TASK_PIT3_Stop, {0}, 0};

/****Functions************************************************/

void TASK_PIT_Init(void)
{
	// 舵机打脚中断函数抢占优先级最高
	
	set_irq_priority(PIT0_IRQn, NVIC_EncodePriority(2, 0, 1));
	set_irq_priority(PIT2_IRQn, NVIC_EncodePriority(2, 3, 3));
	set_irq_priority(PIT3_IRQn, NVIC_EncodePriority(2, 1, 3));
	set_irq_priority(UART1_RX_TX_IRQn, NVIC_EncodePriority(2, 0, 3));
	set_irq_priority(UART2_RX_TX_IRQn, NVIC_EncodePriority(2, 0, 3));
	set_irq_priority(PORTB_IRQn, NVIC_EncodePriority(2, 2, 1));
	set_irq_priority(PORTC_IRQn, NVIC_EncodePriority(2, 0, 1));
	set_irq_priority(DMA0_IRQn, NVIC_EncodePriority(2, 0, 2));
	
	pit_init_us(pit0, 500);// 中断函数pit0_isr  
	
	pit_init_ms(pit2, 10);// 中断函数pit2_isr
	
	pit_init_ms(pit3, 10);// 中断函数pit3_isr
}

void TASK_Delay(int xms)
{
	delayMs = xms;
	while (delayMs);
}

void TASK_PIT3_Start(void)
{
	pit_time_start(pit3);
}

void TASK_PIT3_Stop(void)
{
	int time;
	
	time = pit_time_get(pit3);
	pit_close(pit3);
	time *= (1.0 / bus_clk_mhz);
	
	task.testTime = time;
}

void pit0_isr(void)
{
	static unsigned char i,Counter;
	
	
	Counter++;
	control.MotorControl(Findline.err);
	
	
	if(Counter>=2)
	{
		
		Counter=0;
		spi.Get_Attitude();   
		
		ui.fps_timer_counter[1]++;
		
		if(ui.fps_timer_counter[1]%5 == 0)
		{
			motor.GetSpeed();    
			sensor.GetSensor();
			
		}
		if(ui.fps_timer_counter[1]>=999)
		{
			ui.fps_timer_counter[0]=ui.fps_timer_counter[2],
			ui.fps_timer_counter[1]=ui.fps_timer_counter[2]=0;
			
			if(switch_mode==0)
			{
				debugTime = debugTime>0 ? --debugTime : 0;
				if(debugTime <=0) Findline.loseflag = 1;
			}
		}
		
		
		for (i = 0; i < NumOfTask; i++)
			if (task.taskNum[i])
				task.taskNum[i]--;
		if (delayMs > 0)
			delayMs--;
		
		if (keys.flag > 0)
			keys.flag--;
		
		if (bee.time > 0)
		{
			bee.time--;
			bee.Set(BEE_HIGH);
		}
		else
			bee.Set(BEE_LOW);
		
	}
	
	
	
}

void pit2_isr(void)
{
	picprocss.findline();
	switch(Findline.Process)
	{
	case Crossmeeting:
		picprocss.crossmeeting();
		break;                                            
	case Transome:
		picprocss.transoming();
		break;
	case Stop:
		picprocss.finalstoping();
		break;
	default:
		picprocss.caculate_err();
		break;                          
	}                
	
	laser.VL53L0X_SimpleMea();  
	UART_2_computer();
	ui.fps_timer_counter[2]++;                 
}

void pit3_isr(void)
{
	
}


