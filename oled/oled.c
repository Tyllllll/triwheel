#include "oled.h"

/****Variables************************************************/

const unsigned char F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },   // '\'
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x08, 0x08, 0x49, 0x2A, 0x14, 0x08 }    // cursor
};

/****Function list********************************************/

void OLED_WriteData(unsigned char data);
void OLED_WriteCmd(unsigned char cmd);
void OLED_SetPosition(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_data);
void OLED_DelayMs(unsigned int ms);

void SetStartColumn(unsigned char column);
void SetAddressingMode(unsigned char mode);
void SetColumnAddress(unsigned char begin, unsigned char end);
void SetPageAddress(unsigned char begin, unsigned char end);
void SetStartLine(unsigned char line);
void SetContrastControl(unsigned char contrast);
void SetChargePump(unsigned char cmd);
void SetSegmentRemap(unsigned char cmd);
void SetEntireDisplay(unsigned char cmd);
void SetInverseDisplay(unsigned char cmd);
void SetMultiplexRatio(unsigned char cmd);
void SetDisplay_On_Off(unsigned char cmd);
void SetStartPage(unsigned char address);
void SetCommonRemap(unsigned char dir);
void SetDisplayOffset(unsigned char cmd);
void SetDisplayClock(unsigned char cmd);
void SetPrechargePeriod(unsigned char cmd);
void SetCommonConfig(unsigned char cmd);
void SetVCOMH(unsigned char cmd);
void SetNOP(void);
void OLED_Init(void);
void OLED_GPIO_Config(void);

void OLED_Put6x8Char(unsigned char x, unsigned char y, unsigned char ch);
void OLED_Put6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
void OLED_PrintCharValue(unsigned char x, unsigned char y, unsigned char data);
void OLED_PrintIntValue(unsigned char x, unsigned char y, int data);
void OLED_PrintUintValue(unsigned char x, unsigned char y, unsigned int data);
void OLED_PrintFloatValue(unsigned char x, unsigned char y, float data);
void OLED_PrintReverseFloatValue(unsigned char x, unsigned char y, float data, int8 Bit);
void Showimage(void);
void SetNumDisplay(unsigned char x, unsigned char y, unsigned int num);


/****Objects**************************************************/

OLED_CLASS oled =
{
	&OLED_Init,
	&OLED_Put6x8Char,
	&OLED_Put6x8Str,
	&OLED_PrintCharValue,
	&OLED_PrintIntValue,
	&OLED_PrintUintValue,
	&OLED_PrintFloatValue,
	&OLED_PrintReverseFloatValue,
	&Showimage,
	&SetNumDisplay,
};

/****Functions************************************************/

void OLED_WriteData(unsigned char data)
{
	unsigned char i = 8;

	OLED_DC(1);
	OLED_SCLK(0);

	while(i--)
	{
		if(data & 0x80)
			OLED_SDA(1);
		else
			OLED_SDA(0);

		OLED_SCLK(1);
		asm("nop");
		OLED_SCLK(0);

		data <<= 1;
	}
}

void OLED_WriteCmd(unsigned char cmd)
{
	unsigned char i = 8;

	OLED_DC(0);
	OLED_SCLK(0);

	while(i--)
	{
		if(cmd & 0x80)
			OLED_SDA(1);
		else
			OLED_SDA(0);

		OLED_SCLK(1);
		asm("nop");
		OLED_SCLK(0);

		cmd <<= 1;
	}
}

void OLED_SetPosition(unsigned char x, unsigned char y)
{
	OLED_WriteCmd(0xB0 + y);
	OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);
	OLED_WriteCmd((x & 0x0F) | 0x00);
}

void OLED_Fill(unsigned char bmp_data)
{
	unsigned char x, y;

	for(y = 0 ; y < 8 ; y++)
	{
		OLED_WriteCmd(0xB0 + y);	// 0xb0 + 0~7 means page 0~7
		OLED_WriteCmd(0x00);	// 0x00 + 0~16 means 128 pixs / 16
		OLED_WriteCmd(0x10);	// 0x10 + 0~16 means 128 pixs / 16 groups

		for(x = 0 ; x < 128 ; x++)
			OLED_WriteData(bmp_data);
	}
}

