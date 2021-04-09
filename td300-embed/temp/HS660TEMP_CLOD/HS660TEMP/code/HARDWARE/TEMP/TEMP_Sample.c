/**
  ******************************************************************************
  * @file    temp_sample.c
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   �ṩ�¶Ȳ�����صĹ���
  ******************************************************************************
  * @copy
  *
  */ 
#include "TEMP_Sample.h"
#include "usart.h"
	



//NTC���������ѯ��ʽ�����ڵ�
//���ڱ����¶�-��ֵ��ϵ
typedef struct{
	float temperature;
	float resistor;
	
}RT_node;


/*****************************************************************************
 �� �� ��  : wash_sampling_linearcalc
 ��������  : �������Թ�ϵ����������TRֵ�����Ŀ���¶�ֵ
 �������  : float R
 				float tem1
 				float res1
 				float tem2
 				float res2
 �������  : float T
 �� �� ֵ  : float
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��23��
    ��    ��   : Gao.J

    �޸�����   : �����ɺ���

*****************************************************************************/
float wash_sampling_linearcalc( float R, float tem1, float res1, float tem2, float res2 )
{
    float T = 0.0;
    
	//�������Թ�ϵ  (R - res1)/(T - tem1) = (res2 - res1)/(tem2 - tem1)	
    if(res2 != res1)
    {        
        T = (R - res1) * ((tem2 - tem1) / (res2 - res1)) + tem1;
    }
    else
    {
        T = tem1;
    }
	return T;
}



/*****************************************************************************
 �� �� ��  : wash_sampling_RtoTconvert
 ��������  : ��ֵ-�¶Ȳ��
 �������  : float res
 �������  : float* temperature

 �� �� ֵ  : void
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��23��
    ��    ��   : Gao.J
  2.��    ��   : 2015��5��20��
    ��    ��   : PengChuan

    �޸�����   : �޸���ֵ�¶Ȳ�ѯ����

*****************************************************************************/
//const RT_node RT_TABLE[] = {
//		{0 , 27.550},{1 , 26.37},{2 , 25.25},{3 , 24.19},{4 , 23.18},
//		{5 , 22.210},{6 , 21.29},{7 , 20.42},{8 , 19.58},{9 , 18.79},
//		{10, 18.030},{11, 17.30},{12, 16.61},{13, 15.95},{14, 15.33},
//		{15, 14.720},{16, 14.15},{17, 13.60},{18, 13.08},{19, 12.58},
//		{20, 12.100},{21, 11.64},{22, 11.20},{23, 10.78},{24, 10.38},
//		{25, 10.000},{26, 9.632},{27, 9.279},{28, 8.941},{29, 8.617},
//		{30,  8.307},{31, 8.010},{32, 7.724},{33, 7.451},{34, 7.188},
//		{35,  6.936},{36, 6.695},{37, 6.463},{38, 6.240},{39, 6.026},
//		{40,  5.820},{41, 5.623},{42, 5.433},{43, 5.251},{44, 5.076},
//		{45,  4.907},{46, 4.745},{47, 4.589},{48, 4.439},{49, 4.295},
//		{50,  4.156},{51, 4.023},{52, 3.895},{53, 3.772},{54, 3.653},
//		{55,  3.538},{56, 3.428},{57, 3.321},{58, 3.219},{59, 3.119},
//		{60,  3.024}};

