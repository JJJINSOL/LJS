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
float Vector::Length()
{
	float len = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	return len;
}