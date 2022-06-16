// ===============================================================================
// Copyright (C)  All Rights Reserved
// ģ������: ���������弰����ͨ��ģ���������
// ģ��汾: V1.00
// ������Ա: zhanjw
// ����ʱ��: 2018-09-01
// ===============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ===============================================================================

#include "Data.h"

//========================================================
// ϵ����
//========================================================
// FFT��������ϵ��
// [1ά]:����Ƶ��N:20��40��80
// [2ά]:г������H:1~13
// [3ά]:�������D:0~N-1
// ����ϵ�� sin(H*D*(2pi/N)) �ٷŴ�2^12�� 16λ�������洢
SINT16  g_s16FFTSinCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];
SINT16  g_s16FFTCosCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];

//========================================================
// ����������
//========================================================
// ����ͨ��ģ�����������
// [1ά]:����Ƶ��N: 20��40��80
// [2ά]:ģ����ͨ��C:0~15
// [3ά]:�������D:0~N-1(˫������)
// �Ŵ�500��(��λ:����ͨ��2mA) 16λ������ ˫������
SINT16  g_s16SamData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];

// ����ͨ�����������
SINT16  g_s16DifData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];

//========================================================
// FFT��������
//========================================================
// ģ��������FFT���
// [1ά]:����Ƶ��N  : 20��40��80
// [2ά]:ģ����ͨ��C: 0~15
tagFFTData  g_tFFTData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX];

//========================================================
// �������ݱ�
//========================================================
// ����������ñ��� (TAN@)*500 (��λ:0.5�� �Ŵ�500��)
const UINT16 g_u16Tan_tab[180]=
{
//  0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.6,7.0,7.5,8.0,8.5,9.0,9.5,10.0,
    5,  9,  13, 17, 22, 26, 31, 35, 39, 44, 48, 53, 57, 61, 66, 70, 75, 79, 84, 88,
//  10.5, 11.5,  12.5 13.5 14.5 15.5 16.5, 17.5   18.5  , 19.5
    93, 97, 102,106, 111,115,120,125,129,134,139,143,148,153,158,162,167,172,177,182,
//  20.5, 21.5   22.5 23.5 24.5   25.5 26.5 27.5   28.5 29.5
    187,192,197,202,207,212,217,223,228,233,238,244,249,255,260,266,271,277,283,289,
//  30.5 31.5 32.5   33.5 34.5 35.5   36.5 37.5 38.5   39.5
    295,300,306,312,319,325,331,337,344,350,357,363,370,377,384,391,398,405,412,420,
//  40.5 41.5   42.5 43.5 44.5   45.5 46.5 47.5   48.5 49.5
    427,435,442,450,458,466,474,483,491,500,509,518,527,536,546,555,565,575,585,596,
//  50.5 51.5 52.5 53.5 54.5   55.5 56.5 57.5   58.5 59.5
    607,617,629,640,652,664,676,688,701,714,728,741,755,770,785,800,816,832,849,866,
//  60.5 61.5   62.5 63.5   64.5 65.5 66.5   67.5 68.5 69.5
    884,902,921,940,960,981,1003,1025,1048,1072,1097,1123,1150,1178,1207,1238,1269,1303,1337,1374,
//  70.5 71.5   72.5 73.5 74.5   75.5 76.5 77.5   78.5 79.5
    1412,1452,1494,1539,1586,1635,1688,1744,1803,1866,1933,2005,2083,2166,2255,2352,2458,2572,2698,2836,
//  80.5 81.5   82.5 83.5 84.5   85.5 86.5 87.5 88.5 89.5, 89.56
    2988,3157,3346,3558,3798,4072,4388,4757,5193,5715,6353,7150,8175,9541,11452,14318,19094,28645,57294,65535
};

