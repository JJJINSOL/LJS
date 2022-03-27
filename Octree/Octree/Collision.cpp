#include "Collision.h"
bool Collision::BoxToPoint(Box rt, int x, int y, int z)
{
	if (rt.vMin.x <= x && rt.vMax.x >= x &&
		rt.vMin.y <= y && rt.vMax.y >= y &&
		rt.vMin.z <= z && rt.vMax.z >= z)
	{
		return true;
	}
	return false;
}
bool Collision::BoxToPoint(Box rt, Vector v)
{
	if (rt.vMin.x <= v.x && rt.vMax.x >= v.x &&
		rt.vMin.y <= v.y && rt.vMax.y >= v.y &&
		rt.vMin.z <= v.z && rt.vMax.z >= v.z)
	{
		return true;
	}
	return false;
}

Box Collision::UnionBox(Box rt1, Box rt2)
{
	Box unibox;
	unibox.vMin.x = rt1.vMin.x < rt2.vMin.x ? rt1.vMin.x : rt2.vMin.x;
	unibox.vMin.y = rt1.vMin.y < rt2.vMin.y ? rt1.vMin.y : rt2.vMin.y;
	unibox.vMin.z = rt1.vMin.z < rt2.vMin.z ? rt1.vMin.z : rt2.vMin.z;

	unibox.vMax.x = rt1.vMax.x < rt2.vMax.x ? rt2.vMax.x : rt1.vMax.x;
	unibox.vMax.y = rt1.vMax.y < rt2.vMax.y ? rt2.vMax.y : rt1.vMax.y;
	unibox.vMax.z = rt1.vMax.z < rt2.vMax.z ? rt2.vMax.z : rt1.vMax.z;

	unibox.vSize = unibox.vMax - unibox.vMin;
	unibox.vCenter = (unibox.vMin + unibox.vMax) / 2.0f;
	return unibox;
}
bool Collision::IntersectBox(Box rt1, Box rt2, Box* rt)
{
	Box unibox = UnionBox(rt1, rt2);
	if (unibox.vSize.x <= (rt1.vSize.x + rt2.vSize.x) &&
		unibox.vSize.y <= (rt1.vSize.y + rt2.vSize.y) &&
		unibox.vSize.z <= (rt1.vSize.z + rt2.vSize.z))
	{
		if (rt != nullptr)
		{
			rt->vMin.x = rt1.vMin.x < rt2.vMin.x ? rt2.vMin.x : rt1.vMin.x;
			rt->vMin.y = rt1.vMin.y < rt2.vMin.y ? rt2.vMin.y : rt1.vMin.y;
			rt->vMin.z = rt1.vMin.z < rt2.vMin.z ? rt2.vMin.z : rt1.vMin.z;

			rt->vMax.x = rt1.vMax.x < rt2.vMax.x ? rt1.vMax.x : rt2.vMax.x;
			rt->vMax.y = rt1.vMax.y < rt2.vMax.y ? rt1.vMax.y : rt2.vMax.y;
			rt->vMax.z = rt1.vMax.z < rt2.vMax.z ? rt1.vMax.z : rt2.vMax.z;

			rt->vSize = rt->vMax - rt->vMin;
			rt->vCenter = (rt->vMax + rt->vMin) / 2.0f;
		}
		return true;
	}
	return false;
}

Type Collision::BoxToBox(Box box1, Box box2)
{
	Box bx;
	int ret = IntersectBox(box1, box2, &bx);
	if (ret <= 0)
	{
		return RECT_OUT;
	}
	if (bx == box2) return RECT_IN;
	return RECT_OVERLAP;
}