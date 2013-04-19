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
#define MAX 66000			//�����·�������ֵ
#define SORT 1				//�����ʾ��
#define SEARCH 2			//������ʾ��
#define UPDATA 3			//���±�ʾ��
#define ESC 27				//�˳���ʾ��, 27ΪESC��ASCIIֵ
#define ROADNAMESIZE 50		//��·���Ƶ���󳤶�
#define FILENAMESIZE 25		//�ļ����Ƶ���󳤶�

/*****************************************************************************
*	Memory Align Fashion
*****************************************************************************/
#pragma pack(1)				//�ڴ���뷽ʽ

/****************************************************************************
*	Type Definition	
*****************************************************************************/
struct tagNodeTmp			//���������ֽڴ�С��ȡ�������ļ�����Ϣ
{
	UINT2 nodesize : 16;	//��¼��С
	UINT4 LinkID   : 32;	//��·LinkID
	UINT2 namesize : 16;	//��·����С
	UINT4 Reserved : 32;	//ʼ��Node���鱨
}st_FileData;

struct tagLink   
{
	UINT2 nodesize;			//��¼��С
	UINT4 linkid;			//��·LinkID
	UINT2 namesize;			//��·����С
	UINT2 reserved;			//Reserved
	UINT2 flag;				//·����������Flag
	UINT2 brunch;			//��·��
	UINT2 Class;			//����Link�б�ʾClass����
	char name[ROADNAMESIZE];//��·�����ַ���
}st_Link[MAX];				//�������ڴ��й�ʵ�����򡢲��Ҳ����Ľṹ������

char *	paCalcTime[] = {"", "����", "����", "����"};//����ʱ��ı�ʾ������ӫ����ʾ

HANDLE hOut = NULL;			//��ʼ�����ƾ��Ϊ��

/******************************************************************************
*	������������������˳�����⣬�轫WriteFile������ǰ����	
******************************************************************************/
int WriteFile(const int inum, long time, int sign, char *outputfilename);	//��ȡ�ļ�����

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

