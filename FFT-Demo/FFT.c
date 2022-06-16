// ===============================================================================
// Copyright (C)  All Rights Reserved
// 模块描述: FFT运算
// 模块版本: V1.00
// 创建人员: zhanjw
// 创建时间: 2018-09-01
// ===============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ===============================================================================

#include "Types.h"
#include "Para.h"
#include "Data.h"

// ============================================================================
// 函数功能：计算全波FFT正余弦系数
// 输入参数：无
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_FFT_SinCosCoe( void )
{
    UINT32  dwRatio, dwHarm, dwSamNO;   // 临时变量
    UINT32  dwSamRate;                  // 采样频率
    UINT32  dwHarm0;
    SINT16  *pS16SinCoe, *pS16CosCoe;   // 正余弦系数首地址
    UINT16  u16Coe;                     // 正余弦系数的放大倍数
    FLOAT32 fPiN, fCoe;
    
    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)           // 1. 采样频率N的循环
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);   // 采样频率(20、40、80)
        //u16Coe    = 1<<(CN_FFT_COE_MAG-dwRatio);// 正余弦系数的放大倍数(采样频率20、40、80对应的放大倍数为:2^12、2^11、2^10)
        u16Coe    = 1<<CN_FFT_COE_MAG;          // 正余弦系数的放大倍数(2^16)
        for(dwHarm=0; dwHarm < CN_ANA_HARM_MAX; dwHarm++)        // 2. 谐波次数H的循环
        {
            dwHarm0 = dwHarm + 1;
            for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)          // 3. 采样点数N的循环
            {
                pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][dwHarm][0]);// 正弦系数首地址
                pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][dwHarm][0]);// 余弦系数首地址

                fPiN = 2.0*CN_PI/dwSamRate;     // 2pi/N
                // 正弦系数
                fCoe = (2.0*sin(dwHarm0*dwSamNO*fPiN)/N)*u16Coe;// 2sin(H*D*(2pi/N))/N 再放大2^16倍
                fCoe+= (fCoe>=0)? 0.5 : -0.5 ;                  // 四舍五入
                pS16SinCoe[dwSamNO] = (SINT16)fCoe;             // 16位定点数存储
                // 余弦系数
                fCoe = (2.0*cos(dwHarm0*dwSamNO*fPiN)/N)*u16Coe;// 2cos(H*D*(2pi/N))/N 再放大2^16倍
                fCoe+= (fCoe>=0)? 0.5 : -0.5 ;                  // 四舍五入
                pS16CosCoe[dwSamNO] = (SINT16)fCoe;             // 16位定点数存储
            }
        }
    }
}

// ============================================================================
// 函数功能：计算差分数据
// 输入参数：无
// 输出参数：无
// 返回值：  无
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

    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)           // 1. 采样频率N的循环
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);                       // 采样频率(20、40、80)

        // 求取差分系数   1/(2sin[((2π/N)*D)/2])
        fDifCoe = 1/(2*sin(CN_PI*CN_SAM_DIF/dwSamRate));
        u16DifCoe = (UINT16)(fDifCoe*(1<<CN_FFT_COE_MAG));          // 定点数 放大2^CN_FFT_COE_MAG 倍

        for(dwChanNO=0; dwChanNO < CN_SAM_CHAN_MAX; dwChanNO++)     // 2. 采样通道C的循环
        {
            ps16Sam = &(g_s16SamData[dwRatio][dwChanNO][0]);        // 原始采样数据首地址
            ps16Dif = &(g_s16DifData[dwRatio][dwChanNO][0]);        // 差分采样数据首地址
            for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)          // 3. 采样点数N的循环
            {
                // 采样点指针
                dwSamPtr= (dwSamNO+CN_SAM_PTR)%dwSamRate;;          // 当前采样点指针 n=(i+p)%N
                dwDifPtr= (dwSamPtr+dwSamRate-CN_SAM_DIF)%dwSamRate;// 差分采样点指针 d=(n+N-D)%N   
                // 差分计算
                s16Dif  = ps16Sam[dwSamPtr] - ps16Sam[dwDifPtr];        // 采样数据差分值 id = i(n) - i(d)
                s16Dif  = ((SINT32)s16Dif*u16DifCoe)>>CN_FFT_COE_MAG;   // 差分系数折算(还原为原始采样的系数)
                // 存储结果
                ps16Dif[dwSamPtr]           = s16Dif;                   // 双倍缓存(第一区)
                ps16Dif[dwSamPtr+dwSamRate] = s16Dif;                   // 双倍缓存(第二区)
            }
        }
    }
}

