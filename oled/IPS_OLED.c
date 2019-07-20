/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		SEEKFREE_IPS.c
 * @brief      		IPS 液晶驱动
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-12
 * @note		
					IPS液晶接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SCL                 A15
						SDA                 A16
						RES                 B17
						DC                  B16
						CS                  A14
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
	
					------------------------------------ 

 ********************************************************************************************************************/


//#include "font.h"
#include "IPS_OLED.h"
//#include "string.h"

OLED_CLASS oled = {
	&IPS_Init,
	&ips_showchar,
	&ips_showstr,
	&ips_showuint8,
	&ips_showint16,
	&ips_showuint16,
	&ips_showfloat,
	&ips_showReversefloat,
	&showimage
};
     
const uint8 tft_ascii[95][16] = 
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/

	{0x00,0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x08,0x08,0x00,0x00},/*"!",1*/

	{0x00,0x48,0x24,0x24,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/

	{0x00,0x00,0x00,0x48,0x48,0x48,0x7E,0x24,0x24,0x24,0x7E,0x24,0x24,0x24,0x00,0x00},/*"#",3*/

	{0x00,0x00,0x10,0x3C,0x52,0x52,0x12,0x1C,0x30,0x50,0x50,0x52,0x52,0x3C,0x10,0x10},/*"$",4*/

	{0x00,0x00,0x00,0x22,0x25,0x15,0x15,0x0D,0x2A,0x58,0x54,0x54,0x52,0x22,0x00,0x00},/*"%",5*/

	{0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0A,0x76,0x25,0x29,0x19,0x91,0x6E,0x00,0x00},/*"&",6*/

	{0x00,0x06,0x04,0x04,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/

	{0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00},/*"(",8*/

	{0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00},/*")",9*/

	{0x00,0x00,0x00,0x00,0x08,0x08,0x6B,0x1C,0x1C,0x6B,0x08,0x08,0x00,0x00,0x00,0x00},/*"*",10*/

	{0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x00,0x00,0x00,0x00},/*"+",11*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x04,0x04,0x02},/*",",12*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x00},/*".",14*/

	{0x00,0x00,0x40,0x20,0x20,0x20,0x10,0x10,0x08,0x08,0x08,0x04,0x04,0x02,0x02,0x00},/*"/",15*/

	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",16*/

	{0x00,0x00,0x00,0x10,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",17*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x40,0x20,0x10,0x08,0x04,0x42,0x7E,0x00,0x00},/*"2",18*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x40,0x20,0x18,0x20,0x40,0x42,0x42,0x3C,0x00,0x00},/*"3",19*/

	{0x00,0x00,0x00,0x20,0x30,0x30,0x28,0x24,0x24,0x22,0xFE,0x20,0x20,0xF8,0x00,0x00},/*"4",20*/

	{0x00,0x00,0x00,0x7E,0x02,0x02,0x02,0x1E,0x22,0x40,0x40,0x42,0x22,0x1C,0x00,0x00},/*"5",21*/

	{0x00,0x00,0x00,0x18,0x24,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x44,0x38,0x00,0x00},/*"6",22*/

	{0x00,0x00,0x00,0x7E,0x42,0x20,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x00,0x00},/*"7",23*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",24*/

	{0x00,0x00,0x00,0x1C,0x22,0x42,0x42,0x42,0x62,0x5C,0x40,0x40,0x24,0x18,0x00,0x00},/*"9",25*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00},/*":",26*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08},/*";",27*/

	{0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00},/*"<",28*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",29*/

	{0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*">",30*/

	{0x00,0x00,0x00,0x3C,0x42,0x42,0x46,0x20,0x10,0x10,0x10,0x00,0x18,0x18,0x00,0x00},/*"?",31*/

	{0x00,0x00,0x00,0x1C,0x22,0x5A,0x55,0x55,0x55,0x55,0x55,0x3A,0x42,0x3C,0x00,0x00},/*"@",32*/

	{0x00,0x00,0x00,0x08,0x08,0x18,0x14,0x14,0x24,0x3C,0x22,0x42,0x42,0xE7,0x00,0x00},/*"A",33*/

	{0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x1E,0x22,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"B",34*/

	{0x00,0x00,0x00,0x7C,0x42,0x42,0x01,0x01,0x01,0x01,0x01,0x42,0x22,0x1C,0x00,0x00},/*"C",35*/

	{0x00,0x00,0x00,0x1F,0x22,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1F,0x00,0x00},/*"D",36*/

	{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x42,0x42,0x3F,0x00,0x00},/*"E",37*/

	{0x00,0x00,0x00,0x3F,0x42,0x12,0x12,0x1E,0x12,0x12,0x02,0x02,0x02,0x07,0x00,0x00},/*"F",38*/

	{0x00,0x00,0x00,0x3C,0x22,0x22,0x01,0x01,0x01,0x71,0x21,0x22,0x22,0x1C,0x00,0x00},/*"G",39*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"H",40*/

	{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"I",41*/

	{0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11,0x0F},/*"J",42*/

	{0x00,0x00,0x00,0x77,0x22,0x12,0x0A,0x0E,0x0A,0x12,0x12,0x22,0x22,0x77,0x00,0x00},/*"K",43*/

	{0x00,0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x42,0x7F,0x00,0x00},/*"L",44*/

	{0x00,0x00,0x00,0x77,0x36,0x36,0x36,0x36,0x36,0x2A,0x2A,0x2A,0x2A,0x6B,0x00,0x00},/*"M",45*/

	{0x00,0x00,0x00,0xE3,0x46,0x46,0x4A,0x4A,0x52,0x52,0x52,0x62,0x62,0x47,0x00,0x00},/*"N",46*/

	{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1C,0x00,0x00},/*"O",47*/

	{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x42,0x3E,0x02,0x02,0x02,0x02,0x07,0x00,0x00},/*"P",48*/

	{0x00,0x00,0x00,0x1C,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x4D,0x32,0x1C,0x60,0x00},/*"Q",49*/

	{0x00,0x00,0x00,0x3F,0x42,0x42,0x42,0x3E,0x12,0x12,0x22,0x22,0x42,0xC7,0x00,0x00},/*"R",50*/

	{0x00,0x00,0x00,0x7C,0x42,0x42,0x02,0x04,0x18,0x20,0x40,0x42,0x42,0x3E,0x00,0x00},/*"S",51*/

	{0x00,0x00,0x00,0x7F,0x49,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"T",52*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"U",53*/

	{0x00,0x00,0x00,0xE7,0x42,0x42,0x22,0x24,0x24,0x14,0x14,0x18,0x08,0x08,0x00,0x00},/*"V",54*/

	{0x00,0x00,0x00,0x6B,0x2A,0x2A,0x2A,0x2A,0x2A,0x36,0x14,0x14,0x14,0x14,0x00,0x00},/*"W",55*/

	{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},/*"X",56*/

	{0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x08,0x08,0x08,0x1C,0x00,0x00},/*"Y",57*/

	{0x00,0x00,0x00,0x7E,0x21,0x20,0x10,0x10,0x08,0x04,0x04,0x42,0x42,0x3F,0x00,0x00},/*"Z",58*/

	{0x00,0x78,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x78,0x00},/*"[",59*/

	{0x00,0x00,0x02,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x20,0x20,0x20,0x40,0x40},/*"\",60*/

	{0x00,0x1E,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1E,0x00},/*"]",61*/

	{0x00,0x18,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",63*/

	{0x00,0x06,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x30,0x2C,0x22,0x32,0x6C,0x00,0x00},/*"a",65*/

	{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x1A,0x26,0x42,0x42,0x42,0x26,0x1A,0x00,0x00},/*"b",66*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x02,0x02,0x02,0x44,0x38,0x00,0x00},/*"c",67*/

	{0x00,0x00,0x00,0x00,0x60,0x40,0x40,0x7C,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00},/*"d",68*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x7E,0x02,0x42,0x3C,0x00,0x00},/*"e",69*/

	{0x00,0x00,0x00,0x00,0x30,0x48,0x08,0x3E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"f",70*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x22,0x22,0x1C,0x02,0x3C,0x42,0x42,0x3C},/*"g",71*/

	{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x3A,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"h",72*/

	{0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"i",73*/

	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x38,0x20,0x20,0x20,0x20,0x20,0x20,0x22,0x1E},/*"j",74*/

	{0x00,0x00,0x00,0x00,0x03,0x02,0x02,0x72,0x12,0x0A,0x0E,0x12,0x22,0x77,0x00,0x00},/*"k",75*/

	{0x00,0x00,0x00,0x08,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x3E,0x00,0x00},/*"l",76*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x92,0x92,0x92,0x92,0x92,0xB7,0x00,0x00},/*"m",77*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x46,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},/*"n",78*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},/*"o",79*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1B,0x26,0x42,0x42,0x42,0x26,0x1A,0x02,0x07},/*"p",80*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x40,0xE0},/*"q",81*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x4C,0x04,0x04,0x04,0x04,0x1F,0x00,0x00},/*"r",82*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x42,0x02,0x3C,0x40,0x42,0x3E,0x00,0x00},/*"s",83*/

	{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x3E,0x08,0x08,0x08,0x08,0x48,0x30,0x00,0x00},/*"t",84*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x42,0x42,0x42,0x42,0x62,0xDC,0x00,0x00},/*"u",85*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x22,0x22,0x14,0x14,0x08,0x08,0x00,0x00},/*"v",86*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDB,0x91,0x52,0x5A,0x2A,0x24,0x24,0x00,0x00},/*"w",87*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00},/*"x",88*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x08,0x08,0x06},/*"y",89*/

	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x22,0x10,0x08,0x08,0x44,0x7E,0x00,0x00},/*"z",90*/

	{0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x20,0x20,0x20,0x20,0x20,0xC0,0x00},/*"{",91*/

	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},/*"|",92*/

	{0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x03,0x00},/*"}",93*/

	{0x04,0x5A,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/
};

void IPS_Init(void)
{
	int bund = spi_init(spi2, SPI_PCS0, MASTER, 8*1000*1000);//硬件SPI初始化
	
    gpio_init(IPS_DC_PIN,GPO,0);
	bund = 0;
    gpio_init(IPS_REST_PIN,GPO,0);
    //gpio_init(IPS_BL_PIN,GPO,1);

    IPS_REST(0);
    systick_delay_ms(200);
    
    IPS_REST(1);
    systick_delay_ms(100);
	
    ips_writeIndex(0x36);
    
    if      (IPS_DISPLAY_DIR==0)    ips_writeData(0x00);
    else if (IPS_DISPLAY_DIR==1)    ips_writeData(0xC0);
    else if (IPS_DISPLAY_DIR==2)    ips_writeData(0x70);
    else                            ips_writeData(0xA0);
	
	ips_writeIndex(0x3A); 
	ips_writeData(0x05);

    ips_writeIndex(0xB2);
    ips_writeData(0x0C);
    ips_writeData(0x0C);
    ips_writeData(0x00);
    ips_writeData(0x33);
    ips_writeData(0x33); 

    ips_writeIndex(0xB7); 
    ips_writeData(0x35);  

    ips_writeIndex(0xBB);
    ips_writeData(0x37);

    ips_writeIndex(0xC0);
    ips_writeData(0x2C);

    ips_writeIndex(0xC2);
    ips_writeData(0x01);

    ips_writeIndex(0xC3);
    ips_writeData(0x12);   

    ips_writeIndex(0xC4);
    ips_writeData(0x20);  

    ips_writeIndex(0xC6); 
    ips_writeData(0x0F);    

    ips_writeIndex(0xD0); 
    ips_writeData(0xA4);
    ips_writeData(0xA1);

    ips_writeIndex(0xE0);
    ips_writeData(0xD0);
    ips_writeData(0x04);
    ips_writeData(0x0D);
    ips_writeData(0x11);
    ips_writeData(0x13);
    ips_writeData(0x2B);
    ips_writeData(0x3F);
    ips_writeData(0x54);
    ips_writeData(0x4C);
    ips_writeData(0x18);
    ips_writeData(0x0D);
    ips_writeData(0x0B);
    ips_writeData(0x1F);
    ips_writeData(0x23);

    ips_writeIndex(0xE1);
    ips_writeData(0xD0);
    ips_writeData(0x04);
    ips_writeData(0x0C);
    ips_writeData(0x11);
    ips_writeData(0x13);
    ips_writeData(0x2C);
    ips_writeData(0x3F);
    ips_writeData(0x44);
    ips_writeData(0x51);
    ips_writeData(0x2F);
    ips_writeData(0x1F);
    ips_writeData(0x1F);
    ips_writeData(0x20);
    ips_writeData(0x23);

    ips_writeIndex(0x21); 

    ips_writeIndex(0x11); 
    systick_delay_ms(120); 

    ips_writeIndex(0x29);
    ips_clear(IPS_BGCOLOR);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips_writeIndex(uint8 dat)			//写命令
{	
    IPS_DC(0);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);			

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips_writeData(uint8 dat)			//写数据
{
    IPS_DC(1);
    spi_mosi(spi2,SPI_PCS0,&dat,&dat,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips_writedata_16bit(uint16 dat)	//向液晶屏写一个16位数据
{
    uint8 h,l;
    h = dat >> 8;
    l = (uint8)dat;
    
    IPS_DC(1);
    spi_mosi(spi2,SPI_PCS0,&h,&h,1); 	//写入高8位数据
    spi_mosi(spi2,SPI_PCS0,&l,&l,1);	//写入低8位数据
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      1.14寸 IPS液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------



void ips_set_region(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	if(IPS_DISPLAY_DIR==0)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+52);
		ips_writedata_16bit(x2+52);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+40);
		ips_writedata_16bit(y2+40);
		ips_writeIndex(0x2c);//储存器写
	}
	else if(IPS_DISPLAY_DIR==1)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+53);
		ips_writedata_16bit(x2+53);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+40);
		ips_writedata_16bit(y2+40);
		ips_writeIndex(0x2c);//储存器写
	}
	else if(IPS_DISPLAY_DIR==2)
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+40);
		ips_writedata_16bit(x2+40);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+53);
		ips_writedata_16bit(y2+53);
		ips_writeIndex(0x2c);//储存器写
	}
	else
	{
		ips_writeIndex(0x2a);//列地址设置
		ips_writedata_16bit(x1+40);
		ips_writedata_16bit(x2+40);
		ips_writeIndex(0x2b);//行地址设置
		ips_writedata_16bit(y1+52);
		ips_writedata_16bit(y2+52);
		ips_writeIndex(0x2c);//储存器写
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_clear(IPS_YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void ips_clear(uint16 color)
{
    uint16 i,j;
	ips_set_region(0,0,IPS_X_MAX-1,IPS_Y_MAX-1);
    for(i=0;i<IPS_X_MAX;i++)
    {
        for (j=0;j<IPS_Y_MAX;j++)
	   	{
        	ips_writedata_16bit(color);	 			 
	    }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x     	        坐标x方向的起点
//  @param      y     	        坐标y方向的起点
//  @param      dat       	    需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_drawpoint(0,0,IPS_RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void ips_drawpoint(uint16 x,uint16 y,uint16 color)
{
    ips_set_region(x,y,x,y);
    ips_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX-1）
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips_showchar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;
	
	for(i=0; i<16; i++)
	{
		ips_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	
            {
                ips_writedata_16bit(IPS_PENCOLOR);
            }
			else			ips_writedata_16bit(IPS_BGCOLOR);
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips_showstr(uint16 x,uint16 y,uint8 dat[])
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		ips_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位有符号
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showint8(0,0,x);//x为int8类型
//-------------------------------------------------------------------------------------------------------------------
void ips_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		ips_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips_showchar(x,y*16,' ');
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showuint8(0,0,x);//x为uint8类型
//-------------------------------------------------------------------------------------------------------------------
void ips_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位有符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showint16(0,0,x);//x为int16类型
//-------------------------------------------------------------------------------------------------------------------
void ips_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		ips_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips_showuint16(0,0,x);//x为uint16类型
//-------------------------------------------------------------------------------------------------------------------
void ips_showuint16(uint16 x,uint16 y,uint16 dat)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @return     void
//  @since      v1.0
//  Sample usage:              
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void ips_displayimage032(uint8 *p, uint16 width, uint16 height) 
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;
	
    uint16 coord_x = 0;
    uint16 coord_y = 0;
    
    coord_x = width>IPS_X_MAX?IPS_X_MAX:width;
    coord_y = height>IPS_Y_MAX?IPS_Y_MAX:height;
    ips_set_region(0,0,coord_x-1,coord_y-1);
    
    for(j=0;j<coord_y;j++)
    {
        for(i=0;i<coord_x;i++)
        {
            temp = *(p+j*width+i*width/coord_x);//读取像素点
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips_writedata_16bit(color); 
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶缩放显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @param      dis_width       图像显示宽度  0 -（IPS_X_MAX-1）
//  @param      dis_height      图像显示高度  0 -（IPS_Y_MAX-1）
//  @return     void
//  @since      v1.0
//  Sample usage:              
//  @note       图像的宽度如果超过液晶的宽度，则自动进行缩放显示。这样可以显示全视野
//-------------------------------------------------------------------------------------------------------------------
void ips_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i,j;
                
    uint16 color = 0;
	uint16 temp = 0;

    ips_set_region(0,0,dis_width-1,dis_height-1);//设置显示区域 
    
    for(j=0;j<dis_height;j++)
    {
        for(i=0;i<dis_width;i++)
        {
            temp = *(p+(j*120/dis_height)*width+i*width/dis_width);//读取像素点
            color=(0x001f&((temp)>>3))<<11;
            color=color|(((0x003f)&((temp)>>2))<<5);
            color=color|(0x001f&((temp)>>3));
            ips_writedata_16bit(color); 
        }
    }
}





/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32 mypow(uint8 m,uint8 n)
{
    uint32 result=1;	 
    while(n--)result*=m;    
    return result;
}

/******************************************************************************
      函数说明：显示小数
      入口数据：x,y    起点坐标
                data    要显示的小数
      返回值：  无
******************************************************************************/
void ips_showfloat(uint16 x,uint16 y,float data)
{
	int temp;
	y *= 16;
	temp = (int)data;
	ips_showchar(x, y, ((temp % 1000) / 100) + 48);
	ips_showchar(x + 7, y, ((temp % 100) / 10) + 48);
	ips_showchar(x + 14, y, (temp % 10) + 48);

	ips_showchar(x + 21, y, '.');

	data = data * 100000 + 1;
	temp = (int)data;
	temp %= 100000;
	ips_showchar(x + 28, y, (temp / 10000) + 48);
	ips_showchar(x + 35, y, ((temp % 10000) / 1000) + 48);
	ips_showchar(x + 42, y, ((temp % 1000) / 100) + 48);
//    uint8 t,temp;
//    uint16 num1;
//    num1=(uint16)(num*100);
//    for(t=0;t<len;t++)
//    {
//        temp=(num1/mypow(10,len-t-1))%10;
//        if(t==(len-2))
//        {
//            ips_showchar(x+8*(len-2),y*16,'.');
//            t++;
//            len+=1;
//        }
//        ips_showchar(x+8*t,y*16,temp + '0');
//    }
}

void ips_showReversechar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;
	
	for(i=0; i<16; i++)
	{
		ips_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	
            {
                ips_writedata_16bit(IPS_RED);
            }
			else			ips_writedata_16bit(IPS_BGCOLOR);
			temp>>=1;
		}
	}
}

/******************************************************************************
      函数说明：反色显示小数
      入口数据：x,y    起点坐标
                data    要显示的小数
      返回值：  无
******************************************************************************/

void ips_showReversefloat(uint16 x,uint16 y,float data, int8 Bit)
{
	int temp1, temp2;
	
	y *= 16;
	temp1 = (int)data;
	ips_showchar(x, y, ((temp1 % 1000) / 100) + 48);
	ips_showchar(x + 7, y, ((temp1 % 100) / 10) + 48);
	ips_showchar(x + 14, y, (temp1 % 10) + 48);

	ips_showchar(x + 21, y, '.');

	data = data * 100000 + 1;
	temp2 = (int)data;
	temp2 %= 100000;
	ips_showchar(x + 28, y, (temp2 / 10000) + 48);
	ips_showchar(x + 35, y, ((temp2 % 10000) / 1000) + 48);
	ips_showchar(x + 42, y, ((temp2 % 1000) / 100) + 48);
	
	switch (Bit)
	{
	case -3:
		ips_showReversechar(x + 42, y, ((temp2 % 1000) / 100) + 48);
		break;
	case -2:
		ips_showReversechar(x + 35, y, ((temp2 % 10000) / 1000) + 48);
		break;
	case -1:
		ips_showReversechar(x + 28, y, (temp2 / 10000) + 48);
		break;
	case 0:
		ips_showReversechar(x + 14, y, (temp1 % 10) + 48);
		break;
	case 1:
		ips_showReversechar(x + 7, y, ((temp1 % 100) / 10) + 48);
		break;
	case 2:
		ips_showReversechar(x, y, ((temp1 % 1000) / 100) + 48);
		break;
	default:
		break;
	}
//    uint8 t,temp;
//    uint16 num1;
//    num1=(uint16)(num*100);
//    for(t=0;t<len;t++)
//    {
//        temp=(num1/mypow(10,len-t-1))%10;
//        if(t==(len-2))
//        {
//            ips_showchar(x+8*(len-2),y*16,'.');
//            t++;
//            len+=1;
//        }
//        ips_showchar(x+8*t,y*16,temp + '0');
//    }
}

void OLED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
{
    unsigned char m, i, j, k;
    m = data / 1000;
    i = (data % 1000) / 100;
    j = (data % 100) / 10;
    k = data % 10;
    switch (num)
    {
    case 1:  	
        ips_showchar(x + 8, y, k + 48);
        break;
    case 2:  	
        ips_showchar(x + 8, y, j + 48);
        ips_showchar(x + 16, y, k + 48);
        break;
    case 3:
        ips_showchar(x + 8, y, i + 48);
        ips_showchar(x + 16, y, j + 48);
        ips_showchar(x + 24, y, k + 48);
        break;
    case 4: 	
        ips_showchar(x + 8, y, m + 48);
        ips_showchar(x + 16, y, i + 48);
        ips_showchar(x + 24, y, j + 48);
        ips_showchar(x + 32, y, k + 48);
        break;
    }
}

void OLED_PrintValueI(unsigned char x, unsigned char y, int data, unsigned char num)
{
    unsigned char i, j, k, l, m;
    
    y = y * 16;
    
    if (data < 0)
    {
        ips_showchar(x, y, '-');
        data = -data;
    }
    else
    {
        ips_showchar(x, y, '+');
    }
    
    l = data / 10000;
    m = (data % 10000) / 1000;
    i = (data % 1000) / 100;
    j = (data % 100) / 10;
    k = data % 10;
    
    if (num == 1)
    {
        ips_showchar(x + 8, y, k + 48);
    }
    if (num == 2)
    {
        ips_showchar(x + 8, y, j + 48);
        ips_showchar(x + 16, y, k + 48);
    }
    if (num == 3)
    {
        ips_showchar(x + 8, y, i + 48);
        ips_showchar(x + 16, y, j + 48);
        ips_showchar(x + 24, y, k + 48);
    }
    if (num == 4)
    {
        ips_showchar(x + 8, y, m + 48);
        ips_showchar(x + 16, y, i + 48);
        ips_showchar(x + 24, y, j + 48);
        ips_showchar(x + 32, y, k + 48);
    }
    if (num == 5)
    {
        ips_showchar(x + 8, y, l + 48);
        ips_showchar(x + 16, y, m + 48);
        ips_showchar(x + 24, y, i + 48);
        ips_showchar(x + 32, y, j + 48);
        ips_showchar(x + 40, y, k + 48);
    }
}

void OLED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num)
{
    unsigned char l, m, i, j, k;  //万千百十个
    unsigned char databiti = 6; //整数位数
    unsigned int tempdataui = 0;
    int tempdataii = (int)data; //整数部分
    long int tempdatalp = (long int)((data - (int)data) * 10000); //取小数位后4位
    
    y = y * 16;
    
    //整数部分显示
    if (data < 0.0000001f)  ips_showchar(x, y, '-');
    else ips_showchar(x, y, '+');
    if (tempdataii < 0)tempdataii = -tempdataii;  //去掉整数部分负号
    tempdataui = tempdataii;
    l = tempdataui / 10000;
    m = (tempdataui % 10000) / 1000;
    i = (tempdataui % 1000) / 100;
    j = (tempdataui % 100) / 10;
    k = tempdataui % 10;
    if (l != 0)  //五位
    {
        ips_showchar(x + 8, y, l + 48);
        ips_showchar(x + 16, y, m + 48);
        ips_showchar(x + 24, y, i + 48);
        ips_showchar(x + 32, y, j + 48);
        ips_showchar(x + 40, y, k + 48);
    }
    else if (m != 0) //四位
    {
        databiti = 5;
        ips_showchar(x + 8, y, m + 48);
        ips_showchar(x + 16, y, i + 48);
        ips_showchar(x + 24, y, j + 48);
        ips_showchar(x + 32, y, k + 48);
    }
    else if (i != 0) //三位
    {
        databiti = 4;
        ips_showchar(x + 8, y, i + 48);
        ips_showchar(x + 16, y, j + 48);
        ips_showchar(x + 24, y, k + 48);
    }
    else if (j != 0) //两位
    {
        databiti = 3;
        ips_showchar(x + 8, y, j + 48);
        ips_showchar(x + 16, y, k + 48);
    }
    else
    {
        databiti = 2;
        ips_showchar(x + 8, y, k + 48);
    }
    if (tempdatalp < 0)tempdatalp = -tempdatalp;	//去掉小数部分负号
    switch (num)
    {
    case 0: break;
    case 1:  ips_showchar(x + databiti * 8, y, '.'); OLED_PrintValueFP(x + databiti * 8, y, (unsigned int)(tempdatalp / 1000), num); break;
    case 2:  ips_showchar(x + databiti * 8, y, '.'); OLED_PrintValueFP(x + databiti * 8, y, (unsigned int)(tempdatalp / 100), num); break;
    case 3:  ips_showchar(x + databiti * 8, y, '.'); OLED_PrintValueFP(x + databiti * 8, y, (unsigned int)(tempdatalp / 10), num); break;
    case 4:  ips_showchar(x + databiti * 8, y, '.'); OLED_PrintValueFP(x + databiti * 8, y, (unsigned int)(tempdatalp), num); break;
    }
}

void oled_clear(void)
{
	ips_clear(IPS_BLACK);
}

void showimage(void)
{
	// OLED显示图像划线
	if (Findline.endline != 0)
	{
		for(uint8 i=119;i>=Findline.endline;i--)     
		{
			if(Findline.midline[i]>=0 && Findline.midline[i]<=159) Image[i][Findline.midline[i]]=0;
			if(Findline.leftline[i]>=0 && Findline.leftline[i]<=159) Image[i][Findline.leftline[i]]=0;
			if(Findline.rightline[i]>=0 && Findline.rightline[i]<=159) Image[i][Findline.rightline[i]]=0;
		}
	}
	if (Findline.endline != 0)
	{
		for(uint8 i=119;i>=Findline.endline;i--)     
		{
			if(Findline.midline[i]>=0 && Findline.midline[i]<=159) SobelImage0[i][Findline.midline[i]]=0;
			if(Findline.leftline[i]>=0 && Findline.leftline[i]<=159) SobelImage0[i][Findline.leftline[i]]=0;
			if(Findline.rightline[i]>=0 && Findline.rightline[i]<=159) SobelImage0[i][Findline.rightline[i]]=0;
		}
	}
	if (ui.cursor[1] % 2)
		ips_displayimage032((uint8*)Image, COLUMN, ROW);
	else
		ips_displayimage032((uint8*)SobelImage0, COLUMN, ROW);
}