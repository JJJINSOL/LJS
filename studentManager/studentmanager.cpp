#include "studentmanager.h"



Student* studentmanager::NewNode()
{
	Student* iiuser = new Student();
	iiuser->m_uIndex = uMaxCount;
	iiuser->m_uKor = rand() % 100;
	iiuser->m_uEng = rand() % 100;
	iiuser->m_uMat = rand() % 100;
	iiuser->m_uTotal = iiuser->m_uKor + iiuser->m_uEng + iiuser->m_uMat;
	iiuser->m_uAve = iiuser->m_uTotal / 3.0f;
	iiuser->m_uNext = NULL;
	return iiuser;
}
void studentmanager::AddLink(Student* newuser)
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
		EndUser->m_uNext = newuser;
		EndUser = newuser;
		uMaxCount++;
	}
}
bool studentmanager::FileSave()
{
	//파일 만들고 넣기
	FILE* fp = fopen("test.txt", "wb");
	int ic = uMaxCount;
	fwrite(&uMaxCount, sizeof(int), 1, fp);
	for (Student* iuser = HeadUser; iuser != NULL; iuser = iuser->m_uNext)
	{
		fwrite(iuser, sizeof(Student), 1, fp);
	}
	fclose(fp);

	return true;
}
void studentmanager::Create()
{
	//연결리스트 생성
	for (int i = 0; i < 5; i++)
	{
		AddLink(NewNode());
	}

}
void studentmanager::DelAll()
{
	Student* uNextNode = HeadUser;
	while (uNextNode)
	{
		Student* uDel = uNextNode;
		uNextNode = uDel->m_uNext;
		delete uDel;
		uDel = NULL;
		uMaxCount--;
	}
	HeadUser = NULL;
}
int studentmanager::Insert(FILE* fp)
{
	Student* uEndNode = EndUser;
	for (int i = 0; i < 2; i++)
	{
		AddLink(NewNode());
	}
	fwrite(&uMaxCount, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	for (Student* iuser = uEndNode->m_uNext; iuser != NULL; iuser = iuser->m_uNext)
	{
		fwrite(iuser, sizeof(Student), 1, fp);
	}
	fclose(fp);
	fp = NULL;
	return uMaxCount;
}
void studentmanager::Load()
{
	//파일 읽어오기
	FILE* fp = fopen("test.txt", "rb");
	int Counter = 0;
	fread(&Counter, sizeof(int), 1, fp);
	if (fp != NULL)
	{
		for (int i = 0; i < Counter; i++)
		{
			Student* iuser = new Student();
			memset(iuser, 0, sizeof(Student));
			fread(iuser, sizeof(Student), 1, fp);
			iuser->m_uNext = NULL;
			AddLink(iuser);
		}
		fclose(fp);
		fp = NULL;
	}

}
void studentmanager::Draw()
{
	for (Student* iuser = HeadUser; iuser != NULL; iuser = iuser->m_uNext)
	{
		//printf("%d %d\n", iuser[0], iuser[1]);
		std::cout << *iuser;
	}
	//std::cout << *HeadUser;
}
