#include <Engine/Math/Mat3.hpp>

#include <algorithm>
#include <stdexcept>
#include <cstring>

namespace eng
{

mth::Mat3 mth::Mat3::getIdentity()
{
	return Mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);
}


mth::Mat3::Mat3() : Mat3(0)
{}

mth::Mat3::Mat3(float v)
{
	std::fill(std::begin(values), std::end(values), v);
}

mth::Mat3::Mat3(const Mat3& mat)
{
	std::memcpy(values, mat.values, sizeof(values));
}

mth::Mat3::Mat3(float v00, float v01, float v02,
				float v10, float v11, float v12,
				float v20, float v21, float v22):
	values{v00, v01, v02,
		   v10, v11, v12,
		   v20, v21, v22}
{}


float mth::Mat3::det() const
{
	const Mat3& self = *this;
	return self[0][0] * (self[1][1] * self[2][2] - self[1][2] * self[2][1]) -
		   self[0][1] * (self[1][0] * self[2][2] - self[1][2] * self[2][0]) +
		   self[0][2] * (self[1][0] * self[2][1] - self[1][1] * self[2][0]);
}

bool mth::Mat3::invert(Mat3& out) const
{
	Mat3 self(*this);
	Mat3 I = getIdentity();

	for (unsigned int i = 0; i < 3; i++)
	{
		float pivot = self[i][i];
		if (pivot == 0)
			return false;
		float pivot_inv = 1.0/pivot;

		for (unsigned int j = 0; j < 3; j++)
		{
			self[i][j] *= pivot_inv;
			I[i][j] *= pivot_inv;
		}

		for (unsigned int k = 0; k < 3; k++)
			if (k != i)
			{
				float factor = self[k][i];
				for (unsigned int j = 0; j < 3; j++)
				{
					self[k][j] -= factor * self[i][j];
					I[k][j] -= factor * I[i][j];
				}
			}
	}
	out = I;
	return true;
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

} //namespace eng