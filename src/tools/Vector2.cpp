#include "tools/Vector2.h"


Vector2::Vector2()
{
	x = y = 0;
}

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2(const Vector2& v)
{
	x = v.x;
	y = v.y;
}


Vector2::~Vector2()
{}

Vector2 Vector2::operator+(const Vector2& v)
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v)
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const float s)
{
	return Vector2(x*s, y*s);
}
