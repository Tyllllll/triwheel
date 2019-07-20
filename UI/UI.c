#include "UI.h"

/****Definitions**********************************************/

#define CURSOR		('z' + 1)

/****Variables************************************************/
//发车
int8 ReadyChar[4] = {'R',':','R','D'};
Setting_Class setting;
static float EEPROMData[DATA_SIZE + 1];
//static float FactoryData[DATA_SIZE + 1];

/****Function list********************************************/

void UI_DispUIStrings(uint8 strings[8][22]);
void UI_Disp(void);
void DispImg(void);
void DispAD(void);
void DispGyro(void);
void DispOther(void);
void DispErase(void);
void DispEgg(void);
void DispParameter(void);

/****Objects**************************************************/

UI_CLASS ui =
{
	&UI_Disp,
	&DispImg,
	&DispAD,
	&DispGyro,
	&DispOther,
	&DispParameter
};

/****Functions************************************************/

void UI_Disp(void)
{
	if (speed.Reality < 1)
	{
		switch (ui.level)
		{
		case 0:
			UI_DispUIStrings(UI_Menu);
			break;
		case 1:
			switch (ui.cursor[0])
			{
			case 0:
				// 图像加路况参数
				DispImg();
				break;
			case 1:
				// AD值
				DispAD();
				break;
			case 2:
				// gyro
				DispGyro();
				break;
			case 3:
				// others
				DispOther();
				break;
			case 4:
				// paramenterSetting
				DispEgg();
				break;
			case 5:
				// erase flash
				DispErase();
				break;
			}
			break;
		case 2:
			switch (ui.cursor[0])
			{
			// 按键发车
			case 0:
                bee.time = 200;
                systick_delay_ms(1000);
                ui.level = 1;
                ui.startbutton_flag = 1;
                ui.areYouReady = 0;

                Findline.Process = Normal;
                crossmeet.HasCrossMeet = 0;
                Findline.loseflag = 0;
                //speed.distance = 5;
                speed.Distance_cnt = 0;
                Findline.err[0] = 0;
                Findline.Odometer = 0;
                motor.crazyRunCounter = 0;
                
                loopArray.loopFinishNum = 0;
                
                for(int8 i = 0; i < 6; i++)
                {
                    if(elementArray.ElementArray[i] == 0)
                    {
                        elementArray.elementNum = i+1;
                        break;
                    }
                    else
                        elementArray.elementNum = 6;
                }
                if(crossmeet.reverse == 0)
                    elementArray.elementFinishNum = 0;
                else
                    elementArray.elementFinishNum = elementArray.elementNum-1;
				break;
			case 4:
				DisableInterrupts;
				DispParameter();
				ui.level--;
				EnableInterrupts;
				break;
			case 5:
				if (ui.cursor[1] == 6)
				{
					uint8 result = 1;
					DisableInterrupts;
					result = FLASH_EraseSector(FLASH_TEST_SECTOR);
					EnableInterrupts;
					oled_clear();
					if (result == 0)
					{
						oled.Printf(6, 2, "Erase success.");
						for (int8 i = 0; i < 64; i++)
						{
							EEPROMData[i] = 0.0 / 0.0;
						}
					}
					else
						oled.Printf(6, 2, "Failed");
					systick_delay_ms(1000);
					ui.level = 0;
				}
				else if (ui.cursor[1] == 7)
				{
					ui.level = 0;
				}
				else
				{
					ui.level--;
				}
				break;
			default:
				ui.level--;
				break;
			}
			break;
		default:
			ui.level = 0;
			break;
		}
	}
}

void DispImg(void)
{
	oled.Showimage();
	if (ui.cursor[1] == 0 || ui.cursor[1] == 1)
	{
		for (unsigned char i = 0; i < 8; i++)
			oled.Printf(81, i, UI_Image[i]);
		oled.PrintCharValue(108,1,(uint8)ui.fps_timer_counter[0]);
		oled.Printf(81,3,UI_RoadElement[Findline.Process]);
		oled.PrintFloatValue(81,5,gyro.Car_Angle);
		oled.PrintFloatValue(81,7,elementArray.elementFinishNum);
//		oled.PrintFloatValue(81,7,gyro.Car_Angle);
	}
}