int SearchResultOutput(int iFileNum, struct tagLink dataoutput) //iFileNum�Ǽ��������ļ�¼
{
	FILE *fp = NULL;								//�ļ�ָ���ʼ��Ϊ��
	int i = 0;
	char acFileName[FILENAMESIZE] = "searchresult"; //�ļ���ǰ׺����
	char acFileNum[4] = {0};						//�ļ�����׺����
	sprintf(acFileNum, "%03d", iFileNum);			//��iFileNumת��Ϊ�ַ�������
	strcat(acFileName, acFileNum);					//׷������
	strcat(acFileName, ".txt");						//׷������
	if ( rename(acFileName, acFileName) )			//��rename�����ķ���ֵ�ж���acFileNameΪ�ļ������ļ��Ƿ����
	{
		if ( (fp = fopen(acFileName, "w+")) == NULL )//���ļ�������
		{
			WriteFileErrorInterface();				//������ʾ
			return 0;								//����
		}	
	}
	else if ( (fp = fopen(acFileName, "a+")) == NULL )//���ļ�������׷��
	{
		WriteFileErrorInterface();					//������ʾ
		return 0;									//����
	}
	fprintf(fp, "#linked=%d;roadnameflag=%d;brunch=%d;dispclass=%d", dataoutput.linkid, dataoutput.flag, dataoutput.brunch, dataoutput.Class);	//��Search���д���ļ�
	if (dataoutput.flag == 1)						//������ƴ��ڣ��������·����
	{
		fprintf(fp, ";roadname=");					//������̶���ʽ
		for (i = 4; i < dataoutput.namesize; i++)	//����ǰ��ġ�1=�����ӵ�4����ʼ���
		{
			fprintf(fp, "%c", dataoutput.name[i]);	//�����·����
		}
	}
	fprintf(fp, "#\n");								//���������ʽ
	fclose(fp);										//�ر��ļ�
	fp = NULL;										//ָ���ÿ�
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

int SearchResultInput(int iFileNum, char *pSearchKey, char *pSearchName)  //iFileNum:�ļ�����׺, pSearchKey:�����ؼ���, pSearchName:��������������
{
	FILE *fp = NULL;								//�ļ�ָ���ʼ��Ϊ��
	char c = 0;										//���ļ��ж�ȡ��ÿ���ֽ�
	char acFileNum[4] = {0};						//�ļ�����׺����
	char acFileName[FILENAMESIZE] = "searchresult"; //�ļ���ǰ׺����
	sprintf(acFileNum, "%03d", iFileNum);			//����������ת��Ϊ�ַ�������
	strcat(acFileName, acFileNum);					//׷������
	strcat(acFileName, ".txt");						//׷������
	if ( (fp = fopen(acFileName, "r")) == NULL )    //�ļ���Ϊ��
	{
		WriteFileErrorInterface();					//������ʾ
		return 0;									//����
	}
	TitleWelcomeInterface();						//��ʾ��ӭ�������
	printf("\n");
	printf("\n");
	printf("\n");
	printf("    ��ѯ�ɹ�! ����ѯ�ĵ�·%sΪ��%s���ĵ�·��ϸ��ϢΪ��\n",pSearchName, pSearchKey);//������ѯ�ɹ���ʾ
	printf("\n");
	while (fscanf(fp, "%c", &c) != EOF)				//���ֽ�Ϊ��λ���ļ��ж�ȡ����
	{
		printf("%c", c);							//�����ѯ���
	}
	Gotoxy(0, 22);									//��λ���λ�õ�����(0, 22)
	printf("��������������������������������������������������������������������������������");
	fclose(fp);										//�ر��ļ�
	fp = NULL;										//ָ���ÿ�
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
	int iLinkidkey = 0;								// iLinkidkey �洢�û�����ĵ�·LinkID���
	fflush(stdin);									//��ջ���
	SearchLinkIDInterface();						//������ʾ
	printf("LinkID = ");
	scanf("%d", &iLinkidkey);						
	while (iLinkidkey < 1 || iLinkidkey > inum)		//�������LinkID���Ϸ���һֱѭ��
	{
		SearchErrorInterface();						//������ʾ
		printf("LinkID = ");
		if (scanf("%d", &iLinkidkey) == 0)
		{
			fflush(stdin);
			SearchErrorInterface();					//������ʾ
		}
	}
	//��LinkIDΪ�ؼ��ּ���
	for(i = 0; i < inum; i++)
	{
		if (st_Link[i].linkid == iLinkidkey)		//���������ؼ���
		{
			TitleWelcomeInterface();				//��ʾ��ӭ�������
			printf("\n");
			printf("\n");
			printf("\n");
			printf("    ��ѯ�ɹ�! ����ѯ�ĵ�·���Ϊ��%d���ĵ�·��ϸ��ϢΪ��\n", iLinkidkey);	//������ѯ�ɹ���ʾ
			printf("\n");
			printf("    #linkid=%d;roadnameflag=%d;brunch=%d;dispclass=%d", st_Link[i].linkid, st_Link[i].flag, st_Link[i].brunch, st_Link[i].Class);	//�����ѯ���
			if (st_Link[i].flag == 1)				//�����·���ƴ���
			{
				printf(";roadname=");				//����̶���ʽ
				for (j=4; j<st_Link[i].namesize; j++)//����ǰ��ġ�1=�����ӵ�4����ʼ���
				{
					printf("%c", st_Link[i].name[j]);//��ʾ��·����
				}
			}
			printf("#\n");							//����̶���ʽ
			Gotoxy(0, 22);							//��λ���λ�õ�����(0, 22)
			printf("��������������������������������������������������������������������������������");
			break;
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//��ѯ����ָʾ
	}
	system("pause");								//������ͣ
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
	int iCountRecordNum = 0;						//��ǵ�·��Ϣ��¼��Ŀ
	int iDispclasskey = 0;							//iDispclasskey �洢�û�����ĵ�·Class����
	char acDispclassName[6] = "����";				//������ʾ�Ĳ�ѯ��Ŀ����
	fflush(stdin);									//��ջ���
	SearchDispclassInterface();						//��ӡ����
	printf("Dispclass = ");							//��ʾ�û�����
	scanf("%d", &iDispclasskey);					//������Ϣ
	while (iDispclasskey < 1 || iDispclasskey >= 16)//�����ݲ��Ϸ���һֱѭ��
	{
		SearchErrorInterface();						//������ʾҳ��
		printf("Dispclass = ");						//��ʾ�û�����
		if (scanf("%d", &iDispclasskey) == 0)
		{
			fflush(stdin);							//��ջ���
			SearchErrorInterface();					//������ʾҳ��
		}
	}
	char acFileName[FILENAMESIZE] = "searchresult";	//�ļ���ǰ׺����
	char acFileNum[4] = {0};						//�ļ�����׺����
	sprintf(acFileNum, "%03d", *iCountFileNum);		//����������ת��Ϊ�ַ�������
	strcat(acFileName, acFileNum);					//׷������
	strcat(acFileName, ".txt");						//׷������
	remove(acFileName);								//ɾ�������ļ�
	for (i = 0; i < inum; i++)
	{
		if (st_Link[i].Class == iDispclasskey)		//��������ؼ��ʳɹ�
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);//���ܼ�¼���ж��٣�ȫ��������ݵ��ļ�
			iCountRecordNum++;						//ÿ��һ����Ϣ�ɹ�ƥ�䣬�������+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//��ѯ����ָʾ
	}
	if (iCountRecordNum == 0)						//�����¼ֵΪ0
	{
		SearchFailedInterface();					//������ʾ������ʧ��
		system("pause");							//������ͣ
		return;
	}
	if (iCountRecordNum <= 5)						//����¼��<=5���������¼�����������̨
	{  
		char acTmpData[10] = {0};					//��ʱ���ݴ洢��ʼ��
		sprintf(acTmpData, "%d", iDispclasskey);	//��int��ת��Ϊ�ַ�������acTmpData����
		SearchResultInput(*iCountFileNum, acTmpData, acDispclassName);  
		remove(acFileName);							//ɾ�����ڵ��ļ�
		(*iCountFileNum)--;							//����������1
	}
	else
	{												
		CreatFileOKInterface();						//����ֱ����ʾ�����ļ��ɹ�
	}
	(*iCountFileNum)++;								//�ļ���Ŀ+1
	system("pause");								//������ͣ
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
	int iCountRecordNum = 0;						//��ǵ�·��Ϣ��¼��
	int iBrunchkey = 0;								//�����·�ڱ���
	char acBrunchName[6] = "����";					//������ʾ�Ĳ�ѯ��Ŀ����
	fflush(stdin);									//��ջ���
	SearchBrunchInterface();						//��ӡ����
	printf("Brunch = ");							//��ʾ�û�����
	scanf("%d", &iBrunchkey);						//������Ϣ
	while (iBrunchkey < 1 || iBrunchkey >= 8)		//�����ݲ��Ϸ���һֱѭ��
	{
		fflush(stdin);								//��ջ���
		SearchErrorInterface();						//������ʾ
        printf("Brunch = ");						//��ʾ�û�����
	    if (scanf("%d", &iBrunchkey) == 0)			//�������ݵĺϷ���
		{
			fflush(stdin);							//��ջ���
			SearchErrorInterface();					//������ʾ
		}
	}
	char acFileName[FILENAMESIZE] = "searchresult";	//�ļ���ǰ׺����
	char acFileNum[4] = {0};						//�ļ�����׺����
	sprintf(acFileNum, "%03d", *iCountFileNum);		//����������ת��Ϊ�ַ�������
	strcat(acFileName, acFileNum);					//׷������
	strcat(acFileName, ".txt");						//׷������
	remove(acFileName);								//ɾ�������ļ�
	for (int i = 0;i <= inum;i++)
	{
		if (iBrunchkey < st_Link[i].brunch)			//��������ؼ��ʳɹ�
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);  //���ܼ�¼���ж��٣�ȫ��������ݵ��ļ�
			iCountRecordNum++;						//ÿ��һ����Ϣ�ɹ�ƥ�䣬�������+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//��ѯ����ָʾ
	}
	if (iCountRecordNum == 0)						//�����¼ֵΪ0
	{
		SearchFailedInterface();					//������ʾ������ʧ��
		system("pause");							//������ͣ
		return;
	}
	if (iCountRecordNum <= 5)						//����¼��<=5���������¼�����������̨
	{
		char acTmpData[10] = {0};					//��ʱ���ݴ洢��ʼ��
		sprintf(acTmpData, "%d", iBrunchkey);		//��int������ת��Ϊ�ַ���
		SearchResultInput(*iCountFileNum, acTmpData, acBrunchName);  //����¼��<=5���������¼�����������̨
		remove(acFileName);							//ɾ�����ڵ��ļ�
		(*iCountFileNum)--;							//����������1
	}
	else
	{
		CreatFileOKInterface();						//����¼��>5������ʾ�û����ļ��в鿴
	}
	(*iCountFileNum)++;								//�ļ���Ŀ+1
	system("pause");								//������ͣ
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
	fflush(stdin);									//��ջ���
	int i = 0;
	int j = 0;              
	int iCountRecordNum = 0;						//��ǵ�·��Ϣ��¼��Ŀ
	char acRoadnamekey[ROADNAMESIZE] = {0};			//��·���ƹؼ���
	char acFileName[FILENAMESIZE] = "searchresult";	//�ļ���ǰ׺����
	char acFileNum[4] = {0};						//�ļ�����׺����
	sprintf(acFileNum, "%03d", *iCountFileNum);		//����������ת��Ϊ�ַ�������
	strcat(acFileName, acFileNum);					//׷������
	strcat(acFileName, ".txt");						//׷������
	remove(acFileName);								//ɾ�������ļ�
	SearchRoadNameInterface();						//��ӡ����
	printf("Roadname Key = ");						//��ʾ�û�����
	while (gets(acRoadnamekey))
	{
		if (strlen(acRoadnamekey))					//�������ݺϷ���
		{
			break;									//���Ϸ�, ����,����ִ���������
		}
	}	
	for (i = 0;i <= inum;i++)
	{
		if (strstr(st_Link[i].name, acRoadnamekey)) //��acRoadnamekeyΪ�ؼ��ּ���
		{
			SearchResultOutput(*iCountFileNum, st_Link[i]);  //���ܼ�¼���ж��٣�ȫ��������ݵ��ļ�
			iCountRecordNum++;						//ÿ��һ����Ϣ�ɹ�ƥ�䣬�������+1
		}
		printf("%3d%%\b\b\b\b", i*100/inum);		//��ѯ����ָʾ
	}
	if (iCountRecordNum == 0)						//�����¼ֵΪ0
	{
		SearchFailedInterface();					//������ʾ����
		system("pause");							//������ͣ
		return;
	}
	if (iCountRecordNum <= 5)						//����¼��<=5
	{
		SearchResultInput(*iCountFileNum, acRoadnamekey, "����");  //������¼�����������̨
		remove(acFileName);							//ɾ���ļ�
		(*iCountFileNum)--;							//�ļ����Ƽ�����-1
	}
	else
	{
		CreatFileOKInterface();						//����ֱ����ʾ�����ļ��ɹ�
	}
	(*iCountFileNum)++;								//�ļ���Ŀ+1
	system("pause");								//������ͣ
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

int cmp(const void *a, const void *b)				//���ź���
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
	long st = 0;									//����ʼʱ��
	long end = 0;									//�������ʱ��
	st = clock();									//��ȡϵͳ��ǰʱ��
	qsort(st_Link, inum, sizeof(st_Link[0]), cmp);	//����������
	end = clock();									//��ȡϵͳ��ǰʱ��
	WriteFile(inum, end-st, SORT, "NEW_GTBL.dat");	//��������д���¶������ļ�
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
//�������ܣ��������ܲ˵�
void SearchMenu(const int inum)
{
	char choose = 0;								//�û��Բ˵���ѡ��
	int iCountFileNum = 1;
	while (1)
	{
		SearchMenuInterface();						//��ʾ�����˵�
		choose = getch();							//��ȡ�û��Բ˵���ѡ��
		switch (choose)								//�����û��Բ˵���ѡ������ض�����
		{
		case '1' : SearchLinkID(inum);                    break;//��LinkID����
		case '2' : SearchDispclass(inum, &iCountFileNum); break;//��Dispclass����
		case '3' : SearchBrunch(inum, &iCountFileNum);    break;//��Brunch����
		case '4' : SearchRoadName(inum, &iCountFileNum);  break;//��RoadName����
		case '5' : 
		case ESC : return; break;								//�ж�
		default:break;											//�ж�
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
	int iCountInfoNum = 0;							//���������ܣ��洢˳���ȡ���ĵ�iCountInfoNum����Ϣ
	FILE *fp = NULL;								//�ļ�ָ���ʼ��Ϊ��
	char acTmpData[ROADNAMESIZE] = {0};				//��Link�鱨�ļ��ж�ȡ����ʱ����
	char linkinfofilename[50] = {0};				//��ʼ��linkinfo�ļ�����
	struct tagLink linkinfo = {0};					//��Link�鱨�ļ��ж�ȡ�����ݾ�ת�������linkinfo
	fflush(stdin);									//��ջ���
	UpdateInterface();								//���ݸ��½�����ʾ
	printf("File Path = ");							//��ʾ�û�����
	while (gets(linkinfofilename))					//���ݼ���
	{
		if (strlen(linkinfofilename))				//�������ݺϷ���
		{
			break;
		}
	}
	if ( (fp = fopen(linkinfofilename, "r")) == NULL )//���ļ�������
	{
		ReadFileErrorInterface();					//������ʾ
		system("pause");							//������ͣ
		return 0;
	}
	while (1)										//ѭ����ȡLink�鱨�ļ��е�����
	{
		memset (&linkinfo, 0, sizeof(linkinfo) );	//��ʼ��linkinfo
		//��ȡLinkID
		i = 0;                    
		fseek(fp, 8, 1);							//�����ļ�ָ��λ��
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//��ȡ��ʱ�����ļ��е��ַ�
			if (acTmpData[i] == ';')				//������;��
			{
				break;								//�ж�����
			}
			++i;
		}
		acTmpData[i] = 0;							//�ÿ�
		linkinfo.linkid = atoi(acTmpData);			//���ַ�������ת������������
		//��ȡFlag
		i = 0;                 
		fseek(fp, 13, 1);							//�����ļ�ָ��λ��
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//��ȡ��ʱ�����ļ��е��ַ�
			if (acTmpData[i] == ';')				//������;��
			{
				break;								//�ж�����
			}
			++i;
		}
		acTmpData[i] = 0;							//�ÿ�
		linkinfo.flag = atoi(acTmpData);			//���ַ�������ת������������
		//��ȡ��·��
		i = 0;               
		fseek(fp, 7, 1);							//�����ļ�ָ��λ��
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//��ȡ��ʱ�����ļ��е��ַ�
			if (acTmpData[i] == ';')				//������;��
			{
				break;								//�ж�����
			}
			++i;
		}
		acTmpData[i] = 0;							//�ÿ�
		linkinfo.brunch = atoi(acTmpData);			//���ַ�������ת������������
		//��ȡ����
		i = 0;             
		fseek(fp, 10, 1);							//�����ļ�ָ��λ��
		while (1)
		{
			fscanf(fp, "%c", &acTmpData[i]);		//��ȡ��ʱ�����ļ��е��ַ�
			if (acTmpData[i] == ';' || acTmpData[i] == '#')//������;����#��
			{
				break;								//�ж�����
			}
			++i;
		}
		acTmpData[i] = 0;							//�ÿ�
		linkinfo.Class = atoi(acTmpData);			//���ַ�������ת������������
		//��ȡ��·����
		if (linkinfo.flag)							//���flagΪ��
		{
			strcpy(acTmpData, "����");				//��·���Ƶ�ǰ׺
			i = 4;
			fseek(fp, 9, 1);						//�����ļ�ָ��λ��
			while (1)
			{
				fscanf(fp, "%c", &acTmpData[i]);	//��ȡ��ʱ�����ļ��е��ַ�
				if (acTmpData[i] == '#')			//������#��
				{
					break;							//�ж�����
				}
				++i;
			}
			acTmpData[i] = 0;						//�ÿ�
			strcpy(linkinfo.name, acTmpData);		//����ʱ�洢��name�ַ������Ƶ�linkinfo.name
			linkinfo.namesize = strlen(linkinfo.name);//�����·���ƴ�С
		}
		else
		{
			linkinfo.namesize = 0;					//��namesize��Ϊ0
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
		if (i >= (*inum))							//���������м�¼���ֵ
		{
			if ((*inum) >= MAX)						//����������޶�ֵ
			{
				UpdateErrorInterface();				//��ʾ���³�������
				system("pause");					//������ͣ
				return 0;
			}
			st_Link[i] = linkinfo;
			(*inum)++;
		}
		else
		{
			st_Link[i] = linkinfo;
		}
		TitleWelcomeInterface();					//��ʾ��ӭ����
		printf("\n");
		printf("\n");
		printf("\n");
		printf("    ���ڸ��µ�%d����·��ϸ��Ϣ:\n", ++iCountInfoNum);//��ʾ��Ϣ
		printf("\n");
		printf("    #linkid=%d;roadnameflag=%d;brunch=%d;dispclass=%d", st_Link[i].linkid, st_Link[i].flag, st_Link[i].brunch, st_Link[i].Class);
		if (st_Link[i].flag)						//��ʾ��·����
		{
			printf(";roadname=");
			for (j = 4; j < st_Link[i].namesize; j++)
			{
				printf("%c", st_Link[i].name[j]);
			}
		}
		printf("#\n");
		Gotoxy(0, 22);								//��λ��굽�����괦
		printf("��������������������������������������������������������������������������������");
		printf("ϵͳ��ʾ: ��%d����Ϣ���³ɹ�, ", iCountInfoNum);
		system("pause");							//������ͣ
		if ( fscanf(fp, "%c", &acTmpData[0]) != 1)	//�ж��Ƿ��ļ���β
		{
			break;
		}
	}
	fclose(fp);										//�ر��ļ�
	fp = NULL;										//ָ���ÿ�
	WriteFile((*inum), 0, -1, "GTBL.dat");			//�����º������д���ļ�
	InorderOutput((*inum));							//����������
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
		Sleep(2000);								//����ϵͳ��δ�ҵ������ļ�������ϵͳ�ȴ�2����ʾ���˵����������ʾ
		ReadFileErrorInterface();
		system("pause");
		return 0;
	}
	memset (st_Link, 0, sizeof(st_Link));			//��ʼ���ṹ������
	while (fread(&st_FileData, sizeof(st_FileData), 1, fpr))//ѭ����ȡ�ļ�����
	{
		st_Link[*inum].nodesize = MC_GET_SHORT(st_FileData.nodesize);
		st_Link[*inum].linkid = MC_GET_LONG(st_FileData.LinkID);
		st_Link[*inum].namesize = MC_GET_SHORT(st_FileData.namesize);
		st_FileData.Reserved = MC_GET_LONG(st_FileData.Reserved);
		st_Link[*inum].reserved = MC_GET_RESERVED(st_FileData.Reserved);
		st_Link[*inum].flag = MC_GET_FLAG(st_FileData.Reserved);
		st_Link[*inum].brunch = MC_GET_BRUNCH(st_FileData.Reserved);
		st_Link[*inum].Class = MC_GET_CLASS(st_FileData.Reserved);

		if (st_Link[*inum].flag)					//���flag��Ϊ�����ȡ��·����
		{
			MC_GET_NAME(fpr, st_Link[*inum].name, st_Link[*inum].namesize);
		}
		(*inum)++;
	}
	fclose(fpr);									//�ر��ļ�ָ��
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
	FILE *fpw = NULL;								//�ļ�ָ���ÿ�
	if ( (fpw = fopen(outputfilename, "wb+")) == NULL )
	{
		WriteFileErrorInterface();					//������ʾ
		return 0;
	}
	TitleWelcomeInterface();						//��ʾ��ӭ����
	if (sign == -1)
	{
		printf("\n");
	}
	else
	{
		printf("                      %s����ʱ��Ϊ%ld����\n", paCalcTime[sign], time);//���ʱ��ͳ����Ϣ
	}
	printf("\n");
	printf("\n");
	printf("                      ��������������������������������������\n");
	printf("                      ��                                  ��\n");
	if (sign == -1)
	{
		printf("                      ��    �����ļ�GBTL.dat���³ɹ�!     ��\n");
	}
	else
	{
		printf("                      ��  �����ļ�NEW_GBTL.dat���³ɹ�!   ��\n");
	}
	printf("                      ��    File Updates Successfully!    ��\n");
	printf("                      ��     �뵽��������Ŀ¼�²鿴       ��\n");
	printf("                      ��                                  ��\n");
	printf("                      ��������������������������������������\n");
	Gotoxy(0, 22);
	printf("��������������������������������������������������������������������������������");
	system("pause");
	for (i = 0; i < inum; i++)						//ѭ��д�������ļ�
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
//�������ܣ����˵�
int MainMenu()
{
	char choose = 0;	//�û��Բ˵���ѡ��
	int iNodeNum = 0;	//ͳ�Ƶ�·������
	MainMenuInterface();//��ʾϵͳ���˵�
	if ( ReadFile(&iNodeNum) == 0 )
	{
		return 0;
	}
	choose = getch();	//��ȡ�û��Բ˵���ѡ��
	switch (choose)		//�����û��Բ˵���ѡ������ض�����
	{
	case '1' : InorderOutput(iNodeNum); break;//�������
	case '2' : SearchMenu(iNodeNum);    break;//�����˵�
	case '3' : Update(&iNodeNum);       break;//��������
	case '4' : 
	case ESC :
		ExitInterface();
		return 0;                       break;//�ж�
	default:break;							  //�ж�
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
	system("color 2f");      //���Ŀ���̨��ɫ
	system("title GPS NAVI");//���Ŀ���̨����
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	while (MainMenu());      //ѭ���������˵���ֱ���û��Լ��˳�
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
	pos.X = x;	//������
	pos.Y = y;	//������
	SetConsoleCursorPosition(hOut, pos);//���ù��λ��
}