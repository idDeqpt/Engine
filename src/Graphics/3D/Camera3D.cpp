#include <Engine/Graphics/3D/Camera3D.hpp>

#include <Engine/Core/Node3D.hpp>
#include <Engine/Math/Mat4.hpp>

#include <cmath>


namespace eng
{

gfx::Camera3D::Camera3D() : core::Node3D()
{
	setOrtho(-1, 1, -1, 1, -1, 1);
}


void gfx::Camera3D::setOrtho(float left, float right, float bottom, float top, float near, float far)
{
	float width = right - left;
	float height = top - bottom;
	float distance = far - near;

	m_projection = mth::Mat4(
		2.0/width, 0,          0,            -(right + left)/width,
		0,         2.0/height, 0,            -(top + bottom)/height,
		0,         0,         -2.0/distance, -(far + near)/distance,
		0,         0,          0,              1
	);
}

void gfx::Camera3D::setPerspective(float fovy, float aspect_ratio, float near, float far)
{
	float ct = 1.0/tan(fovy/2);
	float distance = far - near;

	m_projection = mth::Mat4(
		ct/aspect_ratio, 0,   0,                     0,
		0,               ct,  0,                     0,
		0,               0, -(far + near)/distance, -1,
		0,               0, -(2*far*near)/distance,  0
	);
}


mth::Mat4 gfx::Camera3D::getProjectionMatrix()
{
	return m_projection;
}

mth::Mat4 gfx::Camera3D::getViewMatrix()
{
	mth::Mat4 view_mat;
	getGlobalTransform3D().value().getMatrix().invert(view_mat);
	return view_mat;
}

mth::Mat4 gfx::Camera3D::getProjViewMatrix()
{
	return getProjectionMatrix()*getViewMatrix();
}

} //namespace eng