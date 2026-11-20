#include <Engine/Math/Transform2.hpp>

#include <Engine/Math/Mat.hpp>
#include <Engine/Math/Vec.hpp>

#include <cmath>


namespace eng
{

mth::Transform2::Transform2(const Mat3& mat):
	m_matrix(mat)
{}


void mth::Transform2::translate(const Vec2& vec)
{
	m_matrix[0][2] += m_matrix[0][0]*vec.x + m_matrix[0][1]*vec.y;
	m_matrix[1][2] += m_matrix[1][0]*vec.x + m_matrix[1][1]*vec.y;
	m_matrix[2][2] += m_matrix[2][0]*vec.x + m_matrix[2][1]*vec.y;
}

void mth::Transform2::scale(const Vec2& vec)
{
	m_matrix[0][0] *= vec.x;
	m_matrix[1][0] *= vec.x;
	m_matrix[2][0] *= vec.x;

	m_matrix[0][1] *= vec.y;
	m_matrix[1][1] *= vec.y;
	m_matrix[2][1] *= vec.y;
}

void mth::Transform2::rotate(float angle)
{
	const float c = std::cos(angle);
	const float s = std::sin(angle);

	const float m00 = m_matrix[0][0];
	const float m10 = m_matrix[1][0];
	const float m20 = m_matrix[2][0];

	m_matrix[0][0] = m00*c + m_matrix[0][1]*s;
	m_matrix[1][0] = m10*c + m_matrix[1][1]*s;
	m_matrix[2][0] = m20*c + m_matrix[2][1]*s;

	m_matrix[0][1] = m_matrix[0][1]*c - m00*s;
	m_matrix[1][1] = m_matrix[1][1]*c - m10*s;
	m_matrix[2][1] = m_matrix[2][1]*c - m20*s;
}


const mth::Mat3& mth::Transform2::getMatrix() const
{
	return m_matrix;
}

} //namespace eng