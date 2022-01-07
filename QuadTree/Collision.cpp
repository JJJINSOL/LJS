#include "Collision.h"
bool Collision:: RectToPoint(Rect rt, int x, int y)
{

}
bool Collision::RectToPoint(Rect rt, Vector2 v)
{

}
//두 영역의 합집합
Rect Collision:: UnionRect(Rect rt1, Rect rt2)
{

}
//unionrect를 이용하여 두 영역이 떨어져있지않고, 교집합이 있다면 이제 교집합 구하는 함수
//만약 여기서 교집합이 rt2가 된다면 이 rt2는 rt1 영역안에 포함되어있다는 뜻이 됨
bool Collision:: IntersectRect(Rect rt1, Rect rt2, Rect* rt)
{

}
CollisionType Collision::RectToRect(Rect, Rect)
{

}