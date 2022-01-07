#pragma once
#include <iostream>
#include "Vector2.h"
class Object
{
public:
	Vector2 m_pos;


	//Object()
	//{
	//	m_pos.x = 0;
	//	m_pos.y = 0;
	//}
	Object(int x, int y)
	{
		m_pos.x = x;
		m_pos.y = y;
	}

};


