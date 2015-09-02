#include "jansson.h"
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

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

json_t* Vector2::serialize(void)
{
	json_t *root = json_object();

	json_object_set_new(root, "x", json_real(x));
	json_object_set_new(root, "y", json_real(y));

	return root;
}

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

int Vector2::deserialize(json_t *root)
{
	x = (float) json_real_value(json_object_get(root, "x"));
	y = (float) json_real_value(json_object_get(root, "y"));

	return 0;
}
