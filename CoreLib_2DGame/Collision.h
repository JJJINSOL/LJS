#pragma once
#include "Vector3.h"
enum Type { RECT_OUT = 0, RECT_IN = 1, RECT_OVERLAP = 2 };
// È­¸éÁÂÇ¥°è+¿ÞÂÊ»ó´ÜÀÌ ¿øÁ¡
struct Rect2D
{
	Vector2 vMin;
	Vector2 vMax;
	Vector2 vMiddle;
	Vector2 vSize;
	bool operator == (const Rect2D& v)
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
	Rect2D() {};
	Rect2D(Vector2 vMin, Vector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		vSize.x = vMax.x - vMin.x;
		vSize.y = vMax.y - vMin.y;
	}
	Rect2D(Vector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + Vector2(w, h);
		vMiddle = (vMax + vMin) / 2.0f;
		this->vSize.x = w;
		this->vSize.y = h;
	}
};
// È­¸éÁÂÇ¥°è+Áß¾ÓÀÌ ¿øÁ¡
struct Rect
{
	Vector2 vMin;
	Vector2 vMax;
	Vector2 vMiddle;
	Vector2 vSize;
	bool operator == (const Rect& v)
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
	Rect() {};
	Rect(Vector2 vMin, Vector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vMiddle = (vMax + vMin) / 2.0f;
		vSize.x = vMax.x - vMin.x;
		vSize.y = vMax.y - vMin.y;
	}
	Rect(Vector2 pos, float w, float h)
	{
		vMiddle = pos;
		this->vMin = vMiddle - Vector2(w / 2.0f, h / 2.0f);
		this->vMax = vMiddle + Vector2(w / 2.0f, h / 2.0f);
		this->vSize.x = w;
		this->vSize.y = h;
	}
};

class Box
{
public:
	Vector3 vMin;
	Vector3 vMax;
	Vector3 vMiddle;
	Vector3 vSize;

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
	Box(Vector3 Min, Vector3 Max)
	{
		vMin = Min;
		vMax = Max;
		vMiddle = (Max + Min) / 2.0f;
		vSize.x = Max.x - Min.x;
		vSize.y = Max.y - Min.y;
		vSize.z = Max.z - Min.z;
	};
	Box(Vector3 v, float w, float h, float q)
	{
		vMin = v;
		vMax = vMin + Vector3(w, h, q);
		vMiddle = (vMax + vMin) / 2.0f;
		vSize.x = w;
		vSize.y = h;
		vSize.z = q;
	}
};

class Collision
{
public:
	static bool RectToPoint(Rect rt, int x, int y);
	static bool RectToPoint(Rect rt, Vector2 v);
	static Type RectToRect(Rect rt1, Rect rt2);

	static Type ToRect(Rect rt1, Rect rt2);

	static Rect UnionRect(Rect rt1, Rect rt2);
	static bool IntersectRect(Rect rt1, Rect rt2, Rect* rt);


	static bool BoxToPoint(Box bx, int x, int y, int z);
	static bool BoxToPoint(Box bx, Vector3 v);
	static Type BoxToBox(Box box1, Box box2);
	static Box UnionBox(Box bx1, Box bx2);
	static bool IntersectBox(Box bx1, Box bx2, Box* bx);
};

