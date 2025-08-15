#include <Engine/Math/Vec2.hpp>


mth::Vec2::Vec2():
	x(0), y(0) {}

mth::Vec2::Vec2(float v):
	x(v), y(v) {}

mth::Vec2::Vec2(float v1, float v2) :
	x(v1), y(v2) {}

mth::Vec2::Vec2(const Vec2& vec):
	x(vec.x), y(vec.y) {}


mth::Vec2& mth::Vec2::operator=(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

mth::Vec2& mth::Vec2::operator+=(const Vec2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

mth::Vec2 mth::Vec2::operator-() const
{
	return Vec2(-x, -y);
}



mth::Vec2 mth::operator+(const Vec2& left, const Vec2& right)
{
	return Vec2(left.x + right.x, left.y + right.y);
}

mth::Vec2 mth::operator-(const Vec2& left, const Vec2& right)
{
	return Vec2(left.x - right.x, left.y - right.y);
}