#ifndef __UART_H
#define __UART_H

/****Includes*************************************************/

#include "includes.h"

/****Definitions**********************************************/

#define WHICH_UART  	UART0
#define UART_RxPinx 	PTA1
#define UART_TxPinx	PTA2

#define BAUDRATE	115200

#define UartRxBufferLen	4
#define UartRxDataLen	2
#define UartRxCmdLen	7
#define UartRxPairLen	8
#define UartPairNum		SerialPortRx.Buffer[SerialPortRx.Stack - 4]
#define UartPairHigh	        SerialPortRx.Buffer[SerialPortRx.Stack - 3]
#define UartPiarLow		SerialPortRx.Buffer[SerialPortRx.Stack - 2]
#define UartCmdNum		SerialPortRx.Buffer[SerialPortRx.Stack - 3]
#define UartCmdData		SerialPortRx.Buffer[SerialPortRx.Stack - 2]

#define HIGHBYTE(x)	(uint8)(x >> 8)
#define LOWBYTE(x)	(uint8)(x & 0xFF)

typedef struct
{
	int Stack;
	uint8 Data;
	uint8 PreData;
	uint8 Buffer[UartRxBufferLen];
	uint8 Enable;
	uint8 Check;
	int16 count;
} SerialPortType;

/****Variables************************************************/

extern SerialPortType SerialPortRx;

/****Functions************************************************/

void UART_Init(void);
void Lora_Putchar(char ch);
void Lora_SendMessage(char *str, char length);
void UART_2_computer(void);
void Lora_isr(void);

/*************************************************************/


#endif /* __UART_H */
