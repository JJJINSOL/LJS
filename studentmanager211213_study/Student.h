#pragma once
#include "Object.h"
class Student : public Object
{
public:
	int m_index;
	int m_total;
	float m_average;

	//object �θ� Ŭ���� �Լ� ������
	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;

	//�Լ� ����
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

