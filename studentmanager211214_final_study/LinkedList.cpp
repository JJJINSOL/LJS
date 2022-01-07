#include "LinkedList.h"
template <class T>
void LinkedList<T>::AddLink(NodeBox<T>* const node)
{
	if (m_headnode == nullptr)
	{
		m_headnode = node;
		m_endnode = node;
		m_nodenumber++;
	}
	else
	{
		m_endnode->m_next = node;
		m_endnode = node;
		m_nodenumber++;
	}
}

template <class T>
void LinkedList<T>::AllDelete()
{
	NodeBox<T>* next = m_headnode;
	while(next)
	{
		NodeBox<T>* del = next;
		next = del->m_next;
		delete del;
		del = nullptr;
		m_nodenumber--;
	}
	m_headnode = nullptr;
}