// ===============================================================================
// Copyright (C)  All Rights Reserved
// ģ������: ������������������ö���
// ģ��汾: V1.00
// ������Ա: zhanjw
// ����ʱ��: 2018-09-01
// ===============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ===============================================================================

#ifndef __PARA_H__
#define __PARA_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// �궨��
//========================================================
// ��������
#define CN_SAM_PTR          (0)                                 // �������ݴ洢��ʼָ��
#define CN_SAM_DIF          (2)                                 // ��ֵ���
#define CN_ANA_SAM_MAG      (500)                               // ģ�����������ݷŴ���(�Ŵ�500�� ��λ:����ͨ��2mA)
#define CN_FFT_COE_MAG      (16)                                // ������ϵ����ķŴ���(2��n���� 2^16)

#define CN_SAM_RATE         (20)                                // ÿ�ܲ���������
#define CN_SAM_RATIO_NUM    (3)                                 // ����������Ŀ(����1��2��4�ֱ��Ӧ����Ƶ��20��40��80)
//#define CN_SAM_DOT_MAX    (80)                                // ÿ�ܲ�����������
#define CN_SAM_DOT_MAX      (CN_SAM_RATE*(1<<CN_SAM_RATIO_NUM)) // ÿ�ܲ�����������
#define CN_SAM_CHAN_MAX     (16)                                // ������ͨ����
#define CN_ANA_HARM_MAX     (13)                                // ���г������(��������)
#define CN_ANA_HARM_PARA    (CN_ANA_HARM_MAX+1)                 // ����ģ����г������(����ֱ������)

// ���ó�������
#define CN_SQRT_2           (1.4142135623731)                   // sqrt(2)
#define CN_EVO3_2           (0.8660254037844)                   // sqrt(3)*0.5
#define CN_RECIP_3          (0.3333333333333)                   // 3�ĵ���
#define CN_PI               (3.1415926535898)                   // pi
#define CN_Rad2AngCoe       (180/CN_PI)                         // ����ת��Ϊ�Ƕȵ�ϵ��
#define CN_Ang2RadCoe       (CN_PI/180)                         // �Ƕ�ת��Ϊ���ȵ�ϵ��
#define CN_Rad2Ang( Rad )   ( (FLOAT32)(Rad)*CN_Rad2AngCoe )    // ����ת��Ϊ�Ƕ�
#define CN_Ang2Rad( Ang )   ( (FLOAT32)(Ang)*CN_Ang2RadCoe )    // �Ƕ�ת��Ϊ����

#ifdef __cplusplus
}
#endif

#endif // __PARA_H__