void DispAD(void)
{
	UI_DispUIStrings(UI_Sensor);
    oled.PrintUintValue(90,0,sensor.advalue.ad_avr_val[0]);
	oled.PrintUintValue(90,1,sensor.advalue.ad_avr_val[1]);
	oled.PrintUintValue(90,2,sensor.advalue.ad_avr_val[2]);
	oled.PrintUintValue(90,3,sensor.advalue.ad_avr_val[3]);
	oled.PrintUintValue(90,4,sensor.advalue.ad_avr_val[4]);
	oled.PrintUintValue(90,5,sensor.advalue.ad_avr_val[5]);
	oled.PrintUintValue(90,6,sensor.advalue.ad_avr_val[6]);
    oled.PrintUintValue(90,7,sensor.advalue.ad_avr_val[7]);
}

void DispGyro(void)
{
	UI_DispUIStrings(UI_Gryo);
	oled.PrintFloatValue(75,2, gyro.Car_Angle);
	oled.PrintFloatValue(75,3, gyro.TurnAngle_Integral);
}

void DispOther(void)
{
	UI_DispUIStrings(UI_Others);
}

void DispErase(void)
{
	UI_DispUIStrings(UI_Erase);
}

void Parameter_Setting_Init(void)
{
	uint8 i;
	float sum = 0;
	for (i = 0; i < DATA_SIZE + 1; i++)
	{
		EEPROMData[i] = 0;
		EEPROMData[i] = (*(float *)((uint32)(((FLASH_TEST_SECTOR)*4096) + (i * 4))));
	}
	for (uint8 i = 64; i >= 60; i--)
	{
		sum += EEPROMData[i];
	}
	if (sum == EEPROMData[0]) 
	{
		ui.hasADMax = 1;
		
		loopArray.loopNum = (int8)EEPROMData[1];
		loopArray.LoopArray[0] = (int8)EEPROMData[2];
		loopArray.LoopArray[1] = (int8)EEPROMData[3];
		loopArray.LoopArray[2] = (int8)EEPROMData[4];
		loopArray.LoopArray[3] = (int8)EEPROMData[5];
		loopArray.LoopArray[4] = (int8)EEPROMData[6];
		loopArray.LoopArray[5] = (int8)EEPROMData[7];
		

        crossmeet.reverse = (int8)EEPROMData[8];
        elementArray.ElementArray[0] = (int8)EEPROMData[9];
        elementArray.ElementArray[1] = (int8)EEPROMData[10];
        elementArray.ElementArray[2] = (int8)EEPROMData[11];
        elementArray.ElementArray[3] = (int8)EEPROMData[12];
        elementArray.ElementArray[4] = (int8)EEPROMData[13];
        elementArray.ElementArray[5] = (int8)EEPROMData[14];     
        
		sensor.advalue.ad_offset_val[EEEL] = (uint16)EEPROMData[55];
		sensor.advalue.ad_offset_val[EEEM] = (uint16)EEPROMData[56];
		sensor.advalue.ad_offset_val[EEER] = (uint16)EEPROMData[57];
		sensor.advalue.ad_offset_val[EECL] = (uint16)EEPROMData[58];
		sensor.advalue.ad_offset_val[EECR] = (uint16)EEPROMData[59];
		sensor.advalue.ad_max_val[EEEL] = (uint16)EEPROMData[60];
		sensor.advalue.ad_max_val[EEEM] = (uint16)EEPROMData[61];
		sensor.advalue.ad_max_val[EEER] = (uint16)EEPROMData[62];
		sensor.advalue.ad_max_val[EECL] = (uint16)EEPROMData[63];
		sensor.advalue.ad_max_val[EECR] = (uint16)EEPROMData[64];
	}
	
	sprintf(setting.string[0][0], "Loop 1Left 2Right");
	sprintf(setting.string[0][1], "Num");
	setting.data[0][1] = (float32)loopArray.loopNum;
	sprintf(setting.string[0][2], "1");
	setting.data[0][2] = (float32)loopArray.LoopArray[0];
    sprintf(setting.string[0][3], "2");
	setting.data[0][3] = (float32)loopArray.LoopArray[1];
    sprintf(setting.string[0][4], "3");
	setting.data[0][4] = (float32)loopArray.LoopArray[2];
    sprintf(setting.string[0][5], "4");
	setting.data[0][5] = (float32)loopArray.LoopArray[3];
    sprintf(setting.string[0][6], "5");
	setting.data[0][6] = (float32)loopArray.LoopArray[4];
    sprintf(setting.string[0][7], "6");
	setting.data[0][7] = (float32)loopArray.LoopArray[5];
	
	sprintf(setting.string[1][0], "Element 1TS 2RM");
	sprintf(setting.string[1][1], "Reverse");
	setting.data[1][1] = (float32)crossmeet.reverse;
	sprintf(setting.string[1][2], "1");
	setting.data[1][2] = (float32)elementArray.ElementArray[0];
	sprintf(setting.string[1][3], "2");
	setting.data[1][3] = (float32)elementArray.ElementArray[1];
	sprintf(setting.string[1][4], "3");
	setting.data[1][4] = (float32)elementArray.ElementArray[2];
	sprintf(setting.string[1][5], "4");
	setting.data[1][5] = (float32)elementArray.ElementArray[3];
	sprintf(setting.string[1][6], "5");
	setting.data[1][6] = (float32)elementArray.ElementArray[4];
	sprintf(setting.string[1][7], "6");
	setting.data[1][7] = (float32)elementArray.ElementArray[5];
	
//	sprintf(setting.string[1][0], "MOTOR");
//	sprintf(setting.string[1][1], "kp");
//	setting.data[1][1] = (float32)MotorPID.P;
//	sprintf(setting.string[1][2], "ki");
//	setting.data[1][2] = (float32)MotorPID.I;
//	sprintf(setting.string[1][3], "Vnorm");
//	setting.data[1][3] = (float32)speed.Stan;
//    sprintf(setting.string[1][4], "Vlamp");
//	setting.data[1][4] = (float32)speed.lamp;
//    sprintf(setting.string[1][5], "Vhuan");
//	setting.data[1][5] = (float32)speed.Roadabouts;
//    sprintf(setting.string[1][6], "Vlz");
//	setting.data[1][6] = (float32)speed.hengduan;
//    sprintf(setting.string[1][7], "Vzhi");
//	setting.data[1][7] = (float32)speed.straight;
//	
//	sprintf(setting.string[2][0], "OTHER");
//    sprintf(setting.string[2][1], "Dis");
//	setting.data[2][1] = (float32)Distance_Test;
}

