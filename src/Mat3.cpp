#include <Math/Mat3.hpp>

#include <stdexcept>


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