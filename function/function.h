#include "includes.h"

float Constrain_32(float *data, float max_out,float min_out);
int16 Constrain_16(int16 *data, int16 max_out,int16 min_out);
uint8 math_sqrt(uint8 x);
float Midline_Std_Deviation(uint8 row_max, uint8 row_min);
uint8 lose_Leftline(int16 row_beg, int16 row_end);
uint8 lose_Rightline(int16 row_beg, int16 row_end);