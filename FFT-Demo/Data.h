// ===============================================================================
// Copyright (C)  All Rights Reserved
// 模块描述: 数据类型定义
// 模块版本: V1.00
// 创建人员: 占捷文
// 创建时间: 2018-09-01
// ===============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ===============================================================================

#ifndef __DATA_H__
#define __DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// 头文件包含
//========================================================
#include "Types.h"
#include "Para.h"

//========================================================
// 数据结构体定义
//========================================================
// 模拟量参数结构体
typedef struct
{
    UINT32      dwHarm;     // 谐波次数
    FLOAT32     fAm;        // 幅值
    FLOAT32     fAng;       // 初始相角
}tagAnaPara;

// 模拟量向量结构体
typedef struct
{
    // 定点数 放大500倍(单位:电流通道2mA)
    SINT32      s32Re;      // 实部
    SINT32      s32Im;      // 虚部
    SINT32      s32Amp;     // 幅值(平方)
    SINT32      s32Ang;     // 角度(单位:0.5°)
    // 浮点数
    FLOAT32     fVal;       // 有效值(开平方)
    FLOAT32     fAng;       // 角度(单位:1.0°)
}tagAnaVec;

// 模拟量向量结构体
typedef struct
{
    SINT32      s32DC;      // 直流分量
    UINT32      u32RMS;     // 均方根
    tagAnaVec   tAnaVec[CN_ANA_HARM_MAX];   // 全波FFT向量 [1维]:谐波次数H:1~13
    tagAnaVec   tAnaVecDif;                 // 半波FFT向量
}tagFFTData;

//========================================================
// 变量声明
//========================================================
// FFT的正弦系数
extern SINT16  g_s16FFTSinCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];
// FFT的余弦系数
extern SINT16  g_s16FFTCosCoe[CN_SAM_RATIO_NUM][CN_ANA_HARM_MAX][CN_SAM_DOT_MAX];

// 采样通道原始数据区
extern SINT16  g_s16SamData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];
// 采样通道差分数据区
extern SINT16  g_s16DifData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX][CN_SAM_DOT_MAX*2];

// 模拟量向量FFT结果
extern tagFFTData  g_tFFTData[CN_SAM_RATIO_NUM][CN_SAM_CHAN_MAX];

// 仿真测试模拟量参数设置指针数组
extern tagAnaPara *g_ptAnaPara[CN_SAM_CHAN_MAX];

// 正切数据表
extern const UINT16 g_u16Tan_tab[180];

//========================================================
// 函数声明
//========================================================
extern void Cal_Simulate_Sam_Data( void );

#ifdef __cplusplus
}
#endif

#endif // __DATA_H__

