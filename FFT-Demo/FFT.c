// ===============================================================================
// Copyright (C)  All Rights Reserved
// ģ������: FFT����
// ģ��汾: V1.00
// ������Ա: zhanjw
// ����ʱ��: 2018-09-01
// ===============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ===============================================================================

#include "Types.h"
#include "Para.h"
#include "Data.h"

// ============================================================================
// �������ܣ�����ȫ��FFT������ϵ��
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_FFT_SinCosCoe( void )
{
    UINT32  dwRatio, dwHarm, dwSamNO;   // ��ʱ����
    UINT32  dwSamRate;                  // ����Ƶ��
    UINT32  dwHarm0;
    SINT16  *pS16SinCoe, *pS16CosCoe;   // ������ϵ���׵�ַ
    UINT16  u16Coe;                     // ������ϵ���ķŴ���
    FLOAT32 fPiN, fCoe;
    
    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)           // 1. ����Ƶ��N��ѭ��
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);   // ����Ƶ��(20��40��80)
        //u16Coe    = 1<<(CN_FFT_COE_MAG-dwRatio);// ������ϵ���ķŴ���(����Ƶ��20��40��80��Ӧ�ķŴ���Ϊ:2^12��2^11��2^10)
        u16Coe    = 1<<CN_FFT_COE_MAG;          // ������ϵ���ķŴ���(2^16)
        for(dwHarm=0; dwHarm < CN_ANA_HARM_MAX; dwHarm++)        // 2. г������H��ѭ��
        {
            dwHarm0 = dwHarm + 1;
            for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)          // 3. ��������N��ѭ��
            {
                pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][dwHarm][0]);// ����ϵ���׵�ַ
                pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][dwHarm][0]);// ����ϵ���׵�ַ

                fPiN = 2.0*CN_PI/dwSamRate;     // 2pi/N
                // ����ϵ��
                fCoe = (2.0*sin(dwHarm0*dwSamNO*fPiN)/N)*u16Coe;// 2sin(H*D*(2pi/N))/N �ٷŴ�2^16��
                fCoe+= (fCoe>=0)? 0.5 : -0.5 ;                  // ��������
                pS16SinCoe[dwSamNO] = (SINT16)fCoe;             // 16λ�������洢
                // ����ϵ��
                fCoe = (2.0*cos(dwHarm0*dwSamNO*fPiN)/N)*u16Coe;// 2cos(H*D*(2pi/N))/N �ٷŴ�2^16��
                fCoe+= (fCoe>=0)? 0.5 : -0.5 ;                  // ��������
                pS16CosCoe[dwSamNO] = (SINT16)fCoe;             // 16λ�������洢
            }
        }
    }
}

// ============================================================================
// �������ܣ�����������
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_Sam_Dif( void )
{
    UINT32      dwRatio, dwChanNO, dwSamNO;
    UINT32      dwSamRate;
    UINT32      dwSamPtr, dwDifPtr;
    UINT16      u16DifCoe;
    SINT16      *ps16Sam, *ps16Dif;
    SINT16      s16Dif;
    FLOAT32     fDifCoe;

    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)           // 1. ����Ƶ��N��ѭ��
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);                       // ����Ƶ��(20��40��80)

        // ��ȡ���ϵ��   1/(2sin[((2��/N)*D)/2])
        fDifCoe = 1/(2*sin(CN_PI*CN_SAM_DIF/dwSamRate));
        u16DifCoe = (UINT16)(fDifCoe*(1<<CN_FFT_COE_MAG));          // ������ �Ŵ�2^CN_FFT_COE_MAG ��

        for(dwChanNO=0; dwChanNO < CN_SAM_CHAN_MAX; dwChanNO++)     // 2. ����ͨ��C��ѭ��
        {
            ps16Sam = &(g_s16SamData[dwRatio][dwChanNO][0]);        // ԭʼ���������׵�ַ
            ps16Dif = &(g_s16DifData[dwRatio][dwChanNO][0]);        // ��ֲ��������׵�ַ
            for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)          // 3. ��������N��ѭ��
            {
                // ������ָ��
                dwSamPtr= (dwSamNO+CN_SAM_PTR)%dwSamRate;;          // ��ǰ������ָ�� n=(i+p)%N
                dwDifPtr= (dwSamPtr+dwSamRate-CN_SAM_DIF)%dwSamRate;// ��ֲ�����ָ�� d=(n+N-D)%N   
                // ��ּ���
                s16Dif  = ps16Sam[dwSamPtr] - ps16Sam[dwDifPtr];        // �������ݲ��ֵ id = i(n) - i(d)
                s16Dif  = ((SINT32)s16Dif*u16DifCoe)>>CN_FFT_COE_MAG;   // ���ϵ������(��ԭΪԭʼ������ϵ��)
                // �洢���
                ps16Dif[dwSamPtr]           = s16Dif;                   // ˫������(��һ��)
                ps16Dif[dwSamPtr+dwSamRate] = s16Dif;                   // ˫������(�ڶ���)
            }
        }
    }
}