//========================================================
// �������ģ������������
//========================================================
// ͨ��1 �������ģ������������
tagAnaPara  g_tAna01[CN_ANA_HARM_PARA] =
{
//  г������    ��Чֵ  ��ʼ���
    {  0 ,      0.0,     CN_Ang2Rad(0.00)   },      // ֱ��
    {  1 ,      5.0,     CN_Ang2Rad(120 )   },      // ����
    {  2 ,      0.0,     CN_Ang2Rad(0.00)   },      // 2 ��г��
    {  3 ,      0.0,     CN_Ang2Rad(0.00)   },      // 3 ��г��
    {  4 ,      0.0,     CN_Ang2Rad(0.00)   },      // 4 ��г��
    {  5 ,      0.0,     CN_Ang2Rad(0.00)   },      // 5 ��г��
    {  6 ,      0.0,     CN_Ang2Rad(0.00)   },      // 6 ��г��
    {  7 ,      0.0,     CN_Ang2Rad(0.00)   },      // 7 ��г��
    {  8 ,      0.0,     CN_Ang2Rad(0.00)   },      // 8 ��г��
    {  9 ,      0.0,     CN_Ang2Rad(0.00)   },      // 9 ��г��
    {  10,      0.0,     CN_Ang2Rad(0.00)   },      // 10��г��
    {  11,      0.0,     CN_Ang2Rad(0.00)   },      // 11��г��
    {  12,      0.0,     CN_Ang2Rad(0.00)   },      // 12��г��
    {  13,      0.0,     CN_Ang2Rad(0.00)   },      // 13��г��
};

// ͨ��2 �������ģ������������
tagAnaPara  g_tAna02[CN_ANA_HARM_PARA] =
{
//  г������    ��Чֵ  ��ʼ���
    {  0 ,      1.0,     CN_Ang2Rad(0.00)   },      // ֱ��
    {  1 ,      2.0,     CN_Ang2Rad(0   )   },      // ����
    {  2 ,      0.2 ,    CN_Ang2Rad(10  )   },      // 2 ��г��
    {  3 ,      0.3 ,    CN_Ang2Rad(20  )   },      // 3 ��г��
    {  4 ,      0.4 ,    CN_Ang2Rad(30  )   },      // 4 ��г��
    {  5 ,      0.5 ,    CN_Ang2Rad(40  )   },      // 5 ��г��
    {  6 ,      0.6 ,    CN_Ang2Rad(50  )   },      // 6 ��г��
    {  7 ,      0.7 ,    CN_Ang2Rad(60  )   },      // 7 ��г��
    {  8 ,      0.8 ,    CN_Ang2Rad(70  )   },      // 8 ��г��
    {  9 ,      0.9 ,    CN_Ang2Rad(80  )   },      // 9 ��г��
    {  10,      0.10,    CN_Ang2Rad(90  )   },      // 10��г��
    {  11,      0.11,    CN_Ang2Rad(100 )   },      // 11��г��
    {  12,      0.12,    CN_Ang2Rad(110 )   },      // 12��г��
    {  13,      0.13,    CN_Ang2Rad(120 )   },      // 13��г��
};

// ͨ��3 �������ģ������������
tagAnaPara  g_tAna03[CN_ANA_HARM_PARA] =
{
//  г������    ��Чֵ  ��ʼ���
    {  0 ,      0.0,     CN_Ang2Rad(0.00)   },      // ֱ��
    {  1 ,      36.0,    CN_Ang2Rad(240 )   },      // ����
    {  2 ,      0.0,     CN_Ang2Rad(0.00)   },      // 2 ��г��
    {  3 ,      0.0,     CN_Ang2Rad(0.00)   },      // 3 ��г��
    {  4 ,      0.0,     CN_Ang2Rad(0.00)   },      // 4 ��г��
    {  5 ,      0.0,     CN_Ang2Rad(0.00)   },      // 5 ��г��
    {  6 ,      0.0,     CN_Ang2Rad(0.00)   },      // 6 ��г��
    {  7 ,      0.0,     CN_Ang2Rad(0.00)   },      // 7 ��г��
    {  8 ,      0.0,     CN_Ang2Rad(0.00)   },      // 8 ��г��
    {  9 ,      0.0,     CN_Ang2Rad(0.00)   },      // 9 ��г��
    {  10,      0.0,     CN_Ang2Rad(0.00)   },      // 10��г��
    {  11,      0.0,     CN_Ang2Rad(0.00)   },      // 11��г��
    {  12,      0.0,     CN_Ang2Rad(0.00)   },      // 12��г��
    {  13,      0.0,     CN_Ang2Rad(0.00)   },      // 13��г��
};

