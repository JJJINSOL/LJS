#pragma once
#include "NodeBox.h"
class StudentManager
{
public:
	StudentManager()
	{
		userCounter = 0;
		headUser = nullptr;
		endUser = nullptr;
	}
	~StudentManager()
	{
		allDel();
	}
	void SetUserCounter(int iCnt) { userCounter = iCnt; }
	int GetUserCounter() const	{ return userCounter; }

	NodeBox* GetHead() { return headUser; }

private:
	int userCounter;
	NodeBox* headUser;
	NodeBox* endUser;

public:
	NodeBox* const NewNode();
	void AddLink(NodeBox* const pUser);
	void Create();
	bool FileSave(const char* pFileName);
	void allDel();
	void Load(const char* pFileName);
	void Draw();
	friend std::ostream& operator << (std::ostream& os, StudentManager& mgr);
};

