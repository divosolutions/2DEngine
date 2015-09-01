#pragma once

class Vector2
{
public:
	float x, y;
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2&);
	~Vector2();

	Vector2 operator+(const Vector2&);
	Vector2 operator-(const Vector2&);
	Vector2 operator*(const float s);
};

