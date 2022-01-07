#include "Student.h"


User* Student:: NewNode()
{
	User* iiuser = new User();
	iiuser->uIndex = uMaxCount;
	iiuser->uKor = rand() % 100;
	iiuser->uNext = NULL;
	return iiuser;
}
void Student::AddLink(User* newuser)
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
bool Student::FileSave() 
{
	//파일 만들고 넣기
	FILE* fp = fopen("test.txt", "wb");
	int ic = uMaxCount;
	fwrite(&uMaxCount, sizeof(int), 1, fp);
	for (User* iuser = HeadUser; iuser != NULL; iuser = iuser->uNext)
	{
		fwrite(iuser, sizeof(User), 1, fp);
	}
	fclose(fp);
	
	return true;
}
void Student::Create()
{
	//연결리스트 생성
	for (int i = 0; i < 5; i++)
	{
		AddLink(NewNode());
	}

}
void Student::DelAll()
{
	User* uNextNode = HeadUser;
	while (uNextNode)
	{
		User* uDel = uNextNode;
		uNextNode = uDel->uNext;
		free(uDel);
		uDel = NULL;
		uMaxCount--;
	}
	HeadUser = NULL;
}
int Student::Insert(FILE* fp)
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

void Student::Load()
{
	//파일 읽어오기
	FILE* fp = fopen("test.txt", "rb");
	int Counter = 0;
	fread(&Counter, sizeof(int), 1, fp);
	if (fp != NULL)
	{
		for (int i = 0; i < Counter; i++)
		{
			User* iuser = new User();
			memset(iuser, 0, sizeof(User));
			fread(iuser, sizeof(User), 1, fp);
			iuser->uNext = NULL;
			AddLink(iuser);
		}
		fclose(fp);
		fp = NULL;
	}

}
void Student::Draw()
{
	for (User* iuser = HeadUser; iuser != NULL; iuser = iuser->uNext)
	{
		printf("%d %d\n", iuser->uIndex, iuser->uKor);
	}
}