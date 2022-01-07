#pragma once
#include "StudentMiddle.h"
#include "StudentHigh.h"
#include "StudentCollege.h"
template<class T>
class NodeBox
{
public:
	T* m_pData;
	friend std::ostream& operator << (
		std::ostream& os,
		NodeBox<T>& data)
	{
		os << *data.m_pData;
		return os;
	}
public:
	NodeBox* m_pNext;
	NodeBox();
	NodeBox(T* data);
	~NodeBox();
};
//std::ostream& operator << (
//	std::ostream& os,
//	NodeBox<T>& data)
//{
//	os << *data.m_pData;
//	return os;
//}
template<class T>
NodeBox<T>::NodeBox()
{
	m_pData = nullptr;
}
template<class T>
NodeBox<T>::NodeBox(T* data)
{
	m_pData = data;
}
template<class T>
NodeBox<T>::~NodeBox()
{
	delete m_pData;
}