// ============================================================================
// 函数功能： 反正切函数(查表法)
// 输入参数： 虚部/实部值
// 输出参数： 无
// 返回值：   相角,范围0~180度,放大了两倍
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
// 函数功能：计算向量的相角
// 输入参数：
//          SINT32  s32Re: 实部
//          SINT32  s32Im: 虚部
// 输出参数：无
// 返回值：  相角(单位:0.5°)
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
    // 放大了两倍(范围:0~720度)(单位:0.5°)
    return( wAng );
}

// ============================================================================
// 函数功能：根据采样频率获取对应的数组下标
// 输入参数：UINT32 dwSamRate: 采样频率
// 输出参数：无
// 返回值：  对应的数组下标
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
// 函数功能：保存FFT结果到FFT结构体变量
// 输入参数：
//          SINT32      s32Re       : 实部
//          SINT32      s32Im       : 虚部
//          tagAnaVec   *ptAnaVec   : 模拟量向量结构体 
// 输出参数：无
// 返回值：  无
// ============================================================================
void Save_FFTVec( SINT32 s32Re, SINT32 s32Im, tagAnaVec *ptAnaVec )
{
    FLOAT32     f32Amp;
    // 定点数
    ptAnaVec->s32Re = s32Re;                        // 实部
    ptAnaVec->s32Im = s32Im;                        // 虚部
    ptAnaVec->s32Amp= s32Re*s32Re+s32Im*s32Im;      // 幅值(平方)
    ptAnaVec->s32Ang= Cal_Vec_Ang( s32Re, s32Im );  // 角度(单位:0.5°)

    // 浮点数
    // 消除通道放大倍数( 放大500倍;单位:电流通道2mA )
    f32Amp = ((FLOAT32)ptAnaVec->s32Amp)/(CN_ANA_SAM_MAG*CN_ANA_SAM_MAG);
    ptAnaVec->fVal = sqrt(f32Amp/2);                // 有效值(开平方)
    ptAnaVec->fAng = ptAnaVec->s32Ang*0.5;          // 角度(单位:1.0°)
}

// ============================================================================
// 函数功能：计算全波FFT
// 输入参数：
//          SINT16       *ps16Sam  : 数据区指针
//          UINT32       dwSamRate : 采样频率
//          UINT32       dwHarm    : 谐波次数
//          tagFFTData   *ptFFTData: FFT结果指针
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_FFTVec_Full( SINT16 *ps16Sam, UINT32 dwSamRate, UINT32 dwHarm, tagFFTData *ptFFTData )
{
    UINT32      dwSamNO, dwRatio;
    SINT32      s32Re, s32Im;
    SINT64      s64Re, s64Im;
//    SINT16      s16Sam;
    SINT32      s32Sam;
    SINT16      *pS16SinCoe, *pS16CosCoe;   // 正余弦系数首地址

    tagAnaVec   *ptAnaVec;

    // 根据采样频率获取对应的数组下标
    dwRatio = Get_SamRate_NO( dwSamRate );
    // 正余弦系数首地址
    pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][dwHarm][0]); // 正弦系数首地址
    pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][dwHarm][0]); // 余弦系数首地址
    // FFT运算
    s64Re = 0;      // 实部
    s64Im = 0;      // 虚部
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // 采样点数N的循环
    {
        s32Sam = ps16Sam[dwSamNO];
        s64Re += s32Sam*pS16SinCoe[dwSamNO];    // 实部
        s64Im += s32Sam*pS16CosCoe[dwSamNO];    // 虚部
    }
