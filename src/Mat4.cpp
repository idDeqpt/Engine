#include <Math/Mat4.cpp>

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
		*(this)[i][j] = mat[i][j];
}


const mth::Mat4::Row mth::Mat4::operator[](unsigned int index) const
{
	if (index >= 4)
		throw std::out_of_range("Mat4 const index");
	return Mat4::Row(values*index);
}

mth::Mat4::Row mth::Mat4::operator[](unsigned int index)
{
	if (index >= 4)
		throw std::out_of_range("Mat4 const index");
	return Mat4::Row(values*index);
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