void Save_Data(void)
{
	loopArray.loopNum = (int8)setting.data[0][1];
	loopArray.LoopArray[0] = (int8)setting.data[0][2];
	loopArray.LoopArray[1] = (int8)setting.data[0][3];
	loopArray.LoopArray[2] = (int8)setting.data[0][4];
	loopArray.LoopArray[3] = (int8)setting.data[0][5];
	loopArray.LoopArray[4] = (int8)setting.data[0][6];
	loopArray.LoopArray[5] = (int8)setting.data[0][7];
	
	crossmeet.reverse = (int8)setting.data[1][1];
	elementArray.ElementArray[0] = (int8)setting.data[1][2];
	elementArray.ElementArray[1] = (int8)setting.data[1][3];
	elementArray.ElementArray[2] = (int8)setting.data[1][4];
	elementArray.ElementArray[3] = (int8)setting.data[1][5];
	elementArray.ElementArray[4] = (int8)setting.data[1][6];
	elementArray.ElementArray[5] = (int8)setting.data[1][7];
	
//	MotorPID.P = setting.data[1][1];
//	MotorPID.I = setting.data[1][2];
//	speed.Stan = setting.data[1][3];
//	speed.lamp = setting.data[1][4];
//	speed.Roadabouts = setting.data[1][5];
//	speed.hengduan = setting.data[1][6];
//	speed.straight = setting.data[1][7];
//	
//	Distance_Test = setting.data[2][1];
	
	EEPROMData[0] = 0;
	
	EEPROMData[1] = loopArray.loopNum;
	EEPROMData[2] = loopArray.LoopArray[0];
	EEPROMData[3] = loopArray.LoopArray[1];
	EEPROMData[4] = loopArray.LoopArray[2];
	EEPROMData[5] = loopArray.LoopArray[3];
	EEPROMData[6] = loopArray.LoopArray[4];
	EEPROMData[7] = loopArray.LoopArray[5];
	
	EEPROMData[8] = crossmeet.reverse;
	EEPROMData[9] = elementArray.ElementArray[0];
	EEPROMData[10] = elementArray.ElementArray[1];
	EEPROMData[11] = elementArray.ElementArray[2];
	EEPROMData[12] = elementArray.ElementArray[3];
	EEPROMData[13] = elementArray.ElementArray[4];
	EEPROMData[14] = elementArray.ElementArray[5];
	
	EEPROMData[55] = sensor.advalue.ad_offset_val[EEEL];
	EEPROMData[56] = sensor.advalue.ad_offset_val[EEEM];
	EEPROMData[57] = sensor.advalue.ad_offset_val[EEER];
	EEPROMData[58] = sensor.advalue.ad_offset_val[EECL];
	EEPROMData[59] = sensor.advalue.ad_offset_val[EECR];
	EEPROMData[60] = sensor.advalue.ad_max_val[EEEL];
	EEPROMData[61] = sensor.advalue.ad_max_val[EEEM];
	EEPROMData[62] = sensor.advalue.ad_max_val[EEER];
	EEPROMData[63] = sensor.advalue.ad_max_val[EECL];
	EEPROMData[64] = sensor.advalue.ad_max_val[EECR];
	
	for (uint8 i = 64; i >= 60; i--)
	{
		EEPROMData[0] += EEPROMData[i];
	}
	DisableInterrupts;
	while (FLASH_EraseSector(FLASH_TEST_SECTOR));
	while (FLASH_WriteSector((uint32)FLASH_TEST_SECTOR, (uint8*)EEPROMData, (DATA_SIZE + 1) * 4, 0));
	EnableInterrupts;
}

