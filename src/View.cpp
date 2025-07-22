#include <Graphics/View.hpp>

#include <Math/Mat4.hpp>
#include <Math/Transformable3.hpp>


gfx::View::View() : mth::Transformable3()
{
	setOrtho(-1, 1, -1, 1, -1, 1);
}

gfx::View::View(const View& view) : mth::Transformable3()
{
	m_projection = m_projection;
	setOrigin(view.m_origin);
	setPosition(view.m_position);
	setScale(view.m_scale);
	setRotation(view.m_rot_vec, view.m_rot_angle);
}


void gfx::View::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float distance = far - near;

	m_projection[0][0] = 2.0/width;
	m_projection[1][1] = 2.0/height;
	m_projection[2][2] = -2.0/distance;

	m_projection[0][3] = -(right + left)/width;
	m_projection[1][3] = -(top + bottom)/height;
	m_projection[2][3] = -(far + near)/distance;

	m_projection[3][3] = 1;
}


mth::Mat4 gfx::View::getProjectionMatrix()
{
	return m_projection;
}