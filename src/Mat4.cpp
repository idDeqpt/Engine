#include <Math/Mat4.hpp>

#include <stdexcept>


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

mth::Mat4& mth::Mat4::operator=(Mat4& mat)
{
	for (unsigned int i = 0; i < 16; i++)
		values[i] = mat.getValuesPtr()[i];
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