#pragma once
#include "Student.h"
class StudentHigh : public Student
{
public:
	int m_kor;
	int m_eng;
	int m_mat;

	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;
	virtual void SetData(int iIndex) override;

	StudentHigh()
	{
		m_type = 2;
	}
	~StudentHigh()
	{

	}
};
