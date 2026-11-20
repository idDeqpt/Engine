#include <Engine/Graphics/2D/Camera2D.hpp>

#include <Engine/Core/Node2D.hpp>
#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>


namespace eng::gfx
{

Camera2D::Camera2D() : core::Node2D()
{
	setSize(mth::Vec2(1));
}


void Camera2D::setSize(const mth::Vec2& new_size)
{
	setRect(-new_size.x*0.5, new_size.x*0.5, -new_size.y*0.5, new_size.y*0.5);
}

void Camera2D::setRect(const float& left, const float& right, const float& bottom, const float& top)
{
	float width  = right - left;
	float height = top - bottom;

	m_projection = mth::Mat3({
		2.0f/width,      0,             -(right + left)/width,
		0,              -2.0f/height,    (top + bottom)/height,
		m_perspective.x, m_perspective.y, 1
	});
}

void Camera2D::setPerspective(const mth::Vec2& perspective)
{
	m_perspective = perspective;

	m_projection[2][0] = m_perspective.x;
	m_projection[2][1] = m_perspective.y;
}


mth::Mat3 Camera2D::getProjectionMatrix()
{
	return m_projection;
}

mth::Mat3 Camera2D::getViewMatrix()
{
	mth::Mat3 view_mat;
	mth::invert(getGlobalTransform().getMatrix(), view_mat);
	return view_mat;
}

mth::Mat3 Camera2D::getProjViewMatrix()
{
	return getProjectionMatrix()*getViewMatrix();
}

mth::Vec2 Camera2D::convertWindowPoint(const mth::Vec2& point, const RenderTarget& rt)
{
	mth::Vec2 vp_pos  = rt.getViewportPosition();
	mth::Vec2 vp_size = rt.getViewportSize();

	if (vp_size.x <= 0 || vp_size.y <= 0)
		return point;

	float ndc_x = 2*(point.x - vp_pos.x)/vp_size.x - 1;
	float ndc_y = 1 - 2*(point.y - vp_pos.y)/vp_size.y;

	mth::Vec3 ndc_point(ndc_x, ndc_y, 1);

	mth::Mat3 invProjView;
	if (!mth::invert(getProjViewMatrix(), invProjView))
		return mth::Vec2(0);

	mth::Vec3 world_point = invProjView*ndc_point;
	if (std::abs(world_point.z) > 1e-6f)
		return mth::Vec2(world_point.x/world_point.z, world_point.y/world_point.z);
	return mth::Vec2(0);
}

} //namespace eng::gfx