// �������ģ������������ָ������
tagAnaPara *g_ptAnaPara[CN_SAM_CHAN_MAX] =
{
    g_tAna01,   // 1  ͨ��
    g_tAna02,   // 2  ͨ��
    g_tAna03,   // 3  ͨ��
    NULL,       // 4  ͨ��
    NULL,       // 5  ͨ��
    NULL,       // 6  ͨ��
    NULL,       // 7  ͨ��
    NULL,       // 8  ͨ��
    NULL,       // 9  ͨ��
    NULL,       // 10 ͨ��
    NULL,       // 11 ͨ��
    NULL,       // 12 ͨ��
    NULL,       // 13 ͨ��
    NULL,       // 14 ͨ��
    NULL,       // 15 ͨ��
    NULL,       // 16 ͨ��
};

//========================================================
// ģ�����������
//========================================================

// ģ�����������
FLOAT32 Ana_Simulate( FLOAT32 fRad, tagAnaPara  *ptAnaPa )
{
    UINT32      i;
    FLOAT32     fVal;

    fVal = 0;
    for( i=0;i<CN_ANA_HARM_PARA;i++)    // г������H��ѭ��
    {
        if( ptAnaPa[i].dwHarm==0 )      // ֱ������
        {
            fVal += ptAnaPa[i].fAm;
        }
        else                            // ������г������
        {
            // I*sqrt(2)*sin(��*H+��) ��:�����Ƕ� H:г��ϵ�� ��:��ʼ��
            fVal += ptAnaPa[i].fAm*CN_SQRT_2*sin( ptAnaPa[i].dwHarm*fRad + ptAnaPa[i].fAng );
        }
    }
    return(fVal);
}

// �������ͨ��ģ���������
void Cal_Simulate_Sam_Data( void )
{
    UINT32      dwRatio, dwChanNO;
    UINT32      dwSamNO, dwSamPtr;
    UINT32      dwSamRate;
    SINT16      *ps16Sam, s16Sam;
    FLOAT32     fVal, fRad;
    tagAnaPara *ptAnaPara;

    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)           // 1. ����Ƶ��N��ѭ��
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);                       // ����Ƶ��(20��40��80)
        for(dwChanNO=0; dwChanNO < CN_SAM_CHAN_MAX; dwChanNO++)     // 2. ����ͨ��C��ѭ��
        {
            ptAnaPara = g_ptAnaPara[dwChanNO];                      // ��ǰͨ���ķ������ģ������������
            if( ptAnaPara==NULL )
            {
                continue;
            }
            ps16Sam = &(g_s16SamData[dwRatio][dwChanNO][0]);        // ����ͨ����ģ����������׵�ַ
            for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)          // 3. ��������N��ѭ��
            {
                fRad = 2*CN_PI*dwSamNO/dwSamRate;                   // ��ǰ�������Ӧ�ĽǶ� ��=n*(2pi/N)
                fVal = Ana_Simulate( fRad, ptAnaPara );             // ��ǰ�������ģ���������
                fVal*= CN_ANA_SAM_MAG;                              // �Ŵ�500�� ��λ:����ͨ��2mA
                s16Sam = (SINT16)(fVal+0.5);                        // 16λ������
                dwSamPtr = (dwSamNO+CN_SAM_PTR)%dwSamRate;          // ��ǰ������Ĵ洢ָ��
                ps16Sam[dwSamPtr]           = (SINT16)fVal;         // ˫������(��һ��)
                ps16Sam[dwSamPtr+dwSamRate] = (SINT16)fVal;         // ˫������(�ڶ���)
            }
        }
    }
}