//    s32Re = 2*s32Re/(SINT32)dwSamRate;      // *(2/N)
//    s32Im = 2*s32Im/(SINT32)dwSamRate;
    // 消除正余弦系数的放大倍数(2^16)
    s32Re = s64Re>>(CN_FFT_COE_MAG);            // *1/(2^CN_FFT_COE_MAG)
    s32Im = s64Im>>(CN_FFT_COE_MAG);
    // 保存FFT运算结果
    ptAnaVec = &(ptFFTData->tAnaVec[dwHarm]);
    Save_FFTVec( s32Re, s32Im, ptAnaVec );
}

// ============================================================================
// 函数功能：计算半波FFT
// 输入参数：
//          SINT16       *ps16Sam  : 数据区指针
//          UINT32       dwSamRate : 采样频率
//          tagFFTData   *ptFFTData: FFT结果指针
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_FFTVec_Half( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32      dwSamNO, dwRatio;
    SINT32      s32Re, s32Im;
    SINT64      s64Re, s64Im;
//    SINT16      s16Sam;
    SINT32      s32Sam;
    SINT16      *pS16SinCoe, *pS16CosCoe;   // 正余弦系数首地址

    tagAnaVec   *ptAnaVec;

    // 根据采样频率获取对应的数组下标
    dwRatio = Get_SamRate_NO( dwSamRate );
    // 正余弦系数首地址
    pS16SinCoe = &(g_s16FFTSinCoe[dwRatio][0][0]); // 正弦系数首地址
    pS16CosCoe = &(g_s16FFTCosCoe[dwRatio][0][0]); // 余弦系数首地址
    // FFT运算
    s32Re = 0;      // 实部
    s32Im = 0;      // 虚部
    for(dwSamNO=0; dwSamNO < dwSamRate/2; dwSamNO++)  // 采样点数N的循环
    {
        s32Sam = ps16Sam[dwSamNO];
        s64Re += s32Sam*pS16SinCoe[dwSamNO];        // 实部
        s64Im += s32Sam*pS16CosCoe[dwSamNO];        // 虚部
    }
//    s32Re = 4*s32Re/(SINT32)dwSamRate;      // *(4/N)
//    s32Im = 4*s32Im/(SINT32)dwSamRate;
    // 消除正余弦系数的放大倍数(2^16)
    s32Re = 2*(s64Re>>CN_FFT_COE_MAG);        // *1/(2^CN_FFT_COE_MAG)
    s32Im = 2*(s64Im>>CN_FFT_COE_MAG);
    // 保存FFT运算结果
    ptAnaVec = &(ptFFTData->tAnaVecDif);
    //由于半波傅氏算法与全波傅氏算法的数据窗起始点相差半个周波，也就是180度，
    //在全周和半周混合使用的情况下，为保证相角连续，半周算法的实虚部同时取反
    Save_FFTVec( -s32Re, -s32Im, ptAnaVec );
}

// ============================================================================
// 函数功能：计算直流分量
// 输入参数：
//          SINT16       *ps16Sam  : 数据区指针
//          UINT32       dwSamRate : 采样频率
//          tagFFTData   *ptFFTData: FFT结果指针
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_Ana_DC( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32  dwSamNO;
    SINT32  s32DC;
    
    s32DC = 0;
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // 采样点数N的循环
    {
        s32DC += ps16Sam[dwSamNO];
    }
    ptFFTData->s32DC = s32DC/dwSamRate;  // 直流分量计算结果
}

// ============================================================================
// 函数功能：计算均方根
// 输入参数：
//          SINT16       *ps16Sam  : 数据区指针
//          UINT32       dwSamRate : 采样频率
//          tagFFTData   *ptFFTData: FFT结果指针
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_Ana_RMS( SINT16 *ps16Sam, UINT32 dwSamRate, tagFFTData *ptFFTData )
{
    UINT32  dwSamNO;
    UINT32  u32RMS;
    SINT16  s16Sam;
    
    u32RMS = 0;
    for(dwSamNO=0; dwSamNO < dwSamRate; dwSamNO++)  // 采样点数N的循环
    {
        s16Sam = ps16Sam[dwSamNO];
        u32RMS += s16Sam*s16Sam;
    }
    ptFFTData->u32RMS = u32RMS/dwSamRate;  // 均方根计算结果
}

