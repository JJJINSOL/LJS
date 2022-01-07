#pragma once
#include "Student.h"
class StudentCollege : public Student
{
public:
	int m_graphic;
	int m_physics;
	int m_ai;

	template <typename T>
	T operator [] (int i);
	template <>
	float operator [] (int i);

	void Show() override;
	void Save() override;
	void Load() override;

	void SetData(int i) override;

	StudentCollege()
	{
		m_type = 3;
	}
	~StudentCollege() {}
};

