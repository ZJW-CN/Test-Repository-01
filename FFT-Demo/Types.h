// ===============================================================================
// Copyright (C)  All Rights Reserved
// 模块描述: 
// 模块版本: V1.00
// 创建人员: zhanjw
// 创建时间: 2018-09-01
// ===============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// ===============================================================================


#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// 头文件包含
//========================================================
#include "stdio.h"
#include "string.h"             // C语言库
#include "math.h"

//========================================================
// 数据类型定义
//========================================================
typedef unsigned  char      UINT8;
typedef unsigned  short     UINT16;
typedef unsigned  int       UINT32;
typedef unsigned  long long UINT64;

typedef signed  char        SINT8;
typedef short int           SINT16;
typedef int                 SINT32;
typedef signed    long long SINT64;

typedef volatile UINT8      VUINT8;
typedef volatile UINT16     VUINT16;
typedef volatile UINT32     VUINT32;

//typedef float               FLOAT32;
typedef double              FLOAT32;

#ifndef FALSE
    #define FALSE               0
#endif
#ifndef TRUE
    #define TRUE                1
#endif

#ifndef NULL
    #define NULL	            0
#endif

#ifndef BOOL
    #define BOOL                UINT8
#endif

#ifndef STATUS
    #define STATUS              int
#endif

#ifndef ERROR
    #define ERROR               (-1)
#endif

#ifndef OK
    #define OK                  (0)
#endif

#ifdef __cplusplus
}
#endif

#endif // __TYPES_H__

