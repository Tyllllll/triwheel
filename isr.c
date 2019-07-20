#include "isr.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��A�������жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void PORTB_IRQHandler(void)
{
    //����жϱ�־��һ�ַ���ֱ�Ӳ����Ĵ�����ÿһλ��Ӧһ������
    PORTB->ISFR = 0xffffffff;
    //ʹ�����Ǳ�д�ĺ궨����������жϵ�����
    //PORTA_FLAG_CLR(A1);
	int16 cnt = 0, i;
	
	if (gpio_get(GanHuangGuanPT_x) == 1)
	{
//		Findline.Process = Stop;
		bee.time = 1000;
	}
	
	if (UP_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (UP_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTB_FLAG_CLR(KEY_UP_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Up_ISR();
//		while (1)
//		{
//			if (UP_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (UP_IN == 1)
//					break;
//			}
//		}
	}
	
	if (DOWN_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (DOWN_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTB_FLAG_CLR(KEY_DOWN_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Down_ISR();
//		while (1)
//		{
//			if (DOWN_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (DOWN_IN == 1)
//					break;
//			}
//		}
	}
	
	if (RIGHT_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (RIGHT_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTB_FLAG_CLR(KEY_RIGHT_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Right_ISR();
//		while (1)
//		{
//			if (RIGHT_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (RIGHT_IN == 1)
//					break;
//			}
//		}
	}
    	if (CANCEL_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (CANCEL_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTC_FLAG_CLR(KEY_CANCEL_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Cancel_ISR();
//		while (1)
//		{
//			if (CANCEL_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (CANCEL_IN == 1)
//					break;
//			}
//		}
	}

    
}

void PORTC_IRQHandler(void)
{	
  int16 cnt = 0, i;
	if (PORTC->ISFR & 0xC0000)
	{
		PORTC->ISFR = 0xffffffff;
		Camera_ISR();
		return;
	}
	
	PORTC->ISFR = 0xffffffff;	
        
        if (OK_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (OK_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTC_FLAG_CLR(KEY_OK_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Ok_ISR();
//		while (1)
//		{
//			if (OK_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (OK_IN == 1)
//					break;
//			}
//		}
	}
      if (LEFT_IN == 0)
	{
		cnt = 0;
		for (i = 0; i < 2000; i++)
			if (LEFT_IN == 0)
				cnt++;
		if (cnt < 1900)
		{
			PORTB_FLAG_CLR(KEY_LEFT_PTX_n);
			return;
		}
		bee.time = KEY_BEETIME;
		Key_Left_ISR();
//		while (1)
//		{
//			if (LEFT_IN == 1)
//			{
//				systick_delay_ms(20);
//				if (LEFT_IN == 1)
//					break;
//			}
//		}
	}
	

}


void DMA0_IRQHandler(void)
{
    DMA_IRQ_CLEAN(DMA_CH0);
}

void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0);
	pit0_isr();
}

void PIT1_IRQHandler(void)
{
    PIT_FlAG_CLR(pit1);
}

void PIT2_IRQHandler(void)
{
    PIT_FlAG_CLR(pit2);
	pit2_isr();
}

void PIT3_IRQHandler(void)
{
    PIT_FlAG_CLR(pit3);
	pit3_isr();
}

void UART0_RX_TX_IRQHandler(void)
{
    if(UART0->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
		Lora_isr();
    }
    if(UART0->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
        
    }
}

void UART1_RX_TX_IRQHandler(void)
{
    if(UART1->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
    }
    if(UART1->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
        
    }
}

void UART2_RX_TX_IRQHandler(void)
{
    if(UART2->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
    }
    if(UART2->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��UART3�����жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if(UART3->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        mt9v032_cof_uart_interrupt();
    }
    if(UART3->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
        
    }
}

void UART4_RX_TX_IRQHandler(void)
{
    if(UART4->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        
    }
    if(UART4->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������
        
    }
}


/*
�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ������DMA0�жϣ�Ȼ��͵�����ȥ���ĸ���DMA0���жϺ������ƣ��ҵ���дһ�������Ƶĺ�������
void DMA0_IRQHandler(void)
{
;
}
�ǵý����жϺ������־λ


DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/



