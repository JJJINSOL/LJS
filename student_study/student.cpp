#include "student.h"

User* student::NewNode() 
{
	User* newnode = new User();
	newnode->uIndex = uCount;
	newnode->uscore = rand() % 100;
	newnode->uNext = NULL;
	return newnode;
}

void student::AddLink(User* node)
{
	if(uHead==NULL)
	{
		uHead = node;
		uEnd = node;
		uCount++;
	}
	else
	{
		uEnd->uNext = node;
		uEnd = node;
		uCount++;
	}
}

void student::Create()
{
	for (int i = 0; i < 3; i++)
	{
		AddLink(NewNode());
	}
}

bool student::FileSave()
{
	FILE* fp = fopen("test.txt", "wb");
	fwrite(&uCount, sizeof(int), 1, fp);
	for (User* iuser = uHead; iuser != 0; iuser = iuser->uNext)
	{
		fwrite(iuser, sizeof(User), 1, fp);
	}
	fclose(fp);
	return true;
}

void student::AllDel()
{
	User* nextnode = uHead;
	while (nextnode)
	{
		User* delnode = nextnode;
		nextnode = delnode->uNext;
		delete delnode;
		uCount--;
		delnode = NULL;
	}
	uHead = NULL;
}

void student::Load()
{
	FILE* fp = fopen("test.txt", "rb");
	int n = 0;
	fread(&n, sizeof(int), 1, fp);
	if (fp != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			User* iuser = new User(0, 0);
			fread(iuser, sizeof(User), 1, fp);
			iuser->uNext = NULL;
			AddLink(iuser);
		}
		fclose(fp);
		fp = NULL;
	}

}

void student::Draw()
{
	for (User* iuser = uHead; iuser != NULL; iuser = iuser->uNext)
	{
		printf("%d %d\n", iuser->uIndex, iuser->uscore);
	}
}