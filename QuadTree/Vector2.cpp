#include "Vector2.h"
Vector2:: Vector2()
{
	x = y = 0;
}
Vector2:: Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
Vector2:: Vector2(const Vector2& v)
{
	x = v.x;
	y = v.y;
}
Vector2 Vector2:: operator + (const Vector2& v)
{
	Vector2 vec;
	vec.x = this->x + v.x;
	vec.y = this->y + v.y;
	return vec;
}
Vector2 Vector2:: operator - (const Vector2& v)
{
	Vector2 vec;
	vec.x = this->x - v.x;
	vec.y = this->y - v.y;
	return vec;
}
Vector2 Vector2:: operator * (float num)
{
	Vector2 vec;
	vec.x = this->x * num;
	vec.y = this->y * num;
	return vec;
}
Vector2 Vector2:: operator / (float num)
{
	Vector2 vec;
	vec.x = this->x / num;
	vec.y = this->y / num;
	return vec;
}
bool Vector2:: operator == (const Vector2& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			return true;
		}
	}
	return false;
}
bool Vector2:: operator != (const Vector2& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			return false;
		}
	}
	return true;
}
float Vector2:: Length()
{
	float d = sqrt(pow((x), 2) + pow((y), 2));
	return d;
}
//정규화, this 안바뀜
Vector2 Vector2:: Normal()
{
	Vector2 uvec;
	float d = 1.0f / Length();

	uvec.x = x * d;
	uvec.y = y * d;

	return uvec;
}
//자기 자신 정규화, this 바뀜
Vector2 Vector2:: Normalize()
{
	float d = 1.0f / Length();

	x = x * d;
	y = y * d;

	return *this;
}

Vector2 Normalize(Vector2& a)
{
	Vector2 uvec;
	float d = 1.0f / a.Length();

	uvec.x = a.x * d;
	uvec.y = a.y * d;

	return uvec;
}