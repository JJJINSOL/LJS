#pragma once
#include "Object.h"
class Student : public Object
{
public:
	int			m_iIndex; //0
	int			m_iTotal; //5
	float		m_fAverage;	//6
public:
	// 가상테이블 생성됨.
	virtual void	Show();
	virtual void	Save();
	virtual void	Load() override;
	virtual void    SetData(int iIndex);
public:
	// virtual 사용불가
	template<typename T>
	T operator [] (int id);
	template<>
	float operator [] (int id);

	// p1 * 3 , 3 * p1
	friend std::ostream& operator << (
		std::ostream& os,
		Student& data);
	Student()
	{
		m_iIndex = 0;
		m_iType = 0;
	}
	Student(int i, int kor)
	{
		m_iIndex = i;
		m_iType = 0;
	}
	virtual ~Student()
	{
		int k = 0;
	}
};

