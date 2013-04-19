/*****************************************************************************
*	Copyright (c) 2002-2012 Neusoft Co., Ltd. All Rights Reserved.
*	FileName		:	GPS.CPP
*	Date			:	2012.01.09
*	Author			:	ZhengChuang; GaoXin; LiPengwei; LiuLei
*	Module			:	GPS navi
*	Function		:	1) Output the infomation of the data in order
*                       2) Searching Information
*						   a. Search with LinkID
*						   b. Search with Dispclass
*						   c. Search with Brunch
*						   d. Search with RoadName
*						3) Update
*-----------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
******************************************************************************/

/******************************************************************************
*	Include Header Files	
******************************************************************************/
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "Macro.h"
#include "interface.h"

/*****************************************************************************
*	Constant Definition    	
*****************************************************************************/
#define MAX 66000			//定义道路条数最大值
#define SORT 1				//排序标示符
#define SEARCH 2			//搜索标示符
#define UPDATA 3			//更新标示符
#define ESC 27				//退出标示符, 27为ESC的ASCII值
#define ROADNAMESIZE 50		//道路名称的最大长度
#define FILENAMESIZE 25		//文件名称的最大长度

/*****************************************************************************
*	Memory Align Fashion
*****************************************************************************/
#pragma pack(1)				//内存对齐方式

/****************************************************************************
*	Type Definition	
*****************************************************************************/
struct tagNodeTmp			//按给定的字节大小读取出数据文件的信息
{
	UINT2 nodesize : 16;	//记录大小
	UINT4 LinkID   : 32;	//道路LinkID
	UINT2 namesize : 16;	//道路名大小
	UINT4 Reserved : 32;	//始点Node的情报
}st_FileData;

struct tagLink   
{
	UINT2 nodesize;			//记录大小
	UINT4 linkid;			//道路LinkID
	UINT2 namesize;			//道路名大小
	UINT2 reserved;			//Reserved
	UINT2 flag;				//路线名称有无Flag
	UINT2 brunch;			//岔路数
	UINT2 Class;			//交叉Link列表示Class番号
	char name[ROADNAMESIZE];//道路名称字符串
}st_Link[MAX];				//储存在内存中供实际排序、查找操作的结构体数组

char *	paCalcTime[] = {"", "排序", "检索", "更新"};//计算时间的标示符，供荧屏显示

HANDLE hOut = NULL;			//初始化控制句柄为空

/******************************************************************************
*	函数声明：因函数定义顺序问题，需将WriteFile函数提前声明	
******************************************************************************/
int WriteFile(const int inum, long time, int sign, char *outputfilename);	//读取文件函数

void Gotoxy(int x, int y);

/*****************************************************************************
*	Function Definition
*****************************************************************************/

