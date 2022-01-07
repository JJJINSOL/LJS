#pragma once
#include "Student.h"
class StudentMiddle : public Student
{
public:
	int m_kor;
	int m_eng;
	int m_mat;

	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;
	virtual void SetData(int iIndex) override;

	StudentMiddle()
	{
		m_type = 1;
	}
	~StudentMiddle()
	{

	}
};

