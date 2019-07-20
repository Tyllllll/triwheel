#include "includes.h"
#include "function.h"


/***************************************************************
	*	@brief	限幅
	*	@param	float data		当前数据
                float max_out	最大输出
                float min_out	最小输出
	*	@note	无
***************************************************************/
float Constrain_32(float *data, float max_out,float min_out)
{
  *data = (*data)>max_out ? max_out:*data;
  *data = (*data)<min_out ? min_out:*data;
  return *data;
}





/***************************************************************
	*	@brief	限幅
	*	@param	int16 data		当前数据
                int16 max_out	最大输出
                int16 min_out	最小输出
	*	@note	无
***************************************************************/
int16 Constrain_16(int16 *data, int16 max_out,int16 min_out)
{
  *data = (*data)>max_out ? max_out:*data;
  *data = (*data)<min_out ? min_out:*data;
  return *data;
}   




/***************************************************************
	*	@brief	开根号运算
	*	@param	uint8 x
	*	@note	无
***************************************************************/
uint8 math_sqrt(uint8 x)
{
    uint8 ans=0,p=0x80;
    while(p!=0)
    {
        ans+=p;
        if(ans*ans>x)
        {
            ans-=p;
        }
        p=(uint8)(p/2);
    }
    return ans;
}