void Setting_Paint(void)
{
	uint8 i = 0;
	oled_clear();
	switch (setting.page_num)
	{
	case 0:
		oled.Printf(8, 0, (unsigned char*)setting.string[0][0]);
		oled.Printf(115, 0, "P1");
		for(i = 1; i < 8; i++)
		{
			oled.Printf(8, i, (unsigned char*)setting.string[0][i]);
			if (setting.BitFlag == 1 && setting.course == i)
				oled.PrintReverseFloatValue(70, i, setting.data[0][i], setting.Bit);
			else
				oled.PrintFloatValue(70, i, setting.data[0][i]);
		}
		break;
	case 1:
		oled.Printf(8, 0, (unsigned char*)setting.string[1][0]);
		oled.Printf(115, 0, "P2");
		for(i = 1; i < 8; i++)
		{
			oled.Printf(8, i, (unsigned char*)setting.string[1][i]);
			if (setting.BitFlag == 1 && setting.course == i)
				oled.PrintReverseFloatValue(70, i, setting.data[1][i], setting.Bit);
			else
				oled.PrintFloatValue(70, i, setting.data[1][i]);
		}
		break;
	case 2:
		oled.Printf(8, 0, (unsigned char*)setting.string[2][0]);
		oled.Printf(115, 0, "P3");
		for(i = 1; i < 8; i++)
		{
			oled.Printf(8, i, (unsigned char*)setting.string[2][i]);
			if (setting.BitFlag == 1 && setting.course == i)
				oled.PrintReverseFloatValue(70, i, setting.data[2][i], setting.Bit);
			else
				oled.PrintFloatValue(70, i, setting.data[2][i]);
		}
		break;
	}
	if (setting.BitFlag == 0)
		oled.Putchar(0, setting.course, '*');
	else
		oled.Putchar(0, setting.course, ' ');
}

