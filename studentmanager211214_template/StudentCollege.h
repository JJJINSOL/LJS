#pragma once
#include "Student.h"
class StudentCollege : public Student
{
public:
	int   m_iGraphic;
	int   m_iPhysics;
	int   m_iAI;
public:
	void		Show() override;
	void		Save() override;
	void		Load() override;
	void		SetData(int iIndex) override;
public:
	StudentCollege()
	{
		m_iType = 3;
	}
	StudentCollege(int i, int kor)
	{
		m_iType = 3;
		m_iIndex = i;
		m_iPhysics = kor;
	}
	~StudentCollege()
	{
		int k = 0;
	}
};
