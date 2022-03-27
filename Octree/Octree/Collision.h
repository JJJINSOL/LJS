#pragma once
#include "Vector.h"
enum Type { RECT_OUT = 0, RECT_IN = 1, RECT_OVERLAP = 2 };
class Box
{
public:
	Vector vMin;
	Vector vMax;
	Vector vCenter;
	Vector vSize;

	bool operator == (const Box& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Box() {};
	Box(Vector Min, Vector Max)
	{
		vMin = Min;
		vMax = Max;
		vCenter = (Max + Min) / 2.0f;
		vSize.x = Max.x - Min.x;
		vSize.y = Max.y - Min.y;
		vSize.z = Max.z - Min.z;
	};
	Box(Vector v, float w, float h, float q)
	{
		vMin = v;
		vMax = vMin + Vector(w, h, q);
		vCenter = (vMax + vMin) / 2.0f;
		vSize.x = w;
		vSize.y = h;
		vSize.z = q;
	}
};
class Collision
{
public:
	static bool BoxToPoint(Box rt, int x, int y, int z);
	static bool BoxToPoint(Box rt, Vector v);
	static Type BoxToBox(Box box1, Box box2);
	static Box UnionBox(Box rt1, Box rt2);
	static bool IntersectBox(Box rt1, Box rt2, Box* rt);
};