const RT_node RT_TABLE[] = {
	{-30,113.6351},{-29,107.9956},{-28,102.6661},{-27,96.6273},{-26,92.8628},
		{-25,88.3665},{-24,84.0929},{-23,80.0578},{-22,76.2379},{-21,72.6209},
		{-20,69.1948},{-19,65.9481},{-18,62.8718},{-17,59.9546},{-16,57.1883},
		{-15,54.5649},{-14,52.0761},{-13,49.7128},{-12,47.4708},{-11,45.3410},
		{-10, 43.3184},{-9, 41.3957},{-8, 39.5699},{-7, 37.8338},{-6, 36.1825},
		{-5, 34.7320}, {-4, 33.1194},{-3, 31.6975},{-2, 30.3450},{-1, 29.0578},
		{0, 27.7660},  {1, 26.5928}, {2, 25.4829}, {3, 24.4249}, {4, 23.4157},
		{5, 22.3460},  {6, 21.4306}, {7, 20.5600}, {8, 19.7289}, {9, 18.9363},
		{10, 18.0944}, {11, 17.3741},{12, 16.6870},{13, 16.0312},{14, 15.4046},
		{15, 14.7484}, {16, 14.1772},{17, 13.6323},{18, 13.1104},{19, 12.6117},
		{20, 12.0956}, {21, 11.6404},{22, 11.2043},{23, 10.7861},{24, 10.3869},
		{25, 10.000},{26, 9.6609},{27, 9.3089},{28, 8.9709},{29, 8.6470},
		{30, 8.2746}, {31, 7.9788},{32, 7.6951},{33, 7.4234},{34, 7.1626},
		{35, 6.8984},{36, 6.6577},{37, 6.4272},{38, 6.2057},{39, 5.9931},{40, 5.7810},
		{41, 5.5842},{42, 5.3958},{43, 5.2153},{44, 5.0407},{45,4.8680},
		{46, 4.7071},{47, 4.5525},{48, 4.4039},{49, 4.2603},{50,401218},
		{51, 3.9887},{52, 3.8609},{53, 3.7371},{54, 3.6193},{55,3.5048},
		{56, 3.3945},{57, 3.2886},{58, 3.1865},{59, 3.0875},{60,2.9926},
		{61, 2.9012},{62, 2.8130},{63, 2.7269},{64, 2.6447},{65,2.5658},
		{66, 2.4892},{67, 2.4149},{68, 2.3435},{69, 2.2743},{70,2.2082},
		{71, 2.1436},{72, 2.0812},{73, 2.0208},{74, 1.9633},{75,1.9074},
		{76, 1.8527},{77, 1.8002},{78, 1.7497},{79, 1.7001},{80,1.6526},
		{81, 1.6070},{82, 1.5624},{83, 1.5198},{84, 1.4782},{85,1.4513},
		{86, 1.3989},{87, 1.3613},{88, 1.3246},{89, 1.2890},{90,1.2553},
};

void wash_sampling_RtoTconvert( float Res, float* temperature)
{
	

 
    uint16_t usloopvar  = 0;
    uint16_t usarraylen = 0;
    
    /* �������鳤��*/
    usarraylen = (uint16_t)sizeof(RT_TABLE);

		if (RT_TABLE[0].resistor <= Res)
		{//С��������ϴ�¿����账������С�¶�
	    *temperature = RT_TABLE[0].temperature;
//	    printf("�¶ȵ��������");
	    return;
		}

    if (RT_TABLE[usarraylen - 1].resistor >= Res)
    {//����������ϴ�¿����账��������¶�
            *temperature = RT_TABLE[usarraylen - 1].temperature;
//						printf("�¶ȳ�����40��");
            return;
	}
    
    for (usloopvar = 1; usloopvar < usarraylen; usloopvar++)
		{
			if(Res >= RT_TABLE[usloopvar].resistor)
            {
						break;
		    }	
        }	
	
	//�쳣����
	  if (usloopvar >= usarraylen)
		{
	    *temperature = RT_TABLE[usarraylen - 1].temperature;
	    return;
	    }

		/* �������Թ�ϵ����õ��¶�ֵ */
		*temperature = wash_sampling_linearcalc(Res, RT_TABLE[usloopvar].temperature, 
	                                               RT_TABLE[usloopvar].resistor,
	                                               RT_TABLE[usloopvar - 1].temperature,
									                               RT_TABLE[usloopvar - 1].resistor);
			return;
}


//�����ѹֵ
float  ADValueToVoltage(uint16_t adValue)
{
 float  ReferenceVoltage =3.3;//��׼��ѹ
 float  FullADValue =4095.0;//�����̵�ADֵ
	
	
	return (ReferenceVoltage/FullADValue)*(float)adValue;

}

void PrintFloat(float value)
{
    int tmp,tmp1,tmp2;
    tmp = (int)value;
    tmp1=(int)((value-tmp)*10)%10;
    tmp2=(int)((value-tmp)*100)%10;
    printf("value=%d.%d%d\r\n",tmp,tmp1,tmp2);
}
/*****************************************************************************
 �� �� ��  : wash_sampling_adtotemp
 ��������  : ����ADֵ��ȡ��ϴˮ�¶�
 �������  : uint32_t adval 
 �������  : ��
 �� �� ֵ  : float
 
 �޸���ʷ      :
  1.��    ��   : 2014��5��26��
    ��    ��   : Gao.J
  2.��    ��   : 2015��5��20��
    ��    ��   : PengChuan
    �޸�����   : �޸��¶Ȳ�ѯ����

*****************************************************************************/
float wash_sampling_adtotemp(uint16_t adval)
{
	 char *s;
    float Uad = 0.0;
    float dstval = 0.0;
    float dsttemp = 0.0;
	
    Uad = ADValueToVoltage(adval);//adtovoltage(adval);
//    printf("��ǰ�ĵ�ѹֵ��\n");
//     PrintFloat(Uad);
    dstval = voltagetores(Uad);
//  	printf(" ��õ���ֵ�� \n");
//	PrintFloat(dstval);
    //dsttempĿ���¶�ֵ�����ò�ѯ���ó�
    wash_sampling_RtoTconvert(dstval, &dsttemp);
    return dsttemp;
}
