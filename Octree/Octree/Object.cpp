#include "Object.h"
void Object::SetPos(Vector pos)
{
	m_vPos = pos;
}
void Object::SetPos(float x, float y, float z)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;
}
void Object::SetBox(Box box)
{
	m_box = box;
}