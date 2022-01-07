#include "Collision.h"

Box Collision::UnionBox(Box bx1, Box bx2)
{
	Box unibox;
	unibox.vMin.x = bx1.vMin.x < bx2.vMin.x ? bx1.vMin.x : bx2.vMin.x;
	unibox.vMin.y = bx1.vMin.y < bx2.vMin.y ? bx1.vMin.y : bx2.vMin.y;
	unibox.vMin.z = bx1.vMin.z < bx2.vMin.z ? bx1.vMin.z : bx2.vMin.z;

	unibox.vMax.x = bx1.vMax.x < bx2.vMax.x ? bx2.vMax.x : bx1.vMax.x;
	unibox.vMax.y = bx1.vMax.y < bx2.vMax.y ? bx2.vMax.y : bx1.vMax.y;
	unibox.vMax.z = bx1.vMax.z < bx2.vMax.z ? bx2.vMax.z : bx1.vMax.z;

	unibox.vSize = unibox.vMax - unibox.vMin;
	unibox.vMiddle = (unibox.vMin + unibox.vMax) / 2.0f;
	return unibox;
}
bool Collision::IntersectBox(Box bx1, Box bx2, Box* rt)
{
	Box unibox = UnionBox(bx1, bx2);
	if (unibox.vSize.x <= (bx1.vSize.x + bx2.vSize.x) &&
		unibox.vSize.y <= (bx1.vSize.y + bx2.vSize.y) &&
		unibox.vSize.z <= (bx1.vSize.z + bx2.vSize.z))
	{
		if (rt != nullptr)
		{
			rt->vMin.x = bx1.vMin.x < bx2.vMin.x ? bx2.vMin.x : bx1.vMin.x;
			rt->vMin.y = bx1.vMin.y < bx2.vMin.y ? bx2.vMin.y : bx1.vMin.y;
			rt->vMin.z = bx1.vMin.z < bx2.vMin.z ? bx2.vMin.z : bx1.vMin.z;

			rt->vMax.x = bx1.vMax.x < bx2.vMax.x ? bx1.vMax.x : bx2.vMax.x;
			rt->vMax.y = bx1.vMax.y < bx2.vMax.y ? bx1.vMax.y : bx2.vMax.y;
			rt->vMax.z = bx1.vMax.z < bx2.vMax.z ? bx1.vMax.z : bx2.vMax.z;

			rt->vSize = rt->vMax - rt->vMin;
			rt->vMiddle = (rt->vMax + rt->vMin) / 2.0f;
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