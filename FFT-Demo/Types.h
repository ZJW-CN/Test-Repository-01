// ===============================================================================
// Copyright (C)  All Rights Reserved
// ģ������: 
// ģ��汾: V1.00
// ������Ա: zhanjw
// ����ʱ��: 2018-09-01
// ===============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ===============================================================================


#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

//========================================================
// ͷ�ļ�����
//========================================================
#include "stdio.h"
#include "string.h"             // C���Կ�
#include "math.h"

//========================================================
// �������Ͷ���
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