// ============================================================================
// �������ܣ� �����к���(���)
// ��������� �鲿/ʵ��ֵ
// ��������� ��
// ����ֵ��   ���,��Χ0~180��,�Ŵ�������
// ============================================================================
UINT16 Cal_ArcTg( UINT16 u16Tg )
{
    UINT16      wi;

    if( u16Tg<g_u16Tan_tab[0] )
    {
        return(0);
    }

    if( u16Tg>=g_u16Tan_tab[179] )
    {
        return(180);
    }

    for( wi=0; wi<179; wi++ )
    {
        if( (u16Tg>=g_u16Tan_tab[wi]) && (u16Tg<g_u16Tan_tab[wi+1]) )
        {
            break;
        }
    }
    return(wi+1);
}

// ============================================================================
// �������ܣ��������������
// ���������
//          SINT32  s32Re: ʵ��
//          SINT32  s32Im: �鲿
// �����������
// ����ֵ��  ���(��λ:0.5��)
// ============================================================================
UINT16 Cal_Vec_Ang( SINT32 s32Re, SINT32 s32Im )
{
    UINT16     wAng;
    UINT32     dwf;

    if( s32Re==0 )
    {
        if( s32Im==0 ) return(0);
        else return(s32Im>0?90:270);
    }

    if( s32Re>0 )
    {
        if( s32Im>=0 )
        {
            dwf = 500*s32Im / s32Re;
            wAng= Cal_ArcTg( dwf );
        }
        else
        {
            s32Im = (~s32Im)+1;
            dwf = 500*s32Im / s32Re;
            wAng= 720 - Cal_ArcTg( dwf );
        }
    }
    else
    {
        s32Re = (~s32Re)+1;
        if( s32Im>=0 )
        {
            dwf = 500 * s32Im / s32Re;
            wAng= 360 - Cal_ArcTg( dwf );
        }
        else
        {
            s32Im = (~s32Im)+1;
            dwf = 500 * s32Im / s32Re;
            wAng= 360 + Cal_ArcTg( dwf );
        }
    }
    // �Ŵ�������(��Χ:0~720��)(��λ:0.5��)
    return( wAng );
}

// ============================================================================
// �������ܣ����ݲ���Ƶ�ʻ�ȡ��Ӧ�������±�
// ���������UINT32 dwSamRate: ����Ƶ��
// �����������
// ����ֵ��  ��Ӧ�������±�
// ============================================================================
UINT8 Get_SamRate_NO( UINT32 dwSamRate )
{
    UINT8   u8Ratio=0;
    
    if( (dwSamRate/2)==CN_SAM_RATE )
    {
        u8Ratio = 1;
    }
    else if( (dwSamRate/4)==CN_SAM_RATE )
    {
        u8Ratio = 2;
    }
    return( u8Ratio );
}

