#pragma once
#include "Object.h"
enum subject {Index=0, Type=1, Kor=2, Graphic=2, Eng=3, Physics=3, Mat=4, Ai=4, Total=5, Average=6};
class Student : public Object
{
public:
	int m_index;
	int m_total;
	int m_average;

	//재정의
	virtual void Show() override;
	virtual void Save() override;
	virtual void Load() override;
	
	//정의
	virtual void SetData(int iIndex);

	//템플릿 (virtual 사용불가)
	template <typename T>
	T operator [] (int i);

	//템플릿의 특화
	template <>
	float operator [] (int i);

	Student()
	{
		m_index = 0;
		m_type = 0;
	}
	~Student()
	{}
};

