#pragma once
#include <iostream>
#include <Math.h>
struct float3
{
	union
	{
		struct { float x, y, z; };
		float v[3];
	};
};
class Vector : public float3
{
public:
	Vector();
	Vector(float x, float y, float z);
	Vector(const Vector& v);

	Vector operator + (const Vector& v);
	Vector operator - (const Vector& v);
	Vector operator * (float num);
	Vector operator / (float num);
	bool operator == (const Vector& v);
	bool operator != (const Vector& v);

	float Length();

	Vector Normal();
	Vector Normalize();
	Vector Normalize(Vector& v);
};