// ============================================================================
// �������ܣ�����FFT�����FFT�ṹ�����
// ���������
//          SINT32      s32Re       : ʵ��
//          SINT32      s32Im       : �鲿
//          tagAnaVec   *ptAnaVec   : ģ���������ṹ�� 
// �����������
// ����ֵ��  ��
// ============================================================================
void Save_FFTVec( SINT32 s32Re, SINT32 s32Im, tagAnaVec *ptAnaVec )
{
    FLOAT32     f32Amp;
    // ������
    ptAnaVec->s32Re = s32Re;                        // ʵ��
    ptAnaVec->s32Im = s32Im;                        // �鲿
    ptAnaVec->s32Amp= s32Re*s32Re+s32Im*s32Im;      // ��ֵ(ƽ��)
    ptAnaVec->s32Ang= Cal_Vec_Ang( s32Re, s32Im );  // �Ƕ�(��λ:0.5��)

    // ������
    // ����ͨ���Ŵ���( �Ŵ�500��;��λ:����ͨ��2mA )
    f32Amp = ((FLOAT32)ptAnaVec->s32Amp)/(CN_ANA_SAM_MAG*CN_ANA_SAM_MAG);
    ptAnaVec->fVal = sqrt(f32Amp/2);                // ��Чֵ(��ƽ��)
    ptAnaVec->fAng = ptAnaVec->s32Ang*0.5;          // �Ƕ�(��λ:1.0��)
}

// ============================================================================
// �������ܣ�����ȫ��FFT
// ���������
//          SINT16       *ps16Sam  : ������ָ��
//          UINT32       dwSamRate : ����Ƶ��
//          UINT32       dwHarm    : г������
//          tagFFTData   *ptFFTData: FFT���ָ��
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_FFTVec_Full( SINT16 *ps16Sam, UINT32 dwSamRate, UINT32 dwHarm, tagFFTData *ptFFTData )
{
    UINT32      dwSamNO, dwRatio;
    SINT32      s32Re, s32Im;
    SINT64      s64Re, s64Im;
//    SINT16      s16Sam;
    SINT32      s32Sam;
    SINT16      *pS16SinCoe, *pS16CosCoe;   // ������ϵ���׵�ַ

    tagAnaVec   *ptAnaVec;

    // ���ݲ���Ƶ�ʻ�ȡ��Ӧ�������±�
    dwRatio = Get_SamRate_NO( dwSamRate );
    // ������ϵ���׵�ַ
    pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][dwHarm][0]); // ����ϵ���׵�ַ
    pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][dwHarm][0]); // ����ϵ���׵�ַ
    // FFT����
    s64Re = 0;      // ʵ��
    s64Im = 0;      // �鲿
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // ��������N��ѭ��
    {
        s32Sam = ps16Sam[dwSamNO];
        s64Re += s32Sam*pS16SinCoe[dwSamNO];    // ʵ��
        s64Im += s32Sam*pS16CosCoe[dwSamNO];    // �鲿
    }
//    s32Re = 2*s32Re/(SINT32)dwSamRate;      // *(2/N)
//    s32Im = 2*s32Im/(SINT32)dwSamRate;
    // ����������ϵ���ķŴ���(2^16)
    s32Re = s64Re>>(CN_FFT_COE_MAG);            // *1/(2^CN_FFT_COE_MAG)
    s32Im = s64Im>>(CN_FFT_COE_MAG);
    // ����FFT������
    ptAnaVec = &(ptFFTData->tAnaVec[dwHarm]);
    Save_FFTVec( s32Re, s32Im, ptAnaVec );
}

