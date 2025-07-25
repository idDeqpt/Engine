#include <Math/Vec3.hpp>

#include <stdexcept>
#include <cmath>


mth::Vec3::Vec3():
	x(0), y(0), z(0) {}

mth::Vec3::Vec3(float v):
	x(v), y(v), z(v) {}

mth::Vec3::Vec3(float v1, float v2, float v3) :
	x(v1), y(v2), z(v3) {}

mth::Vec3::Vec3(const Vec3& vec):
	x(vec.x), y(vec.y), z(vec.z) {}


float mth::Vec3::len() const
{
	return sqrt(x*x + y*y + z*z);
}

mth::Vec3 mth::Vec3::norm(float new_len) const
{
	float coef = new_len/len();
	return Vec3(x*coef, y*coef, z*coef);
}


float mth::Vec3::operator[](unsigned int index) const
{
	if (index >= 3)
		throw std::out_of_range("Vector3 const size");
	return *(&x + index);
}

float& mth::Vec3::operator[](unsigned int index)
{
	if (index >= 3)
		throw std::out_of_range("Vector3 size");
	return *(&x + index);
}

mth::Vec3& mth::Vec3::operator=(const Vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}

mth::Vec3& mth::Vec3::operator+=(const Vec3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

mth::Vec3 mth::Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}



mth::Vec3 mth::operator*(const Vec3& vec, float coef)
{
	return Vec3(vec.x*coef, vec.y*coef, vec.z*coef);
}

mth::Vec3 mth::operator*(float coef, const Vec3& vec)
{
	return Vec3(vec.x*coef, vec.y*coef, vec.z*coef);
}