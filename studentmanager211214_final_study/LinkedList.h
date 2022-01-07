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

	//���Ḯ��Ʈ�� ��� �߰�
	void AddLink(NodeBox<T>* const node);

	//���Ḯ��Ʈ�� ��� ����
	void AllDelete();

	//������
	LinkedList() 
	{
		m_nodenumber = 0;
		m_headnode = nullptr;
		m_endnode = nullptr;
	}
	//�Ҹ���
	~LinkedList()
	{}
};