/*******************************************************************************
*	Function Name	: SearchResultOutput
*	Description		: Output the search result information
*	Arguments		: (in) iFileNum: Record searching times
*					  (in) dataoutput: Data written to file
* 	Return Value	: 1) Write file error: return 0
*					  2) Others          : return 1
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int SearchResultOutput(int iFileNum, struct tagLink dataoutput) //iFileNum是检索次数的记录
{
	FILE *fp = NULL;								//文件指针初始化为空
	int i = 0;
	char acFileName[FILENAMESIZE] = "searchresult"; //文件名前缀部分
	char acFileNum[4] = {0};						//文件名后缀部分
	sprintf(acFileNum, "%03d", iFileNum);			//将iFileNum转换为字符串类型
	strcat(acFileName, acFileNum);					//追加名称
	strcat(acFileName, ".txt");						//追加名称
	if ( rename(acFileName, acFileName) )			//借rename函数的返回值判断以acFileName为文件名的文件是否存在
	{
		if ( (fp = fopen(acFileName, "w+")) == NULL )//若文件不存在
		{
			WriteFileErrorInterface();				//出错提示
			return 0;								//返回
		}	
	}
	else if ( (fp = fopen(acFileName, "a+")) == NULL )//若文件存在则追加
	{
		WriteFileErrorInterface();					//出错提示
		return 0;									//返回
	}
	fprintf(fp, "#linked=%d;roadnameflag=%d;brunch=%d;dispclass=%d", dataoutput.linkid, dataoutput.flag, dataoutput.brunch, dataoutput.Class);	//将Search结果写入文件
	if (dataoutput.flag == 1)						//如果名称存在，则输出道路名称
	{
		fprintf(fp, ";roadname=");					//先输出固定格式
		for (i = 4; i < dataoutput.namesize; i++)	//忽略前面的“1=”，从第4个开始输出
		{
			fprintf(fp, "%c", dataoutput.name[i]);	//输出道路名称
		}
	}
	fprintf(fp, "#\n");								//保持输出格式
	fclose(fp);										//关闭文件
	fp = NULL;										//指针置空
	return 1;
}

/*******************************************************************************
*	Function Name	: SearchResultInput
*	Description		: input the search result information
*	Arguments		: (in) iFileNum: Record searching times
*					  (in) pSearchKey: Search key
*					  (in) pSearchName: Search name
* 	Return Value	: 1) Write file error: return 0
*					  2) Others          : return 1
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int SearchResultInput(int iFileNum, char *pSearchKey, char *pSearchName)  //iFileNum:文件名后缀, pSearchKey:检索关键词, pSearchName:检索条件的名称
{
	FILE *fp = NULL;								//文件指针初始化为空
	char c = 0;										//从文件中读取的每个字节
	char acFileNum[4] = {0};						//文件名后缀部分
	char acFileName[FILENAMESIZE] = "searchresult"; //文件名前缀部分
	sprintf(acFileNum, "%03d", iFileNum);			//将整型数据转换为字符型数据
	strcat(acFileName, acFileNum);					//追加名称
	strcat(acFileName, ".txt");						//追加名称
	if ( (fp = fopen(acFileName, "r")) == NULL )    //文件打开为空
	{
		WriteFileErrorInterface();					//出错提示
		return 0;									//返回
	}
	TitleWelcomeInterface();						//显示欢迎标题界面
	printf("\n");
	printf("\n");
	printf("\n");
	printf("    查询成功! 您查询的道路%s为“%s”的道路详细信息为：\n",pSearchName, pSearchKey);//给出查询成功提示
	printf("\n");
	while (fscanf(fp, "%c", &c) != EOF)				//以字节为单位从文件中读取数据
	{
		printf("%c", c);							//输出查询结果
	}
	Gotoxy(0, 22);									//定位光标位置到坐标(0, 22)
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
	fclose(fp);										//关闭文件
	fp = NULL;										//指针置空
	return 1;
}

/*******************************************************************************
*	Function Name	: SearchLinkID
*	Description		: Search with LinkID
*	Arguments		: (in) inum: Max data records
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void SearchLinkID(const int inum)
{
	int i = 0;
	int j = 0;
	int iLinkidkey = 0;								// iLinkidkey 存储用户输入的道路LinkID标号
	fflush(stdin);									//清空缓存
	SearchLinkIDInterface();						//界面显示
	printf("LinkID = ");
	scanf("%d", &iLinkidkey);						
	while (iLinkidkey < 1 || iLinkidkey > inum)		//若输入的LinkID不合法则一直循环
	{
		SearchErrorInterface();						//错误提示
		printf("LinkID = ");
		if (scanf("%d", &iLinkidkey) == 0)
		{
			fflush(stdin);
			SearchErrorInterface();					//错误提示
		}
	}
	//以LinkID为关键字检索
	for(i = 0; i < inum; i++)
	{
		if (st_Link[i].linkid == iLinkidkey)		//若检索到关键词
		{
			TitleWelcomeInterface();				//显示欢迎标题界面
			printf("\n");
			printf("\n");
			printf("\n");
			printf("    查询成功! 您查询的道路标号为“%d”的道路详细信息为：\n", iLinkidkey);	//给出查询成功提示
			printf("\n");
			printf("    #linkid=%d;roadnameflag=%d;brunch=%d;dispclass=%d", st_Link[i].linkid, st_Link[i].flag, st_Link[i].brunch, st_Link[i].Class);	//输出查询结果
			if (st_Link[i].flag == 1)				//如果道路名称存在
			{
				printf(";roadname=");				//输出固定格式
				for (j=4; j<st_Link[i].namesize; j++)//忽略前面的“1=”，从第4个开始输出
				{
					printf("%c", st_Link[i].name[j]);//显示道路名称
				}
			}
			printf("#\n");							//输出固定格式
			Gotoxy(0, 22);							//定位光标位置到坐标(0, 22)
			printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
			break;
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//查询进度指示
	}
	system("pause");								//程序暂停
}

/*******************************************************************************
*	Function Name	: SearchDispclass
*	Description		: Search with Dispclass
*	Arguments		: (in) inum: Max data records
*                     (in) iCountFileNum: Count file number
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: GaoXin
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void SearchDispclass(const int inum, int *iCountFileNum)
{
	int i = 0;
	int j = 0;
	int iCountRecordNum = 0;						//标记道路信息记录数目
	int iDispclasskey = 0;							//iDispclasskey 存储用户输入的道路Class番号
	char acDispclassName[6] = "番号";				//定义显示的查询项目名称
	fflush(stdin);									//清空缓存
	SearchDispclassInterface();						//打印界面
	printf("Dispclass = ");							//提示用户输入
	scanf("%d", &iDispclasskey);					//读入信息
	while (iDispclasskey < 1 || iDispclasskey >= 16)//若数据不合法则一直循环
	{
		SearchErrorInterface();						//错误提示页面
		printf("Dispclass = ");						//提示用户输入
		if (scanf("%d", &iDispclasskey) == 0)
		{
			fflush(stdin);							//清空缓存
			SearchErrorInterface();					//错误提示页面
		}
	}
	char acFileName[FILENAMESIZE] = "searchresult";	//文件名前缀部分
	char acFileNum[4] = {0};						//文件名后缀部分
	sprintf(acFileNum, "%03d", *iCountFileNum);		//将整型数据转换为字符型数据
	strcat(acFileName, acFileNum);					//追加名称
	strcat(acFileName, ".txt");						//追加名称
	remove(acFileName);								//删除重名文件
	for (i = 0; i < inum; i++)
	{
		if (st_Link[i].Class == iDispclasskey)		//如果检索关键词成功
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);//不管记录数有多少，全部输出数据到文件
			iCountRecordNum++;						//每有一条信息成功匹配，则计数器+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//查询进度指示
	}
	if (iCountRecordNum == 0)						//如果记录值为0
	{
		SearchFailedInterface();					//错误提示：搜索失败
		system("pause");							//程序暂停
		return;
	}
	if (iCountRecordNum <= 5)						//若记录数<=5，则调出记录，输出到控制台
	{  
		char acTmpData[10] = {0};					//临时数据存储初始化
		sprintf(acTmpData, "%d", iDispclasskey);	//将int型转化为字符串存于acTmpData数组
		SearchResultInput(*iCountFileNum, acTmpData, acDispclassName);  
		remove(acFileName);							//删除存在的文件
		(*iCountFileNum)--;							//计数器回退1
	}
	else
	{												
		CreatFileOKInterface();						//否则直接提示创建文件成功
	}
	(*iCountFileNum)++;								//文件数目+1
	system("pause");								//程序暂停
}

/*******************************************************************************
*	Function Name	: SearchBrunch
*	Description		: Search with Brunch
*	Arguments		: (in) inum: Max data records
*                     (in) iCountFileNum: Count file number
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: LiPengwei
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void SearchBrunch(const int inum, int *iCountFileNum)
{
	int iCountRecordNum = 0;						//标记道路信息记录数
	int iBrunchkey = 0;								//定义岔路口变量
	char acBrunchName[6] = "岔口";					//定义显示的查询项目名称
	fflush(stdin);									//清空缓存
	SearchBrunchInterface();						//打印界面
	printf("Brunch = ");							//提示用户输入
	scanf("%d", &iBrunchkey);						//读入信息
	while (iBrunchkey < 1 || iBrunchkey >= 8)		//若数据不合法则一直循环
	{
		fflush(stdin);								//清空缓存
		SearchErrorInterface();						//错误提示
        printf("Brunch = ");						//提示用户输入
	    if (scanf("%d", &iBrunchkey) == 0)			//检验数据的合法性
		{
			fflush(stdin);							//清空缓存
			SearchErrorInterface();					//错误提示
		}
	}
	char acFileName[FILENAMESIZE] = "searchresult";	//文件名前缀部分
	char acFileNum[4] = {0};						//文件名后缀部分
	sprintf(acFileNum, "%03d", *iCountFileNum);		//将整型数据转换为字符型数据
	strcat(acFileName, acFileNum);					//追加名称
	strcat(acFileName, ".txt");						//追加名称
	remove(acFileName);								//删除重名文件
	for (int i = 0;i <= inum;i++)
	{
		if (iBrunchkey < st_Link[i].brunch)			//如果检索关键词成功
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);  //不管记录数有多少，全部输出数据到文件
			iCountRecordNum++;						//每有一条信息成功匹配，则计数器+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//查询进度指示
	}
	if (iCountRecordNum == 0)						//如果记录值为0
	{
		SearchFailedInterface();					//错误提示：搜索失败
		system("pause");							//程序暂停
		return;
	}
	if (iCountRecordNum <= 5)						//若记录数<=5，则调出记录，输出到控制台
	{
		char acTmpData[10] = {0};					//临时数据存储初始化
		sprintf(acTmpData, "%d", iBrunchkey);		//将int型数据转化为字符型
		SearchResultInput(*iCountFileNum, acTmpData, acBrunchName);  //若记录数<=5，则调出记录，输出到控制台
		remove(acFileName);							//删除存在的文件
		(*iCountFileNum)--;							//检索次数减1
	}
	else
	{
		CreatFileOKInterface();						//若记录数>5，则提示用户到文件中查看
	}
	(*iCountFileNum)++;								//文件数目+1
	system("pause");								//程序暂停
}

/*******************************************************************************
*	Function Name	: SearchRoadName
*	Description		: Search with Roadname
*	Arguments		: (in) inum: Max data records
*                     (in) iCountFileNum: Count file number
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: LiuLei
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void SearchRoadName(const int inum, int *iCountFileNum)
{
	fflush(stdin);									//清空缓存
	int i = 0;
	int j = 0;              
	int iCountRecordNum = 0;						//标记道路信息记录数目
	char acRoadnamekey[ROADNAMESIZE] = {0};			//道路名称关键词
	char acFileName[FILENAMESIZE] = "searchresult";	//文件名前缀部分
	char acFileNum[4] = {0};						//文件名后缀部分
	sprintf(acFileNum, "%03d", *iCountFileNum);		//将整型数据转换为字符型数据
	strcat(acFileName, acFileNum);					//追加名称
	strcat(acFileName, ".txt");						//追加名称
	remove(acFileName);								//删除重名文件
	SearchRoadNameInterface();						//打印界面
	printf("Roadname Key = ");						//提示用户输入
	while (gets(acRoadnamekey))
	{
		if (strlen(acRoadnamekey))					//检验数据合法性
		{
			break;									//若合法, 跳出,继续执行下面代码
		}
	}	
	for (i = 0;i <= inum;i++)
	{
		if (strstr(st_Link[i].name, acRoadnamekey)) //以acRoadnamekey为关键字检索
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);  //不管记录数有多少，全部输出数据到文件
			iCountRecordNum++;						//每有一条信息成功匹配，则计数器+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//查询进度指示
	}
	if (iCountRecordNum == 0)						//如果记录值为0
	{
		SearchFailedInterface();					//错误提示界面
		system("pause");							//程序暂停
		return;
	}
	if (iCountRecordNum <= 5)						//若记录数<=5
	{
		SearchResultInput(*iCountFileNum, acRoadnamekey, "名称");  //调出记录，输出到控制台
		remove(acFileName);							//删除文件
		(*iCountFileNum)--;							//文件名称计数器-1
	}
	else
	{
		CreatFileOKInterface();						//否则直接提示创建文件成功
	}
	(*iCountFileNum)++;								//文件数目+1
	system("pause");								//程序暂停
}

/*******************************************************************************
*	Function Name	: cmp
*	Description		: Quick sort
*	Arguments		: (in) a: Sorted elements
*                     (in) b: Sorted elements
* 	Return Value	: Sorted data
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int cmp(const void *a, const void *b)				//快排函数
{
	struct tagLink *c = (struct tagLink *)a;
	struct tagLink *d = (struct tagLink *)b;
	return c->linkid - d->linkid;
}

/*******************************************************************************
*	Function Name	: InorderOutput
*	Description		: Output data inorder
*	Arguments		: (in) inum: Max data records
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void InorderOutput(const int inum)
{
	long st = 0;									//排序开始时间
	long end = 0;									//排序结束时间
	st = clock();									//获取系统当前时间
	qsort(st_Link, inum, sizeof(st_Link[0]), cmp);	//对数据排序
	end = clock();									//获取系统当前时间
	WriteFile(inum, end-st, SORT, "NEW_GTBL.dat");	//将排序结果写入新二进制文件
	return;
}

/*******************************************************************************
*	Function Name	: SearchMenu
*	Description		: Display search menu
*	Arguments		: (in) inum: Max data records
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
//函数功能：检索功能菜单
void SearchMenu(const int inum)
{
	char choose = 0;								//用户对菜单的选择
	int iCountFileNum = 1;
	while (1)
	{
		SearchMenuInterface();						//显示搜索菜单
		choose = getch();							//获取用户对菜单的选择
		switch (choose)								//根据用户对菜单的选择调用特定函数
		{
		case '1' : SearchLinkID(inum);                    break;//按LinkID搜索
		case '2' : SearchDispclass(inum, &iCountFileNum); break;//按Dispclass搜索
		case '3' : SearchBrunch(inum, &iCountFileNum);    break;//按Brunch搜索
		case '4' : SearchRoadName(inum, &iCountFileNum);  break;//按RoadName搜索
		case '5' : 
		case ESC : return; break;								//中断
		default:break;											//中断
		}
	}
}

/*******************************************************************************
*	Function Name	: Update
*	Description		: Update data information
*	Arguments		: (in) inum: Max data records
* 	Return Value	: Read file error: return 0
*					  Update error   : return 0
*					  Other          : return 1
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int Update(int *inum)
{
	int i = 0;
	int j = 0;
	int iCountInfoNum = 0;							//计数器功能，存储顺序读取到的第iCountInfoNum条信息
	FILE *fp = NULL;								//文件指针初始化为空
	char acTmpData[ROADNAMESIZE] = {0};				//从Link情报文件中读取的临时数据
	char linkinfofilename[50] = {0};				//初始化linkinfo文件名称
	struct tagLink linkinfo = {0};					//从Link情报文件中读取的数据经转换后存入linkinfo
	fflush(stdin);									//清空缓存
	UpdateInterface();								//数据更新界面显示
	printf("File Path = ");							//提示用户输入
	while (gets(linkinfofilename))					//数据检验
	{
		if (strlen(linkinfofilename))				//检验数据合法性
		{
			break;
		}
	}
	if ( (fp = fopen(linkinfofilename, "r")) == NULL )//若文件不存在
	{
		ReadFileErrorInterface();					//错误提示
		system("pause");							//程序暂停
		return 0;
	}
	while (1)										//循环读取Link情报文件中的数据
	{
		memset (&linkinfo, 0, sizeof(linkinfo) );	//初始化linkinfo
		//读取LinkID
		i = 0;                    
		fseek(fp, 8, 1);							//定义文件指针位置
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//读取临时数据文件中的字符
			if (acTmpData[i] == ';')				//遇到“;”
			{
				break;								//中断跳出
			}
			++i;
		}
		acTmpData[i] = 0;							//置空
		linkinfo.linkid = atoi(acTmpData);			//将字符型数据转换成整型数据
		//读取Flag
		i = 0;                 
		fseek(fp, 13, 1);							//定义文件指针位置
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//读取临时数据文件中的字符
			if (acTmpData[i] == ';')				//遇到“;”
			{
				break;								//中断跳出
			}
			++i;
		}
		acTmpData[i] = 0;							//置空
		linkinfo.flag = atoi(acTmpData);			//将字符型数据转换成整型数据
		//读取岔路数
		i = 0;               
		fseek(fp, 7, 1);							//定义文件指针位置
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//读取临时数据文件中的字符
			if (acTmpData[i] == ';')				//遇到“;”
			{
				break;								//中断跳出
			}
			++i;
		}
		acTmpData[i] = 0;							//置空
		linkinfo.brunch = atoi(acTmpData);			//将字符型数据转换成整型数据
		//读取番号
		i = 0;             
		fseek(fp, 10, 1);							//定义文件指针位置
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//读取临时数据文件中的字符
			if (acTmpData[i] == ';' || acTmpData[i] == '#')//遇到“;”或“#”
			{
				break;								//中断跳出
			}
			++i;
		}
		acTmpData[i] = 0;							//置空
		linkinfo.Class = atoi(acTmpData);			//将字符型数据转换成整型数据
		//读取道路名称
		if (linkinfo.flag)							//如果flag为真
		{
			strcpy(acTmpData, "１＝");				//道路名称的前缀
			i = 4;
			fseek(fp, 9, 1);						//定义文件指针位置
			while (1)
			{
				fscanf(fp, "%c", &acTmpData[i]);	//读取临时数据文件中的字符
				if (acTmpData[i] == '#')			//遇到“#”
				{
					break;							//中断跳出
				}
				++i;
			}
			acTmpData[i] = 0;						//置空
			strcpy(linkinfo.name, acTmpData);		//将临时存储的name字符串复制到linkinfo.name
			linkinfo.namesize = strlen(linkinfo.name);//计算道路名称大小
		}
		else
		{
			linkinfo.namesize = 0;					//将namesize置为0
		}
		linkinfo.reserved = 0;
		linkinfo.nodesize = 12 + strlen(linkinfo.name);
		for (i = 0; i < (*inum); i++)
		{
			if (linkinfo.linkid == st_Link[i].linkid)
			{
				break;
			}
		}
		if (i >= (*inum))							//若超过现有记录最大值
		{
			if ((*inum) >= MAX)						//若超出最大限定值
			{
				UpdateErrorInterface();				//显示更新出错界面
				system("pause");					//程序暂停
				return 0;
			}
			st_Link[i] = linkinfo;
			(*inum)++;
		}
		else
		{
			st_Link[i] = linkinfo;
		}
		TitleWelcomeInterface();					//显示欢迎标题
		printf("\n");
		printf("\n");
		printf("\n");
		printf("    正在更新第%d条道路详细信息:\n", ++iCountInfoNum);//提示信息
		printf("\n");
		printf("    #linkid=%d;roadnameflag=%d;brunch=%d;dispclass=%d", st_Link[i].linkid, st_Link[i].flag, st_Link[i].brunch, st_Link[i].Class);
		if (st_Link[i].flag)						//显示道路名称
		{
			printf(";roadname=");
			for (j = 4; j < st_Link[i].namesize; j++)
			{
				printf("%c", st_Link[i].name[j]);
			}
		}
		printf("#\n");
		Gotoxy(0, 22);								//定位光标到此坐标处
		printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
		printf("系统提示: 第%d条信息更新成功, ", iCountInfoNum);
		system("pause");							//程序暂停
		if ( fscanf(fp, "%c", &acTmpData[0]) != 1)	//判断是否到文件结尾
		{
			break;
		}
	}
	fclose(fp);										//关闭文件
	fp = NULL;										//指针置空
	WriteFile((*inum), 0, -1, "GTBL.dat");			//将更新后的数据写入文件
	InorderOutput((*inum));							//最后进行排序
	return 1;										
}

/*******************************************************************************
*	Function Name	: ReadFile
*	Description		: Read data from file
*	Arguments		: (in) inum: Max data records
* 	Return Value	: Read file error: return 0
*					  Others         : return 1
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int ReadFile(int *inum)
{
	FILE *fpr = NULL;
	if ( (fpr = fopen("GTBL.dat", "rb")) == NULL )
	{
		Sleep(2000);								//初进系统若未找到数据文件，则令系统等待2秒显示主菜单后给错误提示
		ReadFileErrorInterface();
		system("pause");
		return 0;
	}
	memset (st_Link, 0, sizeof(st_Link));			//初始化结构体数组
	while (fread(&st_FileData, sizeof(st_FileData), 1, fpr))//循环读取文件数据
	{
		st_Link[*inum].nodesize = MC_GET_SHORT(st_FileData.nodesize);
		st_Link[*inum].linkid = MC_GET_LONG(st_FileData.LinkID);
		st_Link[*inum].namesize = MC_GET_SHORT(st_FileData.namesize);
		st_FileData.Reserved = MC_GET_LONG(st_FileData.Reserved);
		st_Link[*inum].reserved = MC_GET_RESERVED(st_FileData.Reserved);
		st_Link[*inum].flag = MC_GET_FLAG(st_FileData.Reserved);
		st_Link[*inum].brunch = MC_GET_BRUNCH(st_FileData.Reserved);
		st_Link[*inum].Class = MC_GET_CLASS(st_FileData.Reserved);

		if (st_Link[*inum].flag)					//如果flag不为零则读取道路名称
		{
			MC_GET_NAME(fpr, st_Link[*inum].name, st_Link[*inum].namesize);
		}
		(*inum)++;
	}
	fclose(fpr);									//关闭文件指针
	fpr = NULL;
	return 1;
}

/*******************************************************************************
*	Function Name	: WriteFile
*	Description		: Write data into file
*	Arguments		: (in) inum: Max data records
*					: (in) time: Calculate run time
*					: (in) sign: Mark the operate file
*					: (in) outputfilename: Output file name
* 	Return Value	: Write file error: return 0
*					  Others          : return 1
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int WriteFile(const int inum, long time, int sign, char *outputfilename)
{
	int i = 0;
	FILE *fpw = NULL;								//文件指针置空
	if ( (fpw = fopen(outputfilename, "wb+")) == NULL )
	{
		WriteFileErrorInterface();					//错误提示
		return 0;
	}
	TitleWelcomeInterface();						//显示欢迎标题
	if (sign == -1)
	{
		printf("\n");
	}
	else
	{
		printf("                      %s所花时间为%ld毫秒\n", paCalcTime[sign], time);//输出时间统计信息
	}
	printf("\n");
	printf("\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	printf("                      ※                                  ※\n");
	if (sign == -1)
	{
		printf("                      ※    数据文件GBTL.dat更新成功!     ※\n");
	}
	else
	{
		printf("                      ※  数据文件NEW_GBTL.dat更新成功!   ※\n");
	}
	printf("                      ※    File Updates Successfully!    ※\n");
	printf("                      ※     请到程序所在目录下查看       ※\n");
	printf("                      ※                                  ※\n");
	printf("                      ※※※※※※※※※※※※※※※※※※※\n");
	Gotoxy(0, 22);
	printf("※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※");
	system("pause");
	for (i = 0; i < inum; i++)						//循环写入数据文件
	{
		st_FileData.nodesize = MC_GET_SHORT(st_Link[i].nodesize);
		st_FileData.LinkID = MC_GET_LONG(st_Link[i].linkid);
		st_FileData.namesize = MC_GET_SHORT(st_Link[i].namesize);
		st_FileData.Reserved = MC_MARK_INFO(st_Link[i].reserved, st_Link[i].flag, st_Link[i].brunch, st_Link[i].Class);
		fwrite(&st_FileData, sizeof(st_FileData), 1, fpw);
		if (st_Link[i].flag)
		{
			MC_MARK_NAME(fpw, st_Link[i].name, st_Link[i].namesize);
		}
	}
	fclose(fpw);
	fpw = NULL;
	return 1;
}

/*******************************************************************************
*	Function Name	: MainMenu
*	Description		: Display main menu
*	Arguments		: 
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/
//函数功能：主菜单
int MainMenu()
{
	char choose = 0;	//用户对菜单的选择
	int iNodeNum = 0;	//统计道路总条数
	MainMenuInterface();//显示系统主菜单
	if ( ReadFile(&iNodeNum) == 0 )
	{
		return 0;
	}
	choose = getch();	//获取用户对菜单的选择
	switch (choose)		//根据用户对菜单的选择调用特定函数
	{
	case '1' : InorderOutput(iNodeNum); break;//排序输出
	case '2' : SearchMenu(iNodeNum);    break;//检索菜单
	case '3' : Update(&iNodeNum);       break;//更新数据
	case '4' : 
	case ESC :
		ExitInterface();
		return 0;                       break;//中断
	default:break;							  //中断
	}
	return 1;
}

/*******************************************************************************
*	Function Name	: main
*	Description		: Main function
*	Arguments		: 
* 	Return Value	: Run over , return 0
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

int main()
{
	system("color 2f");      //更改控制台颜色
	system("title GPS NAVI");//更改控制台标题
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	while (MainMenu());      //循环调用主菜单，直到用户自己退出
	return 0;
}

/*******************************************************************************
*	Function Name	: Gotoxy
*	Description		: Locate cursor into specific position
*	Arguments		: (in) x: horizontal ordinate
*					: (in) y: vertical coordinates
* 	Return Value	: void
* 	Date			: 2012.01.13
*	Author			: ZhengChuang
*	Remark			: 
*------------------------------------------------------------------------------
*	Modify History
*	NO	Date	 	Modifier		Modified Content
*   1	yyyy.mm.dd	Auther Name		Description
*******************************************************************************/

void Gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;	//横坐标
	pos.Y = y;	//纵坐标
	SetConsoleCursorPosition(hOut, pos);//设置光标位置
}