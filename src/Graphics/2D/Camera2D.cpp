#include <Engine/Graphics/2D/Camera2D.hpp>

#include <Engine/Core/Node2D.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Mat4.hpp>


namespace eng
{

gfx::Camera2D::Camera2D() : core::Node2D()
{
	setSize(mth::Vec2(1));
}


void gfx::Camera2D::setSize(mth::Vec2 new_size)
{
	constexpr float left = 0;
	constexpr float top  = 0;
	float& right  = new_size.x;
	float& bottom = new_size.y;
	float& width  = new_size.x;
	float& height = new_size.y;

	m_projection = mth::Mat3(
		2.0/width,  0,         -1,
		0,         -2.0/height, 1,
		0,          0,          1
	);
}

void gfx::Camera2D::setRect(float left, float right, float bottom, float top)
{
	float width  = right - left;
	float height = top - bottom;

	m_projection = mth::Mat3(
		2.0/width,   0,         -(right + left)/width,
		0,          -2.0/height, (top + bottom)/height,
		0,          0,           1
	);
}


mth::Mat3 gfx::Camera2D::getProjectionMatrix()
{
	return m_projection;
}

mth::Mat3 gfx::Camera2D::getViewMatrix()
{
	mth::Mat3 view_mat;
	getGlobalTransform2D().value().getMatrix().invert(view_mat);
	return view_mat;
}

mth::Mat3 gfx::Camera2D::getProjViewMatrix()
{
	return getProjectionMatrix()*getViewMatrix();
}

mth::Vec2 gfx::Camera2D::convertWindowPoint(const mth::Vec2& point)
{
	const float eps = 1e-6f;
	float width  =  2.0f/m_projection[0][0];
	float height = -2.0f/m_projection[1][1];

	if (std::abs(m_projection[1][1]) < eps || width <= 0 || height <= 0)
	{
		return point;
	}

	float ndc_x = (2.0f*point.x) / width - 1.0f;
	float ndc_y = 1.0f - (2.0f*point.y) / height;

	mth::Vec3 ndc_point(ndc_x, ndc_y, 1.0f);

	mth::Mat3 projView = getProjViewMatrix();
	mth::Mat3 invProjView;
	if (!projView.invert(invProjView))
		return mth::Vec2(0.0f, 0.0f);

	mth::Vec3 world_point = invProjView*ndc_point;
	return mth::Vec2(world_point.x, world_point.y);
}

} //namespace eng