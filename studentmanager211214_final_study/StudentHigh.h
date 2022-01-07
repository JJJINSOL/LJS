#pragma once
#include "Student.h"
class StudentHigh : public Student
{
public:
	int m_kor;
	int m_eng;
	int m_mat;

	template <typename T>
	T operator [] (int i);
	template <>
	float operator [] (int i);

	void Show() override;
	void Save() override;
	void Load() override;

	void SetData(int i) override;

	StudentHigh() 
	{
		m_type = 2;
	}
	~StudentHigh() {}
};

