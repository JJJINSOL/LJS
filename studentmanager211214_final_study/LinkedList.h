#pragma once
#include "NodeBox.h"
template <class T>
class LinkedList
{
public:
	int m_nodenumber;
	NodeBox<T>* m_headnode;
	NodeBox<T>* m_endnode;

	NodeBox<T>* GetHead()
	{
		return m_headnode;
	}

	//연결리스트에 노드 추가
	void AddLink(NodeBox<T>* const node);

	//연결리스트에 노드 삭제
	void AllDelete();

	//생성자
	LinkedList() 
	{
		m_nodenumber = 0;
		m_headnode = nullptr;
		m_endnode = nullptr;
	}
	//소멸자
	~LinkedList()
	{}
};

