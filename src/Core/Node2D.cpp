#include <Engine/Core/Node2D.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>


namespace eng
{

core::Node2D::Node2D():
	m_rotation(0),
	m_scale(mth::Vec2(1)),
	m_transform_need_update(true),
	m_invert_transform_need_update(true), core::Node() {}


void core::Node2D::move(const mth::Vec2& offset)
{
	m_position += offset;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void core::Node2D::scale(const mth::Vec2& scale_v)
{
	m_scale += scale_v;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void core::Node2D::rotate(float angle)
{
	m_rotation += angle;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void core::Node2D::setOrigin(const mth::Vec2& new_origin)
{
	m_origin = new_origin;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void core::Node2D::setPosition(const mth::Vec2& new_position)
{
	m_position = new_position;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void core::Node2D::setScale(const mth::Vec2& new_scale)
{
	m_scale = new_scale;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void core::Node2D::setRotation(float new_angle)
{
	m_rotation = new_angle;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


mth::Vec2 core::Node2D::getOrigin()
{
	return m_origin;
}

mth::Vec2 core::Node2D::getPosition()
{
	return m_position;
}


mth::Vec2 core::Node2D::getGlobalPosition()
{
	if (!m_parent) return m_position;

	mth::Vec3 pos = getGlobalTransform2D().value().getMatrix()*mth::Vec3(m_position.x, m_position.y, 1);
	return mth::Vec2(pos.x, pos.y);
}

float core::Node2D::getGlobalRotation()
{
	if (!m_parent) 
		return m_rotation;
	
	Node2D* parent_2d = dynamic_cast<Node2D*>(m_parent);
	
	if (parent_2d)
		return parent_2d->getGlobalRotation() + m_rotation;
	
	return m_rotation;
}


mth::Transform2 core::Node2D::getLocalTransform()
{
	computeTransform();
	return m_transform;
}

mth::Transform2 core::Node2D::getInvertLocalTransform()
{
	if (m_invert_transform_need_update)
	{
		computeTransform();
		m_invert_transform_need_update = false;
	}
	return m_invert_transform;
}

std::optional<mth::Transform2> core::Node2D::getGlobalTransform2D()
{
	auto parentOpt = m_parent ? m_parent->getGlobalTransform2D() : std::nullopt;
	if (parentOpt)
		return parentOpt.value().getMatrix()*getLocalTransform().getMatrix();
	else
		return getLocalTransform();
}



void core::Node2D::computeTransform()
{
	if (m_transform_need_update)
	{
		m_transform = mth::Mat3::getIdentity();
		m_transform.translate(m_position);
		m_transform.rotate(m_rotation);
		m_transform.scale(m_scale);
		m_transform.translate(-m_origin);

		m_transform_need_update = false;
	}
}

} //namespace eng