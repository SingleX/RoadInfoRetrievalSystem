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
void TitleWelcomeInterface();    //��ӭ�������
void TitleThanksInterface();     //�˳��������

void MainMenuInterface();	     //���˵�������ʾ
void SearchMenuInterface();      //����������ʾ
void UpdateInterface();          //���ݸ��½���
void ExitInterface();            //�˳�����

void SearchLinkIDInterface();    //��LinkID��������
void SearchDispclassInterface(); //�����ż�������
void SearchBrunchInterface();    //����·����������
void SearchRoadNameInterface();  //����·���Ƽ�������
void CreatFileOKInterface();     //�ļ�д��ɹ���ʾ����

void UpdateErrorInterface();     //����ʧ�ܽ���

void SearchFailedInterface();    //����ʧ�ܽ���
void SearchErrorInterface();     //�����ؼ��ʴ������

void ReadFileErrorInterface();   //��ȡ�ļ�ʧ�ܽ���
void WriteFileErrorInterface();  //д���ļ�ʧ�ܽ���

#endif /*__interface_H__*/