// ============================================================================
// 函数功能：计算向量数据
// 输入参数：无
// 输出参数：无
// 返回值：  无
// ============================================================================
void Cal_Ana_Vec( void )
{
    UINT32      dwRatio, dwChanNO, dwHarm;
    UINT32      dwSamPtr0, dwSamPtr1, dwSamPtr2;
    UINT32      dwSamRate;
    SINT16      *ps16Sam, *ps16Dif;
    tagFFTData  *ptFFTData;

    for(dwRatio=0; dwRatio < CN_SAM_RATIO_NUM; dwRatio++)       // 1. 采样频率N的循环
    {
        dwSamRate = CN_SAM_RATE*(1<<dwRatio);   // 采样频率(20、40、80)

        for(dwChanNO=0; dwChanNO < CN_SAM_CHAN_MAX; dwChanNO++) // 2. 采样通道C的循环
        {
            ptFFTData = &(g_tFFTData[dwRatio][dwChanNO]);       // FFT结果指针
            // 采样点指针
            dwSamPtr0= (CN_SAM_PTR+dwSamRate-1)%dwSamRate;      // 当前采样点指针 n=p ( CN_SAM_PTR指向最旧的点)
            dwSamPtr0= dwSamPtr0+dwSamRate;                     // 双倍缓存(第二区)的指针 n=n+N
            dwSamPtr1= dwSamPtr0-dwSamRate+1;                   // 指向本周波最旧的采样点 n' =n-N+1
            dwSamPtr2= dwSamPtr0-dwSamRate/2+1;                 // 指向半周波前的采样点   n''=n-N/2+1
            // 数据区指针
            ps16Sam = &(g_s16SamData[dwRatio][dwChanNO][dwSamPtr1]);    // 原始采样数据首地址(指向一周波前的采样点)
            ps16Dif = &(g_s16DifData[dwRatio][dwChanNO][dwSamPtr2]);    // 差分采样数据首地址(指向半周波前的采样点)
            for(dwHarm=0; dwHarm < CN_ANA_HARM_MAX; dwHarm++)   // 3. 谐波次数H的循环
            {
                // 计算基波及谐波的全波FFT
                Cal_FFTVec_Full( ps16Sam, dwSamRate, dwHarm, ptFFTData );   // 源数据区为原始采样区
            }
            // 计算基波的半周FFT
            Cal_FFTVec_Half( ps16Dif, dwSamRate, ptFFTData );               // 源数据区为差分采样区
            // 计算直流分量
            Cal_Ana_DC( ps16Sam, dwSamRate, ptFFTData );                    // 源数据区为原始采样区
            // 计算均方根
            Cal_Ana_RMS( ps16Sam, dwSamRate, ptFFTData );                   // 源数据区为原始采样区
        }
    }
}

// ============================================================================
// 函数功能：main( )函数
// 输入参数：无
// 输出参数：无
// 返回值：  无
// ============================================================================
void main( void )
{
    //========================================================
    // 初始化仿真数据
    //========================================================
    printf("\n Initializing ... \n");

    // 计算FFT正余弦系数
    Cal_FFT_SinCosCoe( );

    // 计算采样通道模拟仿真数据
    Cal_Simulate_Sam_Data( );

    printf("\n Initialization Complete! \n");

    //========================================================
    // FFT计算
    //========================================================

    printf("\n Calculating ... \n");

    // 计算差分数据
    Cal_Sam_Dif( );

    // 计算向量数据
    Cal_Ana_Vec( );

    printf("\n Calculate Complete! \n");
}


#define C      (16)     // 通道数
#define H      (13)     // 谐波数
#define N      (80)     // 采样点数
void Dbg_FFT( void )
{

    int         i, j, k;
    int         iSam[C*N*2];    // 采样数据
    int         iSin[N*H];      // 正弦系数
    int         iCos[N*H];      // 余弦系数
    int         iRe[C*H];       // 实部
    int         iIm[C*H];       // 虚部
    int         iAmp[C*H];      // 幅值
    long long   A, B;           // 临时变量
    
    for( k=0; k<C; k++ )           // C=16 通道数
    {
        for( j=0; j<H; j++ )       // H=13 谐波数
        {
            A = 0;      // 实部
            B = 0;      // 虚部
            for( i=0; i<N; i++ )   // N=80 采样点数
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