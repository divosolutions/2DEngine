#pragma once

#include "jansson.h"

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
	Vector2& operator=(const Vector2&);
	Vector2& operator+=(const Vector2&);

	json_t* serialize(void);
	int deserialize(json_t*);
};

