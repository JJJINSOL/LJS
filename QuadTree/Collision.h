#pragma once
#include "Vector2.h"

enum CollisionType { Rect_Out = 0, Rect_In = 1, Rect_Overlap = 2};
//메인에서 입력된 오브젝트 위치가 이곳이 맞는지 확인
class Rect 
{
	//2차원 영역의 기준 지점 (x,y 값 갖음)
	Vector2 vMin;
	//2차원 영역의 기준 지점이랑 대각선에 있는 지점 (x,y 값 갖음)
	Vector2 vMax;
	//2차원 영역의 min과 max 사이의 지점으로 가운데 지점 (x,y 값 갖음)
	Vector2 vMiddle;
	//가로,세로 길이 (x,y 값 갖음)
	Vector2 vSize;

	//맵의 영역에서 분할되어 있는 노드에 어떤 객체의 영역이 위치가 완전히 일치하는가?
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

