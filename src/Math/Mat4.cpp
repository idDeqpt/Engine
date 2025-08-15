#include <Engine/Math/Mat4.hpp>

#include <Engine/Math/Mat3.hpp>

#include <stdexcept>


mth::Mat4 mth::Mat4::getIdentity()
{
	return Mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}



mth::Mat4::Mat4() : Mat4(0) {}

mth::Mat4::Mat4(float v)
{
	for (unsigned int i = 0; i < 16; i++)
		values[i] = v;
}

mth::Mat4::Mat4(const Mat4& mat)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
		(*this)[i][j] = mat[i][j];
}

mth::Mat4::Mat4(float v00, float v01, float v02, float v03,
				float v10, float v11, float v12, float v13,
				float v20, float v21, float v22, float v23,
				float v30, float v31, float v32, float v33):
	values{v00, v01, v02, v03,
		   v10, v11, v12, v13,
		   v20, v21, v22, v23,
		   v30, v31, v32, v33} {}


float mth::Mat4::det() const
{
	const Mat4& self = *this;
	float det = 0;
	for (unsigned int i = 0; i < 4; i++)
	{
		Mat3 subMatrix;
		for (unsigned int j = 1; j < 4; j++)
			for (unsigned int k = 0; k < 4; k++)
			{
				if (k < i)
					subMatrix[j - 1][k] = self[j][k];
				else if (k > i)
					subMatrix[j - 1][k - 1] = self[j][k];
			}
		det += (i % 2 == 0 ? 1 : -1) * self[0][i] * subMatrix.det();
	}
	return det;
}

bool mth::Mat4::invert(Mat4& out)
{
	Mat4 self(*this);
	Mat4 I = getIdentity();

	for (unsigned int i = 0; i < 4; i++)
	{
		float pivot = self[i][i];
		if (pivot == 0)
			return false;
		float pivot_inv = 1.0/pivot;

		for (unsigned int j = 0; j < 4; j++)
		{
			self[i][j] *= pivot_inv;
			I[i][j] *= pivot_inv;
		}

		for (unsigned int k = 0; k < 4; k++)
			if (k != i)
			{
				float factor = self[k][i];
				for (unsigned int j = 0; j < 4; j++)
				{
					self[k][j] -= factor * self[i][j];
					I[k][j] -= factor * I[i][j];
				}
			}
	}
	out = I;
	return true;
}


float* mth::Mat4::getValuesPtr()
{
	return values;
}


const mth::Mat4::Row mth::Mat4::operator[](unsigned int index) const
{
	if (index >= 4)
		throw std::out_of_range("Mat4 const index");
	return Mat4::Row(const_cast<float*>(&values[index*4]));
}

mth::Mat4::Row mth::Mat4::operator[](unsigned int index)
{
	if (index >= 4)
		throw std::out_of_range("Mat4 index");
	return Mat4::Row(const_cast<float*>(&values[index*4]));
}

mth::Mat4& mth::Mat4::operator=(const Mat4& mat)
{
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			this->operator[](i)[j] = mat[i][j];
	return *this;
}



mth::Mat4::Row::Row(float* values) : values(values) {}

float mth::Mat4::Row::operator[](unsigned int index) const
{
	if (index >= 4)
		throw std::out_of_range("Mat4 Row const index");
	return values[index];
}

float& mth::Mat4::Row::operator[](unsigned int index)
{
	if (index >= 4)
		throw std::out_of_range("Mat4 Row index");
	return values[index];
}



mth::Mat4 mth::operator*(const Mat4& left, const Mat4& right)
{
	Mat4 result;
	for (unsigned int i = 0; i < 4; i++)
		for (unsigned int j = 0; j < 4; j++)
			for (unsigned int k = 0; k < 4; k++)
				result[i][j] += left[i][k]*right[k][j];
	return result;
}