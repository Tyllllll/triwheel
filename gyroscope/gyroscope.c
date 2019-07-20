#include "includes.h"


float Acc_X,Acc_Y,Acc_Z,Gyro_X,Gyro_Y,Gyro_Z, angle_speed_Compensation;
int read_buff[6];
float angle_speed[4]={0};
SPI_CLASS spi=
{
	&Icm20602_init_spi,
	&Get_Attitude
		
};
GYRO_CLASS gyro;

//float temp;
void Get_Attitude()
{
	//  static float s=0;
	static float faiZ,faiY;
	float ratio_gyro=0.048;//½«ÍÓÂÝÒÇµÄÖµ×ª»¯Îª ¶ÈÃ¿Ãë µÄÏµÊý
	//float ratio_acc =90.0/8192;//½«¼ÓËÙ¶ÈµÄÖµ¹éÒ»»¯Îª-90µ½90£¨½üËÆ½Ç¶È£© µÄÏµÊý
	
//	Acc_X =   (float)(Get_X_Acc());//LPButterworth((float)(Get_X_Acc()),&Butter_Buffer_ACC_X,&Butter_30HZ_Parameter_Acce);
//	Acc_Y = -(float)(Get_Y_Acc());//LPButterworth((float)(Get_Y_Acc()),&Butter_Buffer_ACC_Y,&Butter_30HZ_Parameter_Acce);
//	Acc_Z = -(float)(Get_Z_Acc());//LPButterworth((float)(Get_Z_Acc()),&Butter_Buffer_ACC_Z,&Butter_10HZ_Parameter_Acce);//  
        
	LPF_1_db(10,0.001,(float)(Get_Y_Acc()),&Acc_Y);
	LPF_1_db(25,0.001,-(float)(Get_Z_Acc()),&Acc_Z);
	Gyro_X= -(Get_X_Gyro() + 21.9) ;
	LPF_1_db(30,0.001,-(float)(Get_Y_Gyro()+25),&Gyro_Y);
	
	faiZ= Acc_Z/16384.0f;
	if (faiZ > 1) faiZ = 1;
	if (faiZ < -1) faiZ = -1;
	angle_speed_Compensation = 1456.8*(speed.speedErr[0] - speed.speedErr[1]);
	if(angle_speed_Compensation>5) 
		angle_speed_Compensation=5;
	if(angle_speed_Compensation<-5) 
		angle_speed_Compensation=-5;
	gyro.ACC_Angle = asin(faiZ)/0.01745329 ;
	
	//   faiY= Acc_Y/16384.0f;
	//    if (faiY > 1) faiY = 1;
	//     if (faiY < -1) faiY = -1;
	//  gyro.Gravity_Angle = Acc_Y*0.01 ;
	
	gyro.Angle_Speed=-Gyro_Y*0.03051757*1.56;
	
	Kalman_Filter(gyro.ACC_Angle,-Gyro_Y*0.03051757);            //µ÷ÓÃ¿¨¶ûÂüÂË²¨º¯Êý
	
	gyro.Car_Angle = gyro.CarY_Angle_Uncompensation + 4.172;//+angle_speed_Compensation/4.0;
	//ËÙ¶ÈÑaƒ”
	// for(uint8 i = 19;i>0;i--)
	//   gyro.carAngleErr[i] =  gyro.carAngleErr[i-1];
	// gyro.carAngleErr[0] = gyro.Car_Angle;
	
	gyro.Turn_Speed= Gyro_X*ratio_gyro/1.56; 
	
	gyro.CarY_Angle = gyro.Gravity_Angle;
	
	if(//»·½Ç¶È»ý·Ö  
	   ( Findline.Process == Right_Roundabout_IN || Findline.Process == Right_Roundabout_OUT
		||  Findline.Process == Left_Roundabout_IN || Findline.Process == Left_Roundabout_OUT  )
		   ||  (Findline.Process ==  Crossmeeting && crossmeet.PostureIntegral == 1)
			   ||  (Findline.Process ==  Transome && transome.PostureIntegral == 1 )//»á³µ½Ç¶È»ý·Ö
//				||1
                             )
	{
		gyro.TurnAngle_Integral += gyro.Turn_Speed/1000.0 ;
	}
	else
	{
		gyro.TurnAngle_Integral = 0;
	}
}

//float Q_angle=0.50, Q_gyro=0.070, R_angle=4;
//float Q_angle=0.2, Q_gyro=0.1, R_angle=0.3;
//float Q_angle=0.02, Q_gyro=0.01, R_angle=0.03;
//float Q_angle=0.01, Q_gyro=0.003, R_angle=10;
//float Q_angle=0.001, Q_gyro=0.1, R_angle=10;
//float Q_angle=0.01, Q_gyro=0.1, R_angle=10;
static  float Q_angle=0.001, Q_gyro=0.003, R_angle=300, dt=0.001;
	//QÔö´ó£¬¶¯Ì¬ÏìÓ¦Ôö´ó
static float Pk[2][2] = { {1, 0}, {0, 1 }};
	
static float Pdot[4] ={0,0,0,0};

static float q_bias=0, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;



