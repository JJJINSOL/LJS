#pragma once
#include <iostream>
#include <math.h>
struct float2
{
	union
	{
		struct { float x, y; };
		float v[2];
	};
};
class Vector2 : public float2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);

	Vector2 operator + (const Vector2& v);
	Vector2 operator - (const Vector2& v);
	Vector2 operator * (float num);
	Vector2 operator / (float num);
	bool operator == (const Vector2& v);
	bool operator != (const Vector2& v);

	float Length();

	Vector2 Normal();
	Vector2 Normalize();
	friend Vector2 Normalize(Vector2& v);
};