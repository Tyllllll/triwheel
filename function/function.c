#include "includes.h"
#include "function.h"


/***************************************************************
	*	@brief	�޷�
	*	@param	float data		��ǰ����
                float max_out	������
                float min_out	��С���
	*	@note	��
***************************************************************/
float Constrain_32(float *data, float max_out,float min_out)
{
  *data = (*data)>max_out ? max_out:*data;
  *data = (*data)<min_out ? min_out:*data;
  return *data;
}





/***************************************************************
	*	@brief	�޷�
	*	@param	int16 data		��ǰ����
                int16 max_out	������
                int16 min_out	��С���
	*	@note	��
***************************************************************/
int16 Constrain_16(int16 *data, int16 max_out,int16 min_out)
{
  *data = (*data)>max_out ? max_out:*data;
  *data = (*data)<min_out ? min_out:*data;
  return *data;
}   




/***************************************************************
	*	@brief	����������
	*	@param	uint8 x
	*	@note	��
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

