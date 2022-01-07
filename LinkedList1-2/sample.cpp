#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
struct User 
{
	int uIndex;
	int uKor;
	User* uNext;
};

int uMaxCount = 0;
User* HeadUser = 0;
User* EndUser = 0;

FILE* Create();
User* NewNode();
void AddLink(User* newuser);
int Insert(FILE* fp);
void DelAll();
void Load();
void Draw(int iCounter);

void main()
{
	//데이터 생성
	FILE* fp = Create();
	//데이터 추가
	int Counter = Insert(fp);
	fclose(fp);
	//연결리스트 삭제, 파일에서는 삭제 안함
	DelAll();
	//로드해서 저장
	Load();
	//화면 출력
	Draw(uMaxCount);

	DelAll();
}
User* NewNode()
{
	User* iiuser = (User*)malloc(sizeof(User));
	iiuser->uIndex = uMaxCount;
	iiuser->uKor = rand() % 100;
	iiuser->uNext = NULL;
	return iiuser;
}
void AddLink(User* newuser) 
{
	if (HeadUser == NULL)
	{
		HeadUser = newuser;
		EndUser = newuser;
		uMaxCount++;
		//return;
	}
	else 
	{
		EndUser->uNext = newuser;
		EndUser = newuser;
		uMaxCount++;
	}
}
FILE* Create() 
{
	//연결리스트 생성
	for (int i = 0; i < 5; i++)
	{
		AddLink(NewNode());
	}
	//파일 만들고 넣기
	FILE* fp = fopen("test.txt", "r+b");
	if (fp == NULL)
	{
		fp = fopen("test.txt", "wb");
		//int ic = uMaxCount;
		fwrite(&uMaxCount, sizeof(int), 1, fp);
		for (User* iuser = HeadUser; iuser != NULL; iuser = iuser->uNext)
		{
			fwrite(iuser, sizeof(User), 1, fp);
		}
		fseek(fp, 0, SEEK_SET);
	}
	return fp;
}
void DelAll()
{
	User* uNextNode = HeadUser;
	while (uNextNode)
	{
		User* uDel = uNextNode;
		uNextNode = uDel->uNext;
		free(uDel);
		uDel = NULL;
	}
	HeadUser = NULL;
}
int Insert(FILE* fp)
{
	User* uEndNode = EndUser;
	for (int i = 0; i < 2; i++)
	{
		AddLink(NewNode());
	}
	fwrite(&uMaxCount, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (User* iuser = uEndNode->uNext; iuser != NULL; iuser = iuser->uNext)
	{
		fwrite(iuser, sizeof(User), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return uMaxCount;
}
void Load()
{
	//파일 읽어오기
	FILE* fp = fopen("test.txt", "rb");
	int Counter = 0;
	fread(&Counter, sizeof(int), 1, fp);
	if (fp != NULL)
	{
		for (int i = 0; i < Counter; i++)
		{
			User* iuser = (User*)malloc(sizeof(User));
			memset(iuser, 0, sizeof(User));
			fread(iuser, sizeof(User), 1, fp);
			iuser->uNext = NULL;
			AddLink(iuser);
		}
		fclose(fp);
		fp = NULL;
	}

}
void Draw(int C)
{
	for (User* iuser = HeadUser; iuser != NULL; iuser = iuser->uNext)
	{
		printf("%d %d\n", iuser->uIndex, iuser->uKor);
	}
}