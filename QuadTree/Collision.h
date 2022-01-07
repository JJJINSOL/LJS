#pragma once
#include "Vector2.h"

enum CollisionType { Rect_Out = 0, Rect_In = 1, Rect_Overlap = 2};
//���ο��� �Էµ� ������Ʈ ��ġ�� �̰��� �´��� Ȯ��
class Rect 
{
	//2���� ������ ���� ���� (x,y �� ����)
	Vector2 vMin;
	//2���� ������ ���� �����̶� �밢���� �ִ� ���� (x,y �� ����)
	Vector2 vMax;
	//2���� ������ min�� max ������ �������� ��� ���� (x,y �� ����)
	Vector2 vMiddle;
	//����,���� ���� (x,y �� ����)
	Vector2 vSize;

	//���� �������� ���ҵǾ� �ִ� ��忡 � ��ü�� ������ ��ġ�� ������ ��ġ�ϴ°�?
	bool operator == (const Rect& v)
	{
		if (fabs((vMin - v.vMin).Length() < 0.0001f))
		{
			if (fabs((vMax - v.vMax).Length() < 0.0001f))
			{
				return true;
			}
		}
		return false;
	}
	Rect() {}
	Rect(Vector2 min, Vector2 max) 
	{
		vMin = min;
		vMax = max;
		vMiddle = (vMin + vMax) / 2.0f;
		vSize.x = vMax.x - vMin.x;
		vSize.y = vMax.y - vMin.y;
	}
	Rect(Vector2 v, float w, float h) 
	{
		vMin = v;
		vMax = vMin + Vector2(w, h);
		vMiddle = (vMin + vMax) / 2.0f;
		vSize.x = w;
		vSize.y = h;
	}
};
class Collision
{
public:
	static bool RectToPoint(Rect rt, int x, int y);
	static bool RectToPoint(Rect rt, Vector2 v);
	static CollisionType RectToRect(Rect, Rect);
	static Rect UnionRect(Rect rt1, Rect rt2);
	static bool IntersectRect(Rect rt1, Rect rt2, Rect* rt);
};

