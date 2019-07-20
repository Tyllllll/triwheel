/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2016,��ɿƼ�
* All rights reserved.
* ��������QQȺ��179029047
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file       		SEEKFREE_MT9V032.c
* @brief      		�����(�Ҷ�����ͷ)������
* @company	   		�ɶ���ɿƼ����޹�˾
* @author     		Go For It(1325536866)
* @version    		v1.0
* @Software 		IAR 7.2 or MDK 5.17
* @Target core		MK60DN512VLL10
* @Taobao   		https://seekfree.taobao.com/
* @date       		2016-02-25
* @note	
MT9V032���߶��壺
------------------------------------ 
ģ��ܽ�            ��Ƭ���ܽ�
SDA(51��RX)         C17
SCL(51��TX)         C16
���ж�              C6
�����ж�            C18           
���ݿ�              C8-C15 
------------------------------------ 

Ĭ�Ϸֱ�����            188*120
Ĭ��FPS                 50֡
********************************************************************************************************************/
#include "camera.h"
#include "string.h"
#define MT9V032_COF_UART    uart3

uint8 CameraIsReady;
uint8 Image1[2*ROW][2*COLUMN];
uint8 Image2[2*ROW][2*COLUMN];
uint8 Image[ROW][COLUMN];
uint8 SobelImage0[ROW][COLUMN];
uint8 receive[3];
uint8 receive_num = 0;
uint8 uart_receive_flag = 1;
uint8 Image_Tran;
uint8 Image_Lock;
uint32 Timer;
uint16 CameraFPS;
//��Ҫ���õ�����ͷ������
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
    //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          300}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               110}, //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           2*COLUMN}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           2*ROW}, //ͼ��������        ��Χ1-480
    {LR_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {UD_OFFSET,         0},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
    {GAIN,              32},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
    
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      
    {EXP_TIME,          0},   //�ع�ʱ��          
    {FPS,               0},   //ͼ��֡��          
    {SET_COL,           0},   //ͼ��������        
    {SET_ROW,           0},   //ͼ��������        
    {LR_OFFSET,         0},   //ͼ������ƫ����    
    {UD_OFFSET,         0},   //ͼ������ƫ����    
    {GAIN,              0},   //ͼ������          
};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����3�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_cof_uart_interrupt(void)
{
    uart_getchar(MT9V032_COF_UART,&receive[receive_num]);;
    receive_num++;
    
    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�������Ϣ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
    uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = GET_CFG[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        
        //�ȴ����ܻش�����
        while(!uart_receive_flag);
        uart_receive_flag = 0;
        
        GET_CFG[i][1] = receive[1]<<8 | receive[2];
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�̼��汾
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(void)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    return ((uint16)(receive[1]<<8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ͷ�ع�ʱ��
//  @param      light   �����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
//  @return     uint16  ��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(uint16 light)
{
    uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ͷ�ڲ��Ĵ�������д����
//  @param      addr    ����ͷ�ڲ��Ĵ�����ַ
//  @param      data    ��Ҫд�������
//  @return     uint16  �Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v032_reg(uint8 addr, uint16 data)
{
    uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    systick_delay_ms(10);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;
    
}

uint16 version;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ��ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
void Camera_Init(void)
{
    //���ò���    ������ο�ʹ���ֲ�
    uint16 temp, i;
    uint8  send_buffer[4];
    
    uart_init (MT9V032_COF_UART, 9600);	//��ʼ������ ��������ͷ    
    uart_rx_irq_en(MT9V032_COF_UART);
    
    //�ȴ�����ͷ�ϵ��ʼ���ɹ�
    systick_delay_ms(1000);
    uart_receive_flag = 0;
    
    //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_CFG[i][0];
        temp = MT9V032_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        systick_delay_ms(2);
    }
    //�ȴ�����ͷ��ʼ���ɹ�
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    while((0xff != receive[1]) || (0xff != receive[2]));
    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
    
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    get_config();
    
    //����ͷ�ɼ���ʼ��
    DisableInterrupts;
    //DMAͨ��0��ʼ����PTC18����Դ(Ĭ��������)��Դ��ַΪC_IN_DATA(1)(PTC8-PTC15)��Ŀ�ĵ�ַΪ��image��ÿ�δ���1Byte ������ϱ���Ŀ�ĵ�ַ
    dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(1), (void *)Image1, C18, DMA_BYTE1, 2*COLUMN*2*ROW, DADDR_KEEPON);
    port_init(C18, ALT1 | DMA_RISING | PULLDOWN);  			    //PCLK  ����Դ����
    DMA_DIS(DMA_CH0);                                     		//����DMAͨ��
    DMA_IRQ_CLEAN(DMA_CH0);                               		//���ͨ�������жϱ�־λ
    DMA_IRQ_EN(DMA_CH0);                                  		//����DMAͨ���ж�
    DMA_EN(DMA_CH0);  											//ʹ��DMA
    
    disable_irq(PORTC_IRQn);                             		//�ر�PTC���ж�
    port_init(C19, ALT1 | IRQ_FALLING | PULLDOWN);        		//���жϣ��½��ش����жϡ�����
    set_irq_priority(PORTC_IRQn,1);                             //�ж����ȼ�
    
    Image_Tran = 1;
    Image_Lock = 0;
}

void Camera_Extract(void)
{
    uint8 i,j;
    if(Image_Tran == 1 && Image_Lock != 2)
    {
        for(i=0;i<ROW*2;i+=2)
        {
//            memcpy(*(Image + i/2),*(Image2 + i), sizeof(*(Image2 + i)));
          for(j=0;j<COLUMN*2;j+=2)
          {
            Image[i/2][j/2] = Image2[i][j];
          }
        }
        Image_Lock = 2;
    }
    else if(Image_Tran == 2 && Image_Lock != 1)
    {
        for(i=0;i<ROW*2;i+=2)
        {           
//            memcpy(*(Image + i/2),*(Image1 + i), sizeof(*(Image1 + i)));
          for(j=0;j<COLUMN*2;j+=2)
          {
           Image[i/2][j/2] = Image1[i][j]; 
          }
        }
        Image_Lock = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ���ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void Camera_ISR(void)
{
    DMA_DIS(DMA_CH0);
    
    if(Image_Tran == 1)
        Image_Tran = 2;  //�л���ͼ��ͼ1ͼ2���δ���
    else if(Image_Tran == 2)
        Image_Tran = 1;
    
    if(Image_Tran == 1)//��Ҫ��ͼ1��ͼ1û���ڴ������߽�Ҫ��ͼ2��ͼ2û���ڴ���
        dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)Image2[0],COLUMN*ROW*4);
    else if(Image_Tran == 2)
        dma_repeat(DMA_CH0,(void *)&C_IN_DATA(1),(void *)Image1[0],COLUMN*ROW*4);
    
    DMA_EN(DMA_CH0);
}

uint8   mt9v032_finish_flag = 0;      //һ��ͼ��ɼ���ɱ�־λ
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷDMA����ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void row_finished(void)
{
    CameraIsReady = 1;
    //�����̶Ժڱ߲������������ʹ�����ݵ�ʱ��ʹ��image��������������ұ߼���
    //����ֱ��ʹ��󣬾�û�취ת���ˣ���ΪK60��RAM�᲻��
    //һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.33MS����(50FPS��188*120�ֱ���)
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ͷͼ��������λ���鿴ͼ��
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(void)
{
    uart_putchar(uart0,0x00);uart_putchar(uart0,0xff);uart_putchar(uart0,0x01);uart_putchar(uart0,0x01);//��������
    uart_putbuff(uart0, (uint8_t *)Image, ROW*COLUMN);  //����ͼ��
}


uint8 imgbuff_three[ROW][COLUMN];
uint16 binaryThreshold = 210;
//void MT9V032_extraction(uint8 imgbuff_extration[ROW][COLUMN],uint8 imgbuff_two[ROW][COLUMN])
//{
//    int i=0, j=0;
//    int image_x=0, image_y=0, image=0;
//    for(i=1;i<ROW-1;i++)
//    {
//        for(j=1;j<COLUMN-1;j++)
//        {
//            imgbuff_three[i][j]=(*(*(imgbuff_two+i+1)+j)+*(*(imgbuff_two+i)+j-1)+*(*(imgbuff_two+i)+j)*4+*(*(imgbuff_two+i)+j+1)+*(*(imgbuff_two+i-1)+j))/8;
//        }
//    }
//    for(i=1;i<ROW-1;i++)
//    {
//        for(j=1;j<COLUMN-1;j++)
//        {
//            image_x=(-*(*(imgbuff_two+i-1)+j-1))+(-4*(*(*(imgbuff_two+i)+j-1)))+(-*(*(imgbuff_two+i+1)+j-1))+(*(*(imgbuff_two+i-1)+j+1))+(4*(*(*(imgbuff_two+i)+j+1)))+(*(*(imgbuff_two+i+1)+j+1));
//            if(image_x<0)
//                image_x=-image_x;
//            image_y=(-*(*(imgbuff_two+i+1)+j-1))+(-4*(*(*(imgbuff_two+i+1)+j)))+(-*(*(imgbuff_two+i+1)+j+1))+(*(*(imgbuff_two+i-1)+j-1))+(4*(*(*(imgbuff_two+i-1)+j)))+(*(*(imgbuff_two+i-1)+j+1));
//            if(image_y<0)
//                image_y=-image_y;
//            image=image_x+image_y;
//            if(image>255)
//                image=255;
//            *(*(imgbuff_extration+i)+j)=image>binaryThreshold?0:253;
//        }
//    }
//}

void MT9V032_extraction(uint8 imgbuff_extration[ROW][COLUMN],uint8 imgbuff_two[ROW][COLUMN])
{
    int i=0, j=0;
    int image_x=0, image_y=0, image=0;
    for(i=1;i<ROW-1;i++)
    {
        for(j=1;j<COLUMN-1;j++)
        {
            image_x=(-*(*(imgbuff_two+i-1)+j-1))+(-4*(*(*(imgbuff_two+i)+j-1)))+(-*(*(imgbuff_two+i+1)+j-1))+(*(*(imgbuff_two+i-1)+j+1))+(4*(*(*(imgbuff_two+i)+j+1)))+(*(*(imgbuff_two+i+1)+j+1));
            if(image_x<0)
                image_x=-image_x;
            image_y=(-*(*(imgbuff_two+i+1)+j-1))+(-4*(*(*(imgbuff_two+i+1)+j)))+(-*(*(imgbuff_two+i+1)+j+1))+(*(*(imgbuff_two+i-1)+j-1))+(4*(*(*(imgbuff_two+i-1)+j)))+(*(*(imgbuff_two+i-1)+j+1));
            if(image_y<0)
                image_y=-image_y;
            image=image_x+image_y;
            if(image>255)
                image=255;
            //��ֵ��
//            *(*(imgbuff_extration+i)+j)=image;
            *(*(imgbuff_extration+i)+j)=image>binaryThreshold?0:253;
        }
    }
}