// ============================================================================
// �������ܣ�����벨FFT
// ���������
//          SINT16       *ps16Sam  : ������ָ��
//          UINT32       dwSamRate : ����Ƶ��
//          tagFFTData   *ptFFTData: FFT���ָ��
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_FFTVec_Half( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32      dwSamNO, dwRatio;
    SINT32      s32Re, s32Im;
    SINT64      s64Re, s64Im;
//    SINT16      s16Sam;
    SINT32      s32Sam;
    SINT16      *pS16SinCoe, *pS16CosCoe;   // ������ϵ���׵�ַ

    tagAnaVec   *ptAnaVec;

    // ���ݲ���Ƶ�ʻ�ȡ��Ӧ�������±�
    dwRatio = Get_SamRate_NO( dwSamRate );
    // ������ϵ���׵�ַ
    pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][0][0]); // ����ϵ���׵�ַ
    pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][0][0]); // ����ϵ���׵�ַ
    // FFT����
    s32Re = 0;      // ʵ��
    s32Im = 0;      // �鲿
    for(dwSamNO=0; dwSamNO < dwSamRate/2; dwSamNO++)  // ��������N��ѭ��
    {
        s32Sam = ps16Sam[dwSamNO];
        s64Re += s32Sam*pS16SinCoe[dwSamNO];        // ʵ��
        s64Im += s32Sam*pS16CosCoe[dwSamNO];        // �鲿
    }
//    s32Re = 4*s32Re/(SINT32)dwSamRate;      // *(4/N)
//    s32Im = 4*s32Im/(SINT32)dwSamRate;
    // ����������ϵ���ķŴ���(2^16)
    s32Re = 2*(s64Re>>CN_FFT_COE_MAG);        // *1/(2^CN_FFT_COE_MAG)
    s32Im = 2*(s64Im>>CN_FFT_COE_MAG);
    // ����FFT������
    ptAnaVec = &(ptFFTData->tAnaVecDif);
    //���ڰ벨�����㷨��ȫ�������㷨�����ݴ���ʼ��������ܲ���Ҳ����180�ȣ�
    //��ȫ�ܺͰ��ܻ��ʹ�õ�����£�Ϊ��֤��������������㷨��ʵ�鲿ͬʱȡ��
    Save_FFTVec( -s32Re, -s32Im, ptAnaVec );
}

// ============================================================================
// �������ܣ�����ֱ������
// ���������
//          SINT16       *ps16Sam  : ������ָ��
//          UINT32       dwSamRate : ����Ƶ��
//          tagFFTData   *ptFFTData: FFT���ָ��
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_Ana_DC( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32  dwSamNO;
    SINT32  s32DC;
    
    s32DC = 0;
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // ��������N��ѭ��
    {
        s32DC += ps16Sam[dwSamNO];
    }
    ptFFTData->s32DC = s32DC/dwSamRate;  // ֱ������������
}

// ============================================================================
// �������ܣ����������
// ���������
//          SINT16       *ps16Sam  : ������ָ��
//          UINT32       dwSamRate : ����Ƶ��
//          tagFFTData   *ptFFTData: FFT���ָ��
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_Ana_RMS( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32  dwSamNO;
    UINT32  u32RMS;
    SINT16  s16Sam;
    
    u32RMS = 0;
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // ��������N��ѭ��
    {
        s16Sam = ps16Sam[dwSamNO];
        u32RMS += s16Sam*s16Sam;
    }
    ptFFTData->u32RMS = u32RMS/dwSamRate;  // ������������
}

