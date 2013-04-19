/*******************************************************************************
*	Copyright (c) 2002-? Neusoft Co., Ltd. All Rights Reserved.
*	FileName		:	interface.h
*	Date			:	2012.01.13
*	Author			:	zhengchuang
*	Module			:	Interface
*	Function		:	Interface display
*-------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

#ifndef	__interface_H__
#define	__interface_H__

/*****************************************************************************
*	Function Prototype Declaration 	
*****************************************************************************/
void TitleWelcomeInterface();    //欢迎界面标题
void TitleThanksInterface();     //退出界面标题

void MainMenuInterface();	     //主菜单界面显示
void SearchMenuInterface();      //检索界面显示
void UpdateInterface();          //数据更新界面
void ExitInterface();            //退出界面

void SearchLinkIDInterface();    //按LinkID检索界面
void SearchDispclassInterface(); //按番号检索界面
void SearchBrunchInterface();    //按岔路数检索界面
void SearchRoadNameInterface();  //按道路名称检索界面
void CreatFileOKInterface();     //文件写入成功提示界面

void UpdateErrorInterface();     //更新失败界面

void SearchFailedInterface();    //检索失败界面
void SearchErrorInterface();     //检索关键词错误界面

void ReadFileErrorInterface();   //读取文件失败界面
void WriteFileErrorInterface();  //写入文件失败界面

#endif /*__interface_H__*/

