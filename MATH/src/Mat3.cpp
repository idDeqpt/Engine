#include <Math/Mat3.hpp>

#include <stdexcept>


mth::Mat3 mth::Mat3::getIdentity()
{
	return Mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);
}


mth::Mat3::Mat3() : Mat3(0) {}

mth::Mat3::Mat3(float v)
{
	for (unsigned int i = 0; i < 9; i++)
		values[i] = v;
}

mth::Mat3::Mat3(const Mat3& mat)
{
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
		(*this)[i][j] = mat[i][j];
}

mth::Mat3::Mat3(float v00, float v01, float v02,
				float v10, float v11, float v12,
				float v20, float v21, float v22):
	values{v00, v01, v02,
		   v10, v11, v12,
		   v20, v21, v22} {}


float mth::Mat3::det() const
{
	const Mat3& self = *this;
	return self[0][0] * (self[1][1] * self[2][2] - self[1][2] * self[2][1]) -
		   self[0][1] * (self[1][0] * self[2][2] - self[1][2] * self[2][0]) +
		   self[0][2] * (self[1][0] * self[2][1] - self[1][1] * self[2][0]);
}


float* mth::Mat3::getValuesPtr()
{
	return values;
}


const mth::Mat3::Row mth::Mat3::operator[](unsigned int index) const
{
	if (index >= 3)
		throw std::out_of_range("Mat3 const index");
	return Mat3::Row(const_cast<float*>(&values[index*3]));
}

mth::Mat3::Row mth::Mat3::operator[](unsigned int index)
{
	if (index >= 3)
		throw std::out_of_range("Mat3 index");
	return Mat3::Row(const_cast<float*>(&values[index*3]));
}



mth::Mat3::Row::Row(float* values) : values(values) {}

float mth::Mat3::Row::operator[](unsigned int index) const
{
	if (index >= 3)
		throw std::out_of_range("Mat3 Row const index");
	return values[index];
}

float& mth::Mat3::Row::operator[](unsigned int index)
{
	if (index >= 3)
		throw std::out_of_range("Mat3 Row index");
	return values[index];
}



mth::Mat3 mth::operator*(const Mat3& left, const Mat3& right)
{
	Mat3 result;
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			for (unsigned int k = 0; k < 3; k++)
				result[i][j] += left[i][k]*right[k][j];
	return result;
}