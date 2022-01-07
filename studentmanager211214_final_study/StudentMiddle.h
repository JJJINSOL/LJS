#pragma once
#include "Student.h"
class StudentMiddle : public Student
{
public:
	int m_kor;
	int m_eng;
	int m_mat;

	//¿Á¡§¿«
	void Show() override;
	void Save() override;
	void Load() override;
	void SetData(int iIndex) override;

	template <typename T>
	T operator [] (int i);
	template <>
	float operator[](int i);

	StudentMiddle()
	{
		m_type = 1;
	}
	~StudentMiddle()
	{}
};

