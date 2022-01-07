#pragma once
#include "StudentMiddle.h"
#include "StudentHigh.h"
#include "StudentCollege.h"
template <class T>
class NodeBox
{
public:
	T* m_data;

	NodeBox* m_next;
	NodeBox();
	NodeBox(T* data);
	~NodeBox();
};
template <class T>
NodeBox<T>::NodeBox()
{
	m_data = nullptr;
}
template <class T>
NodeBox<T>::NodeBox(T* data)
{
	m_data = data;
}
template <class T>
NodeBox<T>::~NodeBox()
{
	delete m_data;
}
