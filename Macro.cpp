/*******************************************************************************
*	Copyright (c) 2002-? Neusoft Co., Ltd. All Rights Reserved.
*	FileName		:	Macro.cpp
*	Date			:	2012.01.13
*	Author			:	zhengchuang
*	Module			:	Data analysis
*	Function		:	Data analysis
*-------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

/*****************************************************************************
*	Include Header Files	
*****************************************************************************/
#include "Macro.h"
#include <stdio.h>

/*****************************************************************************
*	Function Definition
*****************************************************************************/

/*******************************************************************************
*	Function Name	: MC_GET_NAME
*	Description		: Get real road name from file
*	Arguments		: (in)     fpr : File pointer
*					  (in/out) name: Save real road name 
*					  (in)     len : Road name size
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void MC_GET_NAME(FILE *fpr, char *name, int len)//按指定长度读取道路名
{
	int i = 0;
	for(i = 0; i < len + 2; i++) //循环获取roadname
	{
		fscanf(fpr, "%c", name + i);
	}
	name[i] = 0;
}

/*******************************************************************************
*	Function Name	: MC_MARK_INFO
*	Description		: Write road name into structure
*	Arguments		: (in) reserved : Reserved
*					  (in) flag     : Mark road name exist or not
*					  (in) brunch   : Brunch num
*					  (in) Class    : Class num
* 	Return Value	: info : Road information
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
int MC_MARK_INFO(int reserved, int flag, int brunch, int Class)
{
	int info = 0;
	info |= reserved;
	info <<= 24;
	info |= (flag << 7);
	info |= (brunch << 4);
	info |= (Class);
	info = MC_GET_LONG(info);
	return info;
}

/*******************************************************************************
*	Function Name	: MC_MARK_NAME
*	Description		: Get real road name from structure
*	Arguments		: (in)     fpr : File pointer
*					  (in/out) name: Save real road name 
*					  (in)     len : Road name size
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void MC_MARK_NAME(FILE *fpw, char *name, int len)
{
	int i = 0;
	for(i = 0; i < len + 2; i++)
	{
		fprintf(fpw, "%c", name[i]);
	}
}