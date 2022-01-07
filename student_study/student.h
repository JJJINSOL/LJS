#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

class User
{
public:
	int uIndex;
	int uscore;
	User* uNext;
	User()
	{
		uIndex = 0;
		uscore = 0;
		uNext = NULL;
	}
	User(int i, int j)
	{
		uIndex = i;
		uscore = j;
		uNext = NULL;
	}
	~User()
	{

	}
};
class student
{
public:
	student()
	{
		uCount = 0;
		uHead = 0;
		uEnd = 0;
	}
	~student()
	{
		AllDel();
	}
	void setCount(int num)
	{
		uCount = num;
	}
	int getCount()
	{
		return uCount;
	}
private:
	int uCount;
	User* uHead;
	User* uEnd;
public:
	User* NewNode();
	void AddLink(User* node);
	void Create();
	bool FileSave();
	void AllDel();
	void Load();
	void Draw();

};

