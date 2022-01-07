#pragma once
#include "NodeBox.h"
class StudentManager
{
public:
	StudentManager()
	{
		g_iMaxUserCounter = 0;
		g_pHeadUserList = 0;
		g_pEndUser = 0;
	}
	~StudentManager()
	{
		DeleteAll();
	}
	void SetUserCounter(int iCnt)
	{
		g_iMaxUserCounter = iCnt;
	}
	int  GetUserCounter() const
	{
		return g_iMaxUserCounter;
	}
	NodeBox<Student>* GetHead()
	{
		return g_pHeadUserList;
	}
private:
	int    g_iMaxUserCounter;
	NodeBox<Student>* g_pHeadUserList;
	NodeBox<Student>* g_pEndUser;
public:
	/// <summary>
	/// 연결리스트 해당파일로 출력 기능
	/// </summary>
	bool FileSave(const char* pFileName);
public:
	NodeBox<Student>* const NewNode();
	void DeleteAll();
	// 1)데이터 초기화 및 입력
	void Create();
	// 3) 로드해서 저장
	void Load(const char* pFileName);
	// 4) 화면 출력
	void Draw();

	void AddLink(NodeBox<Student>* const pUser);

	friend std::ostream& operator << (
		std::ostream& os,
		const  StudentManager& mgr);
private:
};