int8 Setting_Key_Scan(void)
{
	if(UP_IN == 0)
	{
		systick_delay_ms(100);
		if(UP_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 1;
		}
	}
	if(DOWN_IN == 0)
	{
			systick_delay_ms(100);
		if(DOWN_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 2;
		}
	}
	else if(LEFT_IN == 0)
	{
		systick_delay_ms(100);
		if(LEFT_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 3;
		}
	}
	else if(RIGHT_IN == 0)
	{
		systick_delay_ms(100);
		if(RIGHT_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 4;
		}
	}
	else if(OK_IN == 0)
	{
		systick_delay_ms(100);
		if(OK_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 5;
		}
	}
	else if(CANCEL_IN == 0)
	{
		systick_delay_ms(100);
		if(CANCEL_IN == 0)
		{
			bee.Set(BEE_HIGH);
			systick_delay_ms(100);
			bee.Set(BEE_LOW);
			return 6;
		}
	}
	return 0;
}

#define MAX_PAGE 2

void DispParameter(void)
{
	int8 key_state;
	setting.page_num = 0;
	setting.course = 1;
	setting.Bit = 0;
	setting.BitFlag = 0;
	Setting_Paint();
	while (1)
	{
		key_state = Setting_Key_Scan();
		if (setting.BitFlag == 0 && key_state == 6)
			break;
		if (key_state != 0)
		{
			switch (key_state)
			{
			case 1:
				// 上
				if (setting.BitFlag == 0)
				{
                    setting.course--;
					if (setting.course < 1)
						setting.course = 7;
				}
				else
					setting.data[setting.page_num][setting.course] += pow(10, setting.Bit);
				Setting_Paint();
				break;
			case 2:
				// 下
				if (setting.BitFlag == 0)
				{
					setting.course++;
					if (setting.course > 7)
						setting.course = 1;
				}
				else
					setting.data[setting.page_num][setting.course] -= pow(10, setting.Bit);
				Setting_Paint();
				break;
			case 3:
				// 左
				if (setting.BitFlag == 0)
				{
                    setting.page_num--;
					if (setting.page_num < 0)
						setting.page_num = MAX_PAGE;
				}
				else
					if (setting.Bit < 2)
						setting.Bit++;
				Setting_Paint();
				break;
			case 4:
				// 右
				if (setting.BitFlag == 0)
				{
                    setting.page_num++;
					if (setting.page_num > MAX_PAGE)
						setting.page_num = 0;
				}
				else
					if (setting.Bit > -3)
						setting.Bit--;
				Setting_Paint();
				break;
			case 5:
				// 进
				if (setting.BitFlag == 0)
				{
					setting.BitFlag = 1;
					setting.Bit = 0;
				}
				Setting_Paint();
				break;
			case 6:
				// 退
				if (setting.BitFlag == 0)
					setting.page_num = 0;
				else
					setting.BitFlag--;
				Setting_Paint();
				break;
			}
		}
	}
	Save_Data();
	oled_clear();
}

void DispEgg(void)
{
	UI_DispUIStrings(UI_Egg);
}

static void UI_DispUIStrings(uint8 strings[8][22])
{
	int i;
	
	for (i = 0; i < 8; i++)
	{
		if (ui.level == 0 || ui.level == 1 && ui.cursor[0] == 5)
		{
			if (i == ui.cursor[ui.level])
				strings[i][2] = CURSOR;
			else
				strings[i][2] = ' ';
		}
		
		oled.Printf(0, i, strings[i]);
	}
}
