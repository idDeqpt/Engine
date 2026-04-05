#include <Engine/Math/Vec2.hpp>

#include <cmath>


namespace eng
{

mth::Vec2::Vec2():
	x(0), y(0) {}

mth::Vec2::Vec2(float v):
	x(v), y(v) {}

mth::Vec2::Vec2(float v1, float v2) :
	x(v1), y(v2) {}

mth::Vec2::Vec2(const Vec2& vec):
	x(vec.x), y(vec.y) {}


float mth::Vec2::dot(const Vec2& vec) const
{
	return x*vec.x + y*vec.y;
}

float mth::Vec2::len() const
{
	return sqrt(x*x + y*y);
}

mth::Vec2 mth::Vec2::norm(float new_len) const
{
	float coef = new_len/len();
	return Vec2(x*coef, y*coef);
}


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

mth::Vec2 mth::operator*(const Vec2& vec, float coef)
{
	return Vec2(vec.x*coef, vec.y*coef);
}

mth::Vec2 mth::operator*(float coef, const Vec2& vec)
{
	return Vec2(vec.x*coef, vec.y*coef);
}

mth::Vec2 mth::operator/(const Vec2& vec, float coef)
{
	return Vec2(vec.x/coef, vec.y/coef);
}

} //namespace eng