void OLED_DelayMs(unsigned int ms)
{
	uint16 i;
	while(ms--)
	{
		i = 6675;
		while(i--);
	}
}

void SetStartColumn(unsigned char column)
{
	OLED_WriteCmd(0x00 + column % 16);	// Set Lower Column Start Address for Page Addressing Mode
										// Default => 0x00
	OLED_WriteCmd(0x10 + column / 16);	// Set Higher Column Start Address for Page Addressing Mode
										// Default => 0x10
}

void SetAddressingMode(unsigned char mode)
{
	OLED_WriteCmd(0x20);	// Set Memory Addressing Mode
	OLED_WriteCmd(mode);	// Default => 0x02
							// 0x00 => Horizontal Addressing Mode
							// 0x01 => Vertical Addressing Mode
							// 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char begin, unsigned char end)
{
	OLED_WriteCmd(0x21);	// Set Column Address
	OLED_WriteCmd(begin);	// Default => 0x00 (Column Begin Address)
	OLED_WriteCmd(end);		// Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char begin, unsigned char end)
{
	OLED_WriteCmd(0x22);	// Set Page Address
	OLED_WriteCmd(begin);	// Default => 0x00 (Page Begin Address)
	OLED_WriteCmd(end);		// Default => 0x7F (Page End Address)
}

void SetStartLine(unsigned char line)
{
	OLED_WriteCmd(0x40 | line);	// Set Display Start Line
								// Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char contrast)
{
	OLED_WriteCmd(0x81);		// Set Contrast Control
	OLED_WriteCmd(contrast);	// Default => 0x7F
}

void SetChargePump(unsigned char cmd)
{
	OLED_WriteCmd(0x8D);		// Set Charge Pump
	OLED_WriteCmd(0x10 | cmd);	// Default => 0x10
								// 0x10 (0x00) => Disable Charge Pump
								// 0x14 (0x04) => Enable Charge Pump
}

void SetSegmentRemap(unsigned char cmd)
{
	OLED_WriteCmd(0xA0 | cmd);	// Set Segment Re-Map
								// Default => 0xA0
								// 0xA0 (0x00) => Column Address 0 Mapped to SEG0
								// 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SetEntireDisplay(unsigned char cmd)
{
	OLED_WriteCmd(0xA4 | cmd);	// Set Entire Display On / Off
								// Default => 0xA4
								// 0xA4 (0x00) => Normal Display
								// 0xA5 (0x01) => Entire Display On
}

void SetInverseDisplay(unsigned char cmd)
{
	OLED_WriteCmd(0xA6 | cmd);	// Set Inverse Display On/Off
								// Default => 0xA6
								// 0xA6 (0x00) => Normal Display
								// 0xA7 (0x01) => Inverse Display On
}

void SetMultiplexRatio(unsigned char cmd)
{
	OLED_WriteCmd(0xA8);	// Set Multiplex Ratio
	OLED_WriteCmd(cmd);		// Default => 0x3F (1/64 Duty)
}

void SetDisplay_On_Off(unsigned char cmd)
{
	OLED_WriteCmd(0xAE | cmd);	// Set Display On/Off
								// Default => 0xAE
								// 0xAE (0x00) => Display Off
								// 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char address)
{
	OLED_WriteCmd(0xB0 | address);	// Set Page Start Address for Page Addressing Mode
									// Default => 0xB0 (0x00)
}

void SetCommonRemap(unsigned char dir)
{
	OLED_WriteCmd(0xC0 | dir);	// Set COM Output Scan Direction
								// Default => 0xC0
								// 0xC0 (0x00) => Scan from COM0 to 63
								// 0xC8 (0x08) => Scan from COM63 to 0
}

void SetDisplayOffset(unsigned char cmd)
{
	OLED_WriteCmd(0xD3);	// Set Display Offset
	OLED_WriteCmd(cmd);		// Default => 0x00
}

void SetDisplayClock(unsigned char cmd)
{
	OLED_WriteCmd(0xD5);	// Set Display Clock Divide Ratio / Oscillator Frequency
	OLED_WriteCmd(cmd);		// Default => 0x80
							// D[3:0] => Display Clock Divider
							// D[7:4] => Oscillator Frequency
}

void SetPrechargePeriod(unsigned char cmd)
{
	OLED_WriteCmd(0xD9);	// Set Pre-Charge Period
	OLED_WriteCmd(cmd);		// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
							// D[3:0] => Phase 1 Period in 1~15 Display Clocks
							// D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SetCommonConfig(unsigned char cmd)
{
	OLED_WriteCmd(0xDA);		// Set COM Pins Hardware Configuration
	OLED_WriteCmd(0x02 | cmd);	// Default => 0x12 (0x10)
								// Alternative COM Pin Configuration
								// Disable COM Left/Right Re-Map
}

void SetVCOMH(unsigned char cmd)
{
	OLED_WriteCmd(0xDB);	// Set VCOMH Deselect Level
	OLED_WriteCmd(cmd);		// Default => 0x20 (0.77*VCC)
}

void SetNOP(void)
{
	OLED_WriteCmd(0xE3);	// Command for No Operation
}

void OLED_Init(void)
{
	OLED_GPIO_Config();

	OLED_DC(0);
	OLED_SDA(0);
	OLED_SCLK(0);
	OLED_RST(0);
	OLED_DelayMs(50);
	OLED_SCLK(1);
	OLED_RST(0);
	OLED_DelayMs(50);
	OLED_RST(1);

	SetDisplay_On_Off(0x00);	// Display Off (0x00/0x01)
	SetDisplayClock(0x80);		// Set Clock as 100 Frames/Sec
	SetMultiplexRatio(0x3F);	// 1/64 Duty (0x0F~0x3F)
	SetDisplayOffset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	SetChargePump(0x04);		// Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	// Set Page Addressing Mode (0x00/0x01/0x02)
	SetSegmentRemap(0x01);		// Set SEG/Column Mapping     0x00左右反置 0x01正常
	SetCommonRemap(0x08);		// Set COM/Row Scan Direction 0x00上下反置 0x08正常
	SetCommonConfig(0x10);		// Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xCF);	// Set SEG Output Current
	SetPrechargePeriod(0xF1);	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SetVCOMH(0x40);				// Set VCOM Deselect Level
	SetEntireDisplay(0x00);		// Disable Entire Display On (0x00/0x01)
	SetInverseDisplay(0x00);	// Disable Inverse Display On (0x00/0x01)
	SetDisplay_On_Off(0x01);	// Display On (0x00/0x01)
	OLED_Fill(0x00);
	OLED_SetPosition(0,0);
}

void OLED_GPIO_Config(void)
{
	gpio_init(OLED_GPIO_SCLK, GPO, 1);
	gpio_init(OLED_GPIO_SDA, GPO, 1);
	gpio_init(OLED_GPIO_RST, GPO, 1);
	gpio_init(OLED_GPIO_DC, GPO, 1);
	gpio_init(OLED_GPIO_CS, GPO, 0);
}

void OLED_Put6x8Char(unsigned char x, unsigned char y, unsigned char ch)
{
	unsigned char c = 0, i = 0;

	c = ch - 32;

	if(x > 122)
	{
		x = 0;
		y++;
	}
	OLED_SetPosition(x, y);

	for(i = 0 ; i < 6 ; i++)
		OLED_WriteData(F6x8[c][i]);
}

void OLED_Put6x8Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c = 0, i = 0, j = 0;

	while(ch[j] != '\0')
	{
		c = ch[j] - 32;

		if(x > 126)
		{
			x = 0;
			y++;
		}
		OLED_SetPosition(x, y);

		for(i = 0 ; i < 6 ; i++)
			OLED_WriteData(F6x8[c][i]);
		x += 6;
		j++;
	}
}

void OLED_PrintCharValue(unsigned char x, unsigned char y, unsigned char data)
{
	OLED_Put6x8Char(x, y, (data / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (data % 10) + 48);
}

void OLED_PrintIntValue(unsigned char x, unsigned char y, int data)
{
	if(data < 0)
	{
			OLED_Put6x8Char(x, y, '-');
			data = -data;
	}
	else
			OLED_Put6x8Char(x, y, '+');

	OLED_Put6x8Char(x + 6, y, (data / 1000) + 48);
	OLED_Put6x8Char(x + 12, y, ((data % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 18, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 24, y, (data % 10) + 48);
}

void OLED_PrintUintValue(unsigned char x, unsigned char y, unsigned int data)
{
	OLED_Put6x8Char(x, y, (data / 1000) + 48);
	OLED_Put6x8Char(x + 6, y, ((data % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 12, y, ((data % 100) / 10) + 48);
	OLED_Put6x8Char(x + 18, y, (data % 10) + 48);
}

void OLED_PrintFloatValue(unsigned char x, unsigned char y, float data)
{
	int temp;

	temp = (int)data;
	OLED_Put6x8Char(x, y, ((temp % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((temp % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (temp % 10) + 48);

	OLED_Put6x8Char(x + 18, y, '.');

	data = data * 100000 + 1;
	temp = (int)data;
	temp %= 100000;
	OLED_Put6x8Char(x + 24, y, (temp / 10000) + 48);
	OLED_Put6x8Char(x + 30, y, ((temp % 10000) / 1000) + 48);
	OLED_Put6x8Char(x + 36, y, ((temp % 1000) / 100) + 48);
}

void OLED_PutReverse6x8Char(unsigned char x, unsigned char y, unsigned char ch)
{
	unsigned char c = 0, i = 0;

	c = ch - 32;

	if(x > 122)
	{
		x = 0;
		y++;
	}
	OLED_SetPosition(x, y);

	for(i = 0 ; i < 6 ; i++)
		OLED_WriteData(~F6x8[c][i]);
}

void OLED_PrintReverseFloatValue(unsigned char x, unsigned char y, float data, int8 Bit)
{
	int temp1, temp2;

	temp1 = (int)data;
	OLED_Put6x8Char(x, y, ((temp1 % 1000) / 100) + 48);
	OLED_Put6x8Char(x + 6, y, ((temp1 % 100) / 10) + 48);
	OLED_Put6x8Char(x + 12, y, (temp1 % 10) + 48);

	OLED_Put6x8Char(x + 18, y, '.');

	data = data * 100000 + 1;
	temp2 = (int)data;
	temp2 %= 100000;
	OLED_Put6x8Char(x + 24, y, (temp2 / 10000) + 48);
	OLED_Put6x8Char(x + 30, y, ((temp2 % 10000) / 1000) + 48);
	OLED_Put6x8Char(x + 36, y, ((temp2 % 1000) / 100) + 48);
	
	switch (Bit)
	{
	case -3:
		OLED_PutReverse6x8Char(x + 36, y, ((temp2 % 1000) / 100) + 48);
		break;
	case -2:
		OLED_PutReverse6x8Char(x + 30, y, ((temp2 % 10000) / 1000) + 48);
		break;
	case -1:
		OLED_PutReverse6x8Char(x + 24, y, (temp2 / 10000) + 48);
		break;
	case 0:
		OLED_PutReverse6x8Char(x + 12, y, (temp1 % 10) + 48);
		break;
	case 1:
		OLED_PutReverse6x8Char(x + 6, y, ((temp1 % 100) / 10) + 48);
		break;
	case 2:
		OLED_PutReverse6x8Char(x, y, ((temp1 % 1000) / 100) + 48);
		break;
	default:
		break;
	}
}

void Showimage()
{
  int16 i,j,cnt,temp1,data;
  for (j = 0; j < 8; j++)
  {
    OLED_WriteCmd(0xb0 + j);
    OLED_WriteCmd(0x01);
    OLED_WriteCmd(0x10);
    for (i = 0; i < 80; i++)
    {
      temp1 = 0;
      for ( cnt = 7; cnt >= 0; cnt--)
      {
        temp1 |= SobelImage0[(j * 8 + cnt) ][i ] / 253;
        temp1 = temp1 << 1;
      }
      temp1 |= SobelImage0[(j * 8) ][i ] / 253;
      data = temp1;
      OLED_WriteData(data);
    }
  }
}


void SetNumDisplay(unsigned char x, unsigned char y, unsigned int num)
{
  unsigned char c = 0, i = 0;
	c = num+48 - 32;
	if(x > 122)
	{
		x = 0;
		y++;
	}
	OLED_SetPosition(x, y);
	for(i = 0 ; i < 6 ; i++)
		OLED_WriteData(~F6x8[c][i]);
}

void oled_clear(void)
{
	OLED_Fill(0x00);
}