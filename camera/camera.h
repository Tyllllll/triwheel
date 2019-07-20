/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_MT9V032.h
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
						���ж�				δʹ�ã���˲���
						�����ж�            C18           
						���ݿ�              C8-C15 
					------------------------------------ 
	
					Ĭ�Ϸֱ�����            188*120
					Ĭ��FPS                 50֡
 ********************************************************************************************************************/

#include "includes.h"

#ifndef _CAMERA_h
#define _CAMERA_h



#define COLUMN     80//ͼ����   ��Χ1-752     K60�ɼ���������188
#define ROW     60//ͼ��߶�	��Χ1-480




//����ͷ����ö��
typedef enum
{
    INIT = 0,               //����ͷ��ʼ������
    AUTO_EXP,               //�Զ��ع�����
    EXP_TIME,               //�ع�ʱ������
    FPS,                    //����ͷ֡������
    SET_COL,                //ͼ��������
    SET_ROW,                //ͼ��������
    LR_OFFSET,              //ͼ������ƫ������
    UD_OFFSET,              //ͼ������ƫ������
    GAIN,                   //ͼ��ƫ������
    CONFIG_FINISH,          //������λ����Ҫ����ռλ����
    
    SET_EXP_TIME = 0XF0,    //���������ع�ʱ������
    GET_STATUS,             //��ȡ����ͷ��������
    GET_VERSION,            //�̼��汾������
    
    SET_ADDR = 0XFE,        //�Ĵ�����ַ����
    SET_DATA                //�Ĵ�����������
}CMD;

extern uint8 mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ
extern uint8 Image1[2*ROW][2*COLUMN];
extern uint8 Image2[2*ROW][2*COLUMN];
extern uint8 Image[ROW][COLUMN];
extern uint8 SobelImage0[ROW][COLUMN];
extern uint32 Timer;
extern uint16 CameraFPS;

extern uint8 Image_Tran;
extern uint8 Image_Lock;
extern uint8 CameraIsReady;
void   mt9v032_cof_uart_interrupt(void);
void   VSYNC(void);
void   Camera_Init(void);
void   seekfree_sendimg_032(void);
void   row_finished(void);
void   Camera_Extract(void);
void Camera_ISR(void);
void MT9V032_extraction(uint8 imgbuff_extration[ROW][COLUMN],uint8 imgbuff_two[ROW][COLUMN]);


#endif

