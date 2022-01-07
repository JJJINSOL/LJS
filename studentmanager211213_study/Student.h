#pragma once
#include "Object.h"
class Student : public Object
{
public:
	int m_index;
	int m_total;
	float m_average;

	//object 부모 클래스 함수 재정의
	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;

	//함수 선언
	virtual void SetData(int index);

	Student()
	{
		m_index = 0;
		m_type = 0;
	}
	~Student()
	{
	}

};

