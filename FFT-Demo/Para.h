// ===============================================================================
// Copyright (C)  All Rights Reserved
// 模块描述: 数据区及仿真参数设置定义
// 模块版本: V1.00
// 创建人员: zhanjw
// 创建时间: 2018-09-01
// ===============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ===============================================================================

#ifndef __PARA_H__
#define __PARA_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// 宏定义
//========================================================
// 参数设置
#define CN_SAM_PTR          (0)                                 // 采样数据存储起始指针
#define CN_SAM_DIF          (2)                                 // 差分点数
#define CN_ANA_SAM_MAG      (500)                               // 模拟量采样数据放大倍数(放大500倍 单位:电流通道2mA)
#define CN_FFT_COE_MAG      (16)                                // 正余弦系数表的放大倍数(2的n次幂 2^16)

#define CN_SAM_RATE         (20)                                // 每周波采样点数
#define CN_SAM_RATIO_NUM    (3)                                 // 采样倍率数目(倍率1、2、4分别对应采样频率20、40、80)
//#define CN_SAM_DOT_MAX    (80)                                // 每周波最大采样点数
#define CN_SAM_DOT_MAX      (CN_SAM_RATE*(1<<CN_SAM_RATIO_NUM)) // 每周波最大采样点数
#define CN_SAM_CHAN_MAX     (16)                                // 最大采样通道数
#define CN_ANA_HARM_MAX     (13)                                // 最大谐波次数(包括基波)
#define CN_ANA_HARM_PARA    (CN_ANA_HARM_MAX+1)                 // 测试模拟量谐波参数(包括直流分量)

// 常用常数定义
#define CN_SQRT_2           (1.4142135623731)                   // sqrt(2)
#define CN_EVO3_2           (0.8660254037844)                   // sqrt(3)*0.5
#define CN_RECIP_3          (0.3333333333333)                   // 3的倒数
#define CN_PI               (3.1415926535898)                   // pi
#define CN_Rad2AngCoe       (180/CN_PI)                         // 弧度转化为角度的系数
#define CN_Ang2RadCoe       (CN_PI/180)                         // 角度转化为弧度的系数
#define CN_Rad2Ang( Rad )   ( (FLOAT32)(Rad)*CN_Rad2AngCoe )    // 弧度转化为角度
#define CN_Ang2Rad( Ang )   ( (FLOAT32)(Ang)*CN_Ang2RadCoe )    // 角度转化为弧度

#ifdef __cplusplus
}
#endif

#endif // __PARA_H__

