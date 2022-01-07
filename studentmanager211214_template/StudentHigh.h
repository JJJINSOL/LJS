#pragma once
#include "Student.h"
class StudentHigh : public Student
{
public:
	int			m_iKor;
	int			m_iEng;
	int			m_iMat;
public:
	void		Show() override;
	void		Save() override;
	void		Load() override;
	void		SetData(int iIndex)override;
public:
	StudentHigh()
	{
		m_iType = 2;
	}
	StudentHigh(int i, int kor)
	{
		m_iIndex = i;
		m_iKor = kor;
		m_iType = 2;
	}
	~StudentHigh()
	{
		int k = 0;
	}
};
