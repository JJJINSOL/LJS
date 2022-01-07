#pragma once
#include <iostream>
#include "Collision.h"
class Object
{
public:
	Vector m_vPos;
	Box m_box;
	int m_obindex;

	void SetPos(Vector pos);
	void SetPos(float x, float y, float z);
	void SetBox(Box box);

	Object(int x, int y, int z)
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	Object()
	{
		m_vPos.x = 0;
		m_vPos.y = 0;
		m_vPos.z = 0;
	}
};

