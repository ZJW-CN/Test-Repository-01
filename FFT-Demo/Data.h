// ===============================================================================
// Copyright (C)  All Rights Reserved
// ģ������: �������Ͷ���
// ģ��汾: V1.00
// ������Ա: ռ����
// ����ʱ��: 2018-09-01
// ===============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ===============================================================================

#ifndef __DATA_H__
#define __DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// ͷ�ļ�����
//========================================================
#include "Types.h"
#include "Para.h"

//========================================================
// ���ݽṹ�嶨��
//========================================================
// ģ���������ṹ��
typedef struct
{
    UINT32      dwHarm;     // г������
    FLOAT32     fAm;        // ��ֵ
    FLOAT32     fAng;       // ��ʼ���
}tagAnaPara;

// ģ���������ṹ��
typedef struct
{
    // ������ �Ŵ�500��(��λ:����ͨ��2mA)
    SINT32      s32Re;      // ʵ��
    SINT32      s32Im;      // �鲿
    SINT32      s32Amp;     // ��ֵ(ƽ��)
    SINT32      s32Ang;     // �Ƕ�(��λ:0.5��)
    // ������
    FLOAT32     fVal;       // ��Чֵ(��ƽ��)
    FLOAT32     fAng;       // �Ƕ�(��λ:1.0��)
}tagAnaVec;

// ģ���������ṹ��
typedef struct
{
    SINT32      s32DC;      // ֱ������
    UINT32      u32RMS;     // ������
    tagAnaVec   tAnaVec[CN_ANA_HARM_MAX];   // ȫ��FFT���� [1ά]:г������H:1~13
    tagAnaVec   tAnaVecDif;                 // �벨FFT����
}tagFFTData;

//========================================================
// ��������
//========================================================
// FFT������ϵ��
extern SINT16  g_s16FFTSinCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];
// FFT������ϵ��
extern SINT16  g_s16FFTCosCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];

// ����ͨ��ԭʼ������
extern SINT16  g_s16SamData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];
// ����ͨ�����������
extern SINT16  g_s16DifData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];

// ģ��������FFT���
extern tagFFTData  g_tFFTData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX];

// �������ģ������������ָ������
extern tagAnaPara *g_ptAnaPara[CN_SAM_CHAN_MAX];

// �������ݱ�
extern const UINT16 g_u16Tan_tab[180];

//========================================================
// ��������
//========================================================
extern void Cal_Simulate_Sam_Data( void );

#ifdef __cplusplus
}
#endif

#endif // __DATA_H__

