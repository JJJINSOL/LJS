#pragma once
#include <math.h>

struct Float2
{
	union
	{
		struct
		{
			float x, y;
			float v[2];
		};

	};
};

class Vector2 : public Float2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);

	Vector2 operator + (const Vector2& v);
	//º¤ÅÍ »¬¼À
	Vector2 operator - (const Vector2& v);
	//½ºÄ®¶ó °ö¼À
	Vector2 operator * (float num);

	Vector2 operator / (float num);
	bool operator == (const Vector2& v);
	bool operator != (const Vector2& v);

	//Á¤±ÔÈ­
	Vector2 Normalize();
	Vector2 Normal();
	friend Vector2 Normalize(Vector2& a);

	//Å©±â
	float Length();



};