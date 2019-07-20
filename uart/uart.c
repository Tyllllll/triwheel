#include "uart.h"

/****Definitions**********************************************/

/****Functions************************************************/

void UART_isr(void);

/****Variables************************************************/

double UartData[9] = {0};
double PairData[9] = {0};
uint8 sendData[34] = {0};


//山外
int8 buf12[36] = { 0x03, 0xfc, 
0, 0, 0, 0, 0, 0 , 0,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 0, 0 , 
0,0,0,0,0,0,0,0,0,0,
0xfc, 0x03 };
//山外
//int8 buf12[12] = { 0x03, 0xfc, 
//0, 0, 0, 0, 0, 0 , 0 , 0,
//0xfc, 0x03 };
int buf12_num=0;

SerialPortType SerialPortRx;

/****Functions************************************************/

void UART_Init(void)
{
//	// Lora
	uart_init(uart0, 9600);
//	// 蓝牙
	uart_init(uart1, 115200);
	uart_rx_irq_en(uart0);
//	uart_rx_irq_en(uart1);
}

void UART_Putchar(char ch)
{
	uart_putchar(uart1, ch);
}

void Lora_Putchar(char ch)
{
	uart_putchar(uart0, ch);
}

void Lora_SendMessage(char *str, char length)
{
  for (char i = 0; i <length; i++)
  {
    Lora_Putchar(*(str + i));
  }
}

void UART_Printf(char *str)
{
	int i = 0;
	
	while (str[i] != '\0')
	{
		UART_Putchar(str[i]);
		i++;
	}
}

void UART_Set(int chx, int data)
{
	sendData[chx * 2] = HIGHBYTE(data);
	sendData[chx * 2 + 1] = LOWBYTE(data);
}

void UartDebug(void)
{
	
}

void UartCmd(uint8 CmdNum,uint8 Data)
{
	if (CmdNum == 2 && Data == 102)//page up
	{
		;
	}
	if (CmdNum == 2 && Data == 103)//page down
	{
		;
	}
}

void Lora_isr(void)
{
	int d1;
	
	uart_getchar(uart0, &SerialPortRx.Data);
	
	SerialPortRx.Buffer[SerialPortRx.Stack] = SerialPortRx.Data;
	if(SerialPortRx.Buffer[0]==0x52)
		SerialPortRx.Stack++;
	if (SerialPortRx.Stack >= UartRxBufferLen)
	{
		if(
		   SerialPortRx.Buffer[0]==0x52//R
			   && SerialPortRx.Buffer[1]==0x3A//:
				   )
		{
			
			d1 = //((int)SerialPortRx.Buffer[4])<<24|((int)SerialPortRx.Buffer[5])<<16|
				((int)SerialPortRx.Buffer[2])<<8|((int)SerialPortRx.Buffer[3])<<0;
			if(d1 == 0x444C) //DL
			{
				crossmeet.FinishflagAnother = 1;
				SerialPortRx.count = 200;
			}
			if(d1 == 0x4758) //GX
			{
				ui.areYouReady = 1;
				SerialPortRx.count = 200;
			}
			if (d1 == 0x464B) //FK
			{
				crossmeet.FinishflagAnother = 1;
                                crossmeet.RunTime = 0;
				ui.areYouReady = 1;
				SerialPortRx.count = 200;
			}
		}
		SerialPortRx.Stack = 0;
	}
	if (SerialPortRx.count == 5)
	{
		SerialPortRx.Stack = 0;
	}
}




//山外
void uint2Byte(float *target, int8 *buf, int8 beg4) //类型转换函数 将uint16转成字节发送 
{
	int8 *point;
	point = (int8*)target; 
	buf[beg4] = point[0];
	buf[beg4 +1] = point[1];
	buf[beg4 +2] = point[2];
	buf[beg4 +3] = point[3];
}

void UART_2_computer (void)
{    
	static float float_out=0; 
        
	// 1
	float_out = Findline.Process;
	uint2Byte(&float_out,buf12,2);
	// 2
    float_out = speedloop.Reality*100;
	uint2Byte(&float_out,buf12,6);
	// 3
	float_out = sensor.once_uni_ad[EEEM];
	uint2Byte(&float_out,buf12,10);
	// 4
	float_out=Findline.errBuff;
	uint2Byte(&float_out,buf12,14);
	// 5
	float_out=speed.Reality*100;
	uint2Byte(&float_out,buf12,18);
	// 6
	float_out=gyro.TurnAngle_Integral;
	uint2Byte(&float_out,buf12,22);
	// 7
	float_out= speedloop.Intigral*0.1;
	uint2Byte(&float_out,buf12,26);
	// 8
	float_out=sensor.once_uni_ad[EEEM];
	uint2Byte(&float_out,buf12,30);
	
	
	for(buf12_num=0;buf12_num<36;buf12_num++)
	{
		UART_Putchar(buf12[buf12_num]);
	}
}