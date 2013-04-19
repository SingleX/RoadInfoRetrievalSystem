/*******************************************************************************
*	Copyright (c) 2002-? Neusoft Co., Ltd. All Rights Reserved.
*	FileName		:	Macro.h
*	Date			:	2012.01.13
*	Author			:	zhengchuang
*	Module			:	Data analysis
*	Function		:	Data analysis
*-------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

#ifndef	__Macro_H__
#define	__Macro_H__
/*****************************************************************************
*	Include Header Files	
*****************************************************************************/
#include <stdio.h>
/****************************************************************************
*	Type Definition	
*****************************************************************************/
/* Description of char */
typedef char INT1;

/* Description of unsigned char */
typedef unsigned char UINT1;

/* Description of unsigned short */
typedef unsigned short UINT2;

/* Description of unsigned long */
typedef unsigned long UINT4;

/******************************************************************************
*	Macro Definition	
******************************************************************************/
/* Complete description of what this macro does */
#define MC_GET_CHAR(__data__)  (*((char *)(__data__)))                       //强制转换为char型

#define MC_GET_SHORT(__data__) ( ((__data__)<<8) | (((__data__)>>8)&0x00ff) )//将unsigned short型变量的高8位和低8位交换

//将long型变量的4个字节逆序
#define MC_GET_LONG(__data__) ( (((__data__)>>24) & 0x000000ff) | (((__data__)>>8) & 0x0000ff00) | (((__data__)<<8) & 0x00ff0000) | (((__data__)<<24) & 0xff000000) )

//获取1个字节中的3bit
#define MC_GET_3BN(__data__)  ( ( ((UINT4)MC_GET_CHAR((__data__)))<<16 )|( ((UINT4)MC_GET_SHORT(__data__+1))&0x0000ffff ) )

//获取Reserved
#define MC_GET_RESERVED(__data__) ( ((__data__) & 0xffffff00) >> 8 )

//获取flag变量
#define MC_GET_FLAG(__data__)     ( (( (__data__) & 0x000000ff ) & 128) >> 7 )

//获取岔路数
#define MC_GET_BRUNCH(__data__)   ( (((__data__) & 0x000000ff) & 112) >> 4)

//获取番号
#define MC_GET_CLASS(__data__)    ( ((__data__) & 0x000000ff) & 15 )

/*****************************************************************************
*	Function Prototype Declaration 	
*****************************************************************************/
//利用reserved、flag、brunch和class获取Link情报
int MC_MARK_INFO(int reserved, int falg, int brunch, int Class);

//获取道路名称
void MC_GET_NAME(FILE *fp, char *name, int len);

//将道路名称写入内存
void MC_MARK_NAME(FILE *fpn, char *name, int len);

#endif /*__Macro_H__*/
