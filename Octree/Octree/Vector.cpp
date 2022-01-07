#include "Vector.h"
Vector::Vector()
{
	x = y = z = 0.0f;
}
Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector::Vector(const Vector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

Vector Vector:: operator + (const Vector& v)
{
	Vector vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;
	return vec;
}
Vector Vector:: operator - (const Vector& v)
{
	Vector vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;
	return vec;
}
Vector Vector:: operator * (float num)
{
	Vector vec;
	vec.x = x * num;
	vec.y = y * num;
	vec.z = z * num;
	return vec;
}
Vector Vector:: operator / (float num)
{
	Vector vec;
	vec.x = x / num;
	vec.y = y / num;
	vec.z = z / num;
	return vec;
}
bool Vector:: operator == (const Vector& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			if (fabs(z - v.z) < 0.0001f)
			{
				return true;
			}
		}
	}
	return false;
}
bool Vector:: operator != (const Vector& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			if (fabs(z - v.z) < 0.0001f)
			{
				return false;
			}
		}
	}
	return true;
}

float Vector::Length()
{
	float len = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	return len;
}

Vector Vector::Normal()
{
	Vector vec;
	float d = 1.0f / Length();
	vec.x = x * d;
	vec.y = y * d;
	vec.z = z * d;
	return vec;
}
Vector Vector::Normalize()
{
	float d = 1.0f / Length();
	x = x * d;
	y = y * d;
	z = z * d;
	return *this;
}
Vector Vector::Normalize(Vector& v)
{
	Vector vec;
	float d = 1.0f / Length();
	vec.x = v.x * d;
	vec.y = v.y * d;
	vec.z = v.z * d;
	return vec;
}