// ============================================================================
// �������ܣ�������������
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================
void Cal_Ana_Vec( void )
{
    UINT32      dwRatio, dwChanNO, dwHarm;
    UINT32      dwSamPtr0, dwSamPtr1, dwSamPtr2;
    UINT32      dwSamRate;
    SINT16      *ps16Sam, *ps16Dif;
    tagFFTData  *ptFFTData;

    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)       // 1. ����Ƶ��N��ѭ��
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);   // ����Ƶ��(20��40��80)

        for(dwChanNO=0; dwChanNO < CN_SAM_CHAN_MAX; dwChanNO++) // 2. ����ͨ��C��ѭ��
        {
            ptFFTData = &(g_tFFTData[dwRatio][dwChanNO]);       // FFT���ָ��
            // ������ָ��
            dwSamPtr0= (CN_SAM_PTR+dwSamRate-1)%dwSamRate;      // ��ǰ������ָ�� n=p ( CN_SAM_PTRָ����ɵĵ�)
            dwSamPtr0= dwSamPtr0+dwSamRate;                     // ˫������(�ڶ���)��ָ�� n=n+N
            dwSamPtr1= dwSamPtr0-dwSamRate+1;                   // ָ���ܲ���ɵĲ����� n' =n-N+1
            dwSamPtr2= dwSamPtr0-dwSamRate/2+1;                 // ָ����ܲ�ǰ�Ĳ�����   n''=n-N/2+1
            // ������ָ��
            ps16Sam = &(g_s16SamData[dwRatio][dwChanNO][dwSamPtr1]);    // ԭʼ���������׵�ַ(ָ��һ�ܲ�ǰ�Ĳ�����)
            ps16Dif = &(g_s16DifData[dwRatio][dwChanNO][dwSamPtr2]);    // ��ֲ��������׵�ַ(ָ����ܲ�ǰ�Ĳ�����)
            for(dwHarm=0; dwHarm < CN_ANA_HARM_MAX; dwHarm++)   // 3. г������H��ѭ��
            {
                // ���������г����ȫ��FFT
                Cal_FFTVec_Full( ps16Sam, dwSamRate, dwHarm, ptFFTData );   // Դ������Ϊԭʼ������
            }
            // ��������İ���FFT
            Cal_FFTVec_Half( ps16Dif, dwSamRate, ptFFTData );               // Դ������Ϊ��ֲ�����
            // ����ֱ������
            Cal_Ana_DC( ps16Sam, dwSamRate, ptFFTData );                    // Դ������Ϊԭʼ������
            // ���������
            Cal_Ana_RMS( ps16Sam, dwSamRate, ptFFTData );                   // Դ������Ϊԭʼ������
        }
    }
}

// ============================================================================
// �������ܣ�main( )����
// �����������
// �����������
// ����ֵ��  ��
// ============================================================================
void main( void )
{
    //========================================================
    // ��ʼ����������
    //========================================================
    printf("\n Initializing ... \n");

    // ����FFT������ϵ��
    Cal_FFT_SinCosCoe( );

    // �������ͨ��ģ���������
    Cal_Simulate_Sam_Data( );

    printf("\n Initialization Complete! \n");

    //========================================================
    // FFT����
    //========================================================

    printf("\n Calculating ... \n");

    // ����������
    Cal_Sam_Dif( );

    // ������������
    Cal_Ana_Vec( );

    printf("\n Calculate Complete! \n");
}


#define C      (16)     // ͨ����
#define H      (13)     // г����
#define N      (80)     // ��������
void Dbg_FFT( void )
{

    int         i, j, k;
    int         iSam[C*N*2];    // ��������
    int         iSin[N*H];      // ����ϵ��
    int         iCos[N*H];      // ����ϵ��
    int         iRe[C*H];       // ʵ��
    int         iIm[C*H];       // �鲿
    int         iAmp[C*H];      // ��ֵ
    long long   A, B;           // ��ʱ����
    
    for( k=0; k<C; k++ )           // C=16 ͨ����
    {
        for( j=0; j<H; j++ )       // H=13 г����
        {
            A = 0;      // ʵ��
            B = 0;      // �鲿
            for( i=0; i<N; i++ )   // N=80 ��������
            {
                A += iSam[i]*iSin[j*N+i];
                B += iSam[i]*iCos[j*N+i];
            }
            iRe[k*H+j] = A>>16;
            iIm[k*H+j] = B>>16;
            iAmp[k*H+j]= A*A + B*B;
        }
    }


}