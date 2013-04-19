/*******************************************************************************
*	Copyright (c) 2002-? Neusoft Co., Ltd. All Rights Reserved.
*	FileName		:	interface.cpp
*	Date			:	2012.01.13
*	Author			:	GaoXin
*	Module			:	Interface display
*	Function		:	Interface display
*-------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

/*****************************************************************************
*	Include Header Files	
*****************************************************************************/
#include <stdio.h>
#include <windows.h>

/*****************************************************************************
*	Function Definition
*****************************************************************************/

/*******************************************************************************
*	Function Name	: TitleWelcomeInterface
*	Description		: TitleWelcomeInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void TitleWelcomeInterface()
{
	system("cls");
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n");
	printf("                              欢迎使用北斗导航系统!                             \n");
	printf("                     Welcome to Beidou's navigation system!                     \n");
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n");
}

/*******************************************************************************
*	Function Name	: TitleThanksInterface
*	Description		: TitleThanksInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void TitleThanksInterface()
{
	system("cls");
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n");
	printf("           感谢您使用中国北斗导航系统!此次服务到此结束，祝您旅途愉快!           \n");
	printf("                             Thank you!  Good Luck!                             \n");
	printf("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n");
}

/*******************************************************************************
*	Function Name	: SearchLinkIDInterface
*	Description		: SearchLinkIDInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchLinkIDInterface()
{
	system("cls");
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※    请输入要查询的道路标号信息!   ※\n");
	printf("                     ※       Please Input LinkID!       ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchDispclassInterface
*	Description		: SearchDispclassInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchDispclassInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※    请输入要查询的道路番号信息!   ※\n");
	printf("                     ※       Please Input Dispclass!    ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchBrunchInterface
*	Description		: SearchBrunchInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchBrunchInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※    请输入要查询的道路岔口信息!   ※\n");
	printf("                     ※       Please Input Brunch!       ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchRoadNameInterface
*	Description		: SearchRoadNameInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchRoadNameInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※    请输入要查询的道路名称信息!   ※\n");
	printf("                     ※       Please Input RoadName!     ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: CreatFileOKInterface
*	Description		: CreatFileOKInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void CreatFileOKInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※ 生成文件成功!请到程序目录下查看! ※\n");
	printf("                     ※      Creat File Successfully!    ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchFailedInterface
*	Description		: SearchFailedInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchFailedInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※      查询失败! 指定信息不存在!   ※\n");
	printf("                     ※           Search Failed!         ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: ReadFileErrorInterface
*	Description		: ReadFileErrorInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void ReadFileErrorInterface()
{
	TitleThanksInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	printf("                      ※                                  ※\n");
	printf("                      ※        未找到数据文件!           ※\n");
	printf("                      ※        File Read Error!          ※\n");
	printf("                      ※                                  ※\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: WriteFileErrorInterface
*	Description		: WriteFileErrorInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void WriteFileErrorInterface()
{
	TitleThanksInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	printf("                      ※                                  ※\n");
	printf("                      ※        数据文件创建失败!         ※\n");
	printf("                      ※        File Write Error!         ※\n");
	printf("                      ※                                  ※\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
	system("pause");
}

/*******************************************************************************
*	Function Name	: MainMenuInterface
*	Description		: MainMenuInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void MainMenuInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\t\t\t  ◆ [1] 排序输出\n");
	printf("\n");
	printf("\t\t\t  ◆ [2] 信息检索\n");
	printf("\n");
	printf("\t\t\t  ◆ [3] 数据更新\n");
	printf("\n");
	printf("\t\t\t  ◆ [4] 退出程序\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchMenuInterface
*	Description		: SearchMenuInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchMenuInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\t\t\t  ◆ [1] 根据  LinkID     检索\n");
	printf("\n");
	printf("\t\t\t  ◆ [2] 根据  Class番号  检索\n");
	printf("\n");
	printf("\t\t\t  ◆ [3] 根据  岔路数 N   检索\n");
	printf("\n");
	printf("\t\t\t  ◆ [4] 根据  道路名称   检索\n");
	printf("\n");
	printf("\t\t\t  ◆ [5] 返回主菜单\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: ExitInterface
*	Description		: ExitInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void ExitInterface()
{
	TitleThanksInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                    ※※※※※※※※※※※※※※※※※※※\n");
	printf("                    ※                                  ※\n");
	printf("                    ※             退出成功!            ※\n");
	printf("                    ※          Exit Sucessfully!       ※\n");
	printf("                    ※                                  ※\n");
	printf("                    ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: SearchErrorInterface
*	Description		: SearchErrorInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void SearchErrorInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※      输入数据有误，请重新输入!   ※\n");
	printf("                     ※           Input Error!           ※\n");
	printf("                     ※                                  ※\n");
	printf("                     ※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: UpdateInterface
*	Description		: UpdateInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void UpdateInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                   ※※※※※※※※※※※※※※※※※※※※※\n");
	printf("                   ※                                      ※\n");
	printf("                   ※ 请输入Link情报更新文件的路径和名称!  ※\n");
	printf("                   ※     Please Input Link File Info!     ※\n");
	printf("                   ※                                      ※\n");
	printf("                   ※※※※※※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}

/*******************************************************************************
*	Function Name	: UpdateErrorInterface
*	Description		: UpdateErrorInterface display
*	Arguments		: void
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			:
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
void UpdateErrorInterface()
{
	TitleWelcomeInterface();
	printf("\n");
	printf("\n");
	printf("\n");
	printf("                        ※※※※※※※※※※※※※※※※\n");
	printf("                        ※                            ※\n");
	printf("                        ※ 你妹啊!数据库满啦! 不干啦! ※\n");
	printf("                        ※     Database Overflow!     ※\n");
	printf("                        ※                            ※\n");
	printf("                        ※※※※※※※※※※※※※※※※\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
}
