#include <Math/Vec4.hpp>

#include <stdexcept>


mth::Vec4::Vec4():
	values{0, 0, 0, 0} {}

mth::Vec4::Vec4(float v):
	values{v, v, v, v} {}

mth::Vec4::Vec4(float v1, float v2, float v3, float v4) :
	values{v1, v2, v3, v4} {}

mth::Vec4::Vec4(const Vec4& vec):
	values{vec.x, vec.y, vec.z, vec.w} {}


float mth::Vec4::operator[](unsigned int index) const
{
	if (index >= 4)
		throw std::out_of_range("Vector4 size");
	return values[index];
}

float& mth::Vec4::operator[](unsigned int index)
{
	if (index >= 4)
		throw std::out_of_range("Vector4 size");
	return values[index];
}