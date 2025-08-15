#include <Engine/Math/Transform2.hpp>

#include <Engine/Math/Mat3.hpp>
#include <Engine/Math/Vec2.hpp>

#include <cmath>


mth::Transform2::Transform2()
{
	m_matrix = Mat3::getIdentity();
}

mth::Transform2::Transform2(const Mat3& mat)
{
	m_matrix = mat;
}

mth::Transform2::Transform2(Transform2& transform) : Transform2(transform.getMatrix()) {}


void mth::Transform2::translate(const Vec2& vec)
{
	Mat3 translate_m(
		1, 0, vec.x,
		0, 1, vec.y,
		0, 0, 1
	);
	m_matrix = m_matrix*translate_m;
}

void mth::Transform2::scale(const Vec2& vec)
{
	Mat3 scale_m(
		vec.x, 0,     0,
		0,     vec.y, 0,
		0,     0,     1
	);
	m_matrix = m_matrix*scale_m;
}

void mth::Transform2::rotate(float angle)
{
	float cos_angle = std::cos(angle);
	float sin_angle = std::sin(angle);
	Mat3 rotation_m(
		cos_angle, -sin_angle, 0,
		sin_angle,  cos_angle, 0,
		0,          0,         1
	);
	m_matrix = m_matrix*rotation_m;
}


mth::Mat3 mth::Transform2::getMatrix()
{
	return m_matrix;
}