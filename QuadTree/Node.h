#pragma once
#include "Object.h"
#include <vector>
#include <list>
#include <queue>
#include <iostream>

struct Rect
{
	Vector2 p1, p2, middle, size;

};
class Node
{
public:
	Node* m_parent;
	Node* m_child[4];
	int m_depth;
	Rect m_rect;
	std::list<Object*> m_objectlist;

	void AddObject(int x, int y);
	bool IsRect(int x, int y);

	Node()
	{
		m_parent = nullptr;
		m_child[0] = nullptr;
		m_child[1] = nullptr;
		m_child[2] = nullptr;
		m_child[3] = nullptr;
		m_depth = 0;
		for (std::list<Object*>::iterator iter = m_objectlist.begin(); iter != m_objectlist.end(); iter++)
		{
			Object* i = *iter;
			delete i;
		}
		m_objectlist.clear();
	}
	Node(float x, float y, float w , float h) : Node()
	{
		m_rect.p1.x = x;
		m_rect.p1.y = y;
		m_rect.p2.x = x+w;
		m_rect.p2.y = y+h;
		m_rect.middle.x = x+(w/2.0f);
		m_rect.middle.y = y+(h/2.0f);
		m_rect.size.x = w;
		m_rect.size.y = h;
		m_child[0] = nullptr;
		m_child[1] = nullptr;
		m_child[2] = nullptr;
		m_child[3] = nullptr;
	}
	~Node()
	{
		delete m_child[0];
		delete m_child[1];
		delete m_child[2];
		delete m_child[3];
	}
};


