#include <Math/Vec4.hpp>

#include <Math/Mat4.hpp>
#include <stdexcept>


mth::Vec4::Vec4():
	x(0), y(0), z(0), w(0) {}

mth::Vec4::Vec4(float v):
	x(v), y(v), z(v), w(v) {}

mth::Vec4::Vec4(float v1, float v2, float v3, float v4) :
	x(v1), y(v2), z(v3), w(v4) {}

mth::Vec4::Vec4(const Vec4& vec):
	x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}


float mth::Vec4::operator[](unsigned int index) const
{
	if (index >= 4)
		throw std::out_of_range("Vector4 const size");
	return *(&x + index);
}

float& mth::Vec4::operator[](unsigned int index)
{
	if (index >= 4)
		throw std::out_of_range("Vector4 size");
	return *(&x + index);
}



mth::Vec4 mth::operator*(const Mat4& left, const Vec4& right)
{
	Vec4 result;
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			result[i] += left[i][j]*right[j];
	return result;
}