//-------------------------------------------------------
void Kalman_Filter(float angle_m,float gyro_m)			
{
	gyro.CarY_Angle_Uncompensation+=(gyro_m-q_bias) * dt; ///Ô¤²âÖµ
	Pdot[0]=Q_angle - Pk[0][1] - Pk[1][0];
	Pdot[1]=- Pk[1][1];
	Pdot[2]=- Pk[1][1];
	Pdot[3]=Q_gyro;
	
	Pk[0][0] += Pdot[0] * dt;
	Pk[0][1] += Pdot[1] * dt;
	Pk[1][0] += Pdot[2] * dt;
	Pk[1][1] += Pdot[3] * dt;
	
	angle_err = angle_m -gyro.CarY_Angle_Uncompensation;///²âÁ¿Öµ-Ô¤²âÖµ
	
	PCt_0 =  Pk[0][0];
	PCt_1 =  Pk[1][0];
	
	E = R_angle + PCt_0;
	
	K_0 = PCt_0 / E; ///¿¨¶ûÂüÔöÒæ
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = Pk[0][1];

	Pk[0][0] -= K_0 * t_0;
	Pk[0][1] -= K_0 * t_1;
	Pk[1][0] -= K_1 * t_0;
	Pk[1][1] -= K_1 * t_1;
	
	gyro.CarY_Angle_Uncompensation+= K_0 * angle_err; ///×îÓÅ½Ç¶È=Ô¤²âÖµ+¿¨¶ûÂüÔöÒæ*(²âÁ¿Öµ-Ô¤²âÖµ)
	q_bias	+= K_1 * angle_err;
	gyro.Angle_Speed = gyro_m-q_bias;
}

void Icm20602_init_spi(void)
{
	uint8 val = 0x0;
	spi_init(spi0, SPI_PCS0, MASTER,10*1000*1000); //Ó²¼þSPI³õÊ¼»¯
	systick_delay_ms(10);
	//¼ì²â
	icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_1,0x80);
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_1,0x01);
	systick_delay_ms(10);
//	do
//	{
//		icm_spi_r_reg_byte(0x75, &val);
//		systick_delay_ms(10);
//	}while(0x12 != val); 
	
	/*¸´Î»reg*/
	icm_spi_w_reg_byte(MPU_RA_SIGNAL_PATH_RESET,0x03);
	systick_delay_ms(10);
	/*¸´Î»reg*/
	icm_spi_w_reg_byte(MPU_RA_USER_CTRL,0x01);	
	systick_delay_ms(10);
	icm_spi_w_reg_byte(0x70,0x40);//dmp 
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_PWR_MGMT_2,0x00);
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_SMPLRT_DIV,0);
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_CONFIG,ICM20602_LPF_20HZ);
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_GYRO_CONFIG,(2 << 3));
	systick_delay_ms(10);
	icm_spi_w_reg_byte(MPU_RA_ACCEL_CONFIG,( 0<< 3));
	systick_delay_ms(10);
	/*¼ÓËÙ¶È¼ÆLPF 20HZ*/
	icm_spi_w_reg_byte(0X1D,0x04);
	systick_delay_ms(10);
	/*¹Ø±ÕµÍ¹¦ºÄ*/
	icm_spi_w_reg_byte(0X1E,0x00);
	systick_delay_ms(10);
	/*¹Ø±ÕFIFO*/
	icm_spi_w_reg_byte(0X23,0x00);
	systick_delay_ms(10);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPIå†™å¯„å­˜å™¨
//  @param      cmd     å¯„å­˜å™¨åœ°å€
//  @param      val     éœ€è¦å†™å…¥çš„æ•°æ®
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_w_reg_byte(uint8 cmd, uint8 val)
{
    cmd |= ICM20602_SPI_W;
    spi_mosi_cmd(spi0,SPI_PCS0,&cmd,NULL,&val,NULL,1,1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPIè¯»å¯„å­˜å™¨
//  @param      cmd     å¯„å­˜å™¨åœ°å€
//  @param      *val    æŽ¥æ”¶æ•°æ®çš„åœ°å€
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_r_reg_byte(uint8 cmd, uint8 *val)
{
    cmd |= ICM20602_SPI_R;
    spi_mosi_cmd(spi0,SPI_PCS0,&cmd,NULL,val,val,1,1);
}

/******************»ñµÃXÖá¼ÓËÙ¶È******************/
int16 Get_X_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_XOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_XOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
/******************»ñµÃYÖá¼ÓËÙ¶È******************/
int16 Get_Y_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_YOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_YOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
/******************»ñµÃZÖá¼ÓËÙ¶È******************/
int16 Get_Z_Acc()
{
    int16 temp;
    uint8 dat[2];
    icm_spi_r_reg_byte(ICM20602_ACCEL_ZOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_ACCEL_ZOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
/*****************»ñµÃxÖá½ÇËÙ¶È*****************/
int16 Get_X_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_XOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_GYRO_XOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
/*****************»ñµÃyÖá½ÇËÙ¶È*****************/
int16 Get_Y_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_YOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_GYRO_YOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
/*****************»ñµÃzÖá½ÇËÙ¶È*****************/
int16 Get_Z_Gyro()
{
    uint8 dat[2];
    int16 temp;
    icm_spi_r_reg_byte(ICM20602_GYRO_ZOUT_H,dat+0);
    icm_spi_r_reg_byte(ICM20602_GYRO_ZOUT_L,dat+1);
    temp = (int16)(((uint16)dat[0]<<8 | dat[1]));
    return temp;
}
