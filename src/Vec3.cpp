#include <Math/Vec3.hpp>

#include <stdexcept>


mth::Vec3::Vec3():
	values{0, 0, 0} {}

mth::Vec3::Vec3(float v):
	values{v, v, v} {}

mth::Vec3::Vec3(float v1, float v2, float v3) :
	values{v1, v2, v3} {}

mth::Vec3::Vec3(const Vec3& vec):
	values{vec.x, vec.y, vec.z} {}


float mth::Vec3::operator[](unsigned int index) const
{
	if (index >= 3)
		throw std::out_of_range("Vector3 const size");
	return values[index];
}

float& mth::Vec3::operator[](unsigned int index)
{
	if (index >= 3)
		throw std::out_of_range("Vector3 size");
	return values[index];
}