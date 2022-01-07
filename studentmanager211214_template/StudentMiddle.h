#pragma once
#include "Student.h"
class StudentMiddle : public Student
{
public:
	int			m_iKor; // 2
	int			m_iEng; // 3
	int			m_iMat; // 4
	template<typename T>
	T operator [] (int id);
	template<>
	float operator [] (int id);
public:
	virtual void	Show()override;
	virtual void	Save()override;
	virtual void	Load() override;
	virtual void    SetData(int iIndex) override;;
public:
	StudentMiddle()
	{
		m_iType = 1;
	}
	StudentMiddle(int i, int kor)
	{
		m_iType = 1;
		m_iIndex = i;
		m_iKor = kor;
	}
	~StudentMiddle()
	{
		int k = 0;
	}
};
