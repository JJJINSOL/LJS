#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#define N 2
struct TUser
{
	int iIndex;
	int iKor;
	TUser* pNext;
};
struct TReturn
{
	FILE* fp;
	TUser* list;
};
//TUser user[N];
int UserCount = 0;

TUser* HeadUser = 0;
TUser* EndUser = 0;

TUser* NewNode();
void DeleteAll();

//TUser* userlistRead = 0;
// 1)데이터 초기화 및 입력
FILE* Create();
// 2) 데이터 추가
int Insert(FILE* fp);
// 3) 로드해서 저장
void Load();
// 4) 화면 출력
void Draw(int iCounter);

void main(int argc, char* argv[])
{
	// 1) 데이터 초기화 및 입력
	FILE* fp = Create();
	// 2) 데이터 추가
	int iCounter = Insert(fp);
	fclose(fp);
	DeleteAll();
	// 3) 로드해서 저장
	Load();
	// 4) 화면 출력
	Draw(UserCount);
	DeleteAll();
}
TUser* NewNode()
{
	TUser* pUser = (TUser*)malloc(sizeof(TUser));
	pUser->iIndex = UserCount;
	pUser->iKor = rand() % 100;
	pUser->pNext = NULL;
	return pUser;
}
void AddLink(TUser* pUser)
{
	if (HeadUser == NULL)
	{
		HeadUser = pUser;
		EndUser = pUser;
		UserCount++;
		return;
	}
	EndUser->pNext = pUser;
	EndUser = pUser;
	UserCount++;
}

FILE* Create()
{
	for (int Data = 0; Data < 30; Data++)
	{
		AddLink(NewNode());
	}

	FILE* fpWrite = fopen("test2.txt", "r+b");
	if (fpWrite == NULL)
	{
		fpWrite = fopen("test2.txt", "wb");
		//블럭단위(덩어리) 입출력 함수
		int iCouner = UserCount;
		fwrite(&iCouner, sizeof(int), 1, fpWrite);
		for (TUser* user = HeadUser; user != NULL; user = user->pNext)
		{
			fwrite(user, sizeof(TUser), 1, fpWrite);
		}
		fseek(fpWrite, 0, SEEK_SET);
		//fclose(fpWrite);
		//fpWrite = fopen("test2.txt", "r+");
	}
	return fpWrite;
}
void DeleteAll()
{
	TUser* pNext = HeadUser;
	while (pNext)
	{
		TUser* DelUser = pNext;
		pNext = DelUser->pNext;
		free(DelUser);
		DelUser = NULL;
	}
	HeadUser = NULL;
}
int Insert(FILE* fp)
{
	TUser* saveEndNode = EndUser;
	for (int Data = 0; Data < 2; Data++)
	{
		AddLink(NewNode());
	}
	fwrite(&UserCount, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (TUser* user = saveEndNode->pNext; user != NULL; user = user->pNext)
	{
		fwrite(user, sizeof(TUser), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return UserCount;
}

void Load()
{
	FILE* fpRead = fopen("test2.txt", "rb");
	int iCounerRead = 0;
	fread(&iCounerRead, sizeof(int), 1, fpRead);
	if (fpRead != NULL)
	{
		for (int i = 0; i < iCounerRead; i++)
		{
			TUser* pUser = (TUser*)malloc(sizeof(TUser));
			memset(pUser, 0, sizeof(TUser));
			fread(pUser, sizeof(TUser),1, fpRead);
			pUser->pNext = 0;
			AddLink(pUser);
		}
		fclose(fpRead);
		fpRead = NULL;
	}
}

void Draw(int iCnt)
{
	for (TUser* iUser = HeadUser; iUser !=NULL; iUser = iUser->pNext)
	{
		printf("%d %d\n", iUser->iIndex, iUser->iKor);
	}
}