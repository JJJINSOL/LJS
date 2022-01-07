#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <list>
#include "Object.h"
class Node
{
public:
	Node* m_Parent;
	Node* m_Child[8];
	Box m_box; 
	int m_Depth;
	int index;

	std::list<Object*> m_ObjectList;
	void AddObject(Object* obj);

	Node()
	{
		m_Parent = nullptr;
		m_Depth = 0;
		for (int i = 0; i < 8; i++)
		{
			m_Child[i] = nullptr;
		}
		for (std::list<Object*>::iterator iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
		{
			Object* obj = *iter;
			delete obj;
		}
		m_ObjectList.clear();
	}
	Node(float x, float y, float z, float w, float h, float q) : Node()
	{
		m_box.vMin.x = x;
		m_box.vMin.y = y;
		m_box.vMin.z = z;

		m_box.vMax.x = x + w;
		m_box.vMax.y = y + h;
		m_box.vMax.z = z + q;

		m_box.vMiddle.x = x + (w / 2.0f);
		m_box.vMiddle.y = y + (h / 2.0f);
		m_box.vMiddle.z = z + (q / 2.0f);

		m_box.vSize.x = w;
		m_box.vSize.y = h;
		m_box.vSize.z = q;

		for (int i = 0; i < 8; i++)
		{
			m_Child[i] = nullptr;
		}
	}
	~Node()
	{
		for (int i = 0; i < 8; i++)
		{
			delete m_Child[i];
		}
	}
};

