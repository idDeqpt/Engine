#include <Math/Transformable2.hpp>

#include <Math/Transform2.hpp>
#include <Math/Vec2.hpp>


mth::Transformable2::Transformable2() : mth::Transformable2::Transformable2(nullptr) {}

mth::Transformable2::Transformable2(Transformable2* parent)
{
	m_parent = parent;
	m_scale = Vec2(1);
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void mth::Transformable2::move(const Vec2& offset)
{
	m_position += offset;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::scale(const Vec2& scale_v)
{
	m_scale += scale_v;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::rotate(float angle)
{
	m_rotation += angle;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void mth::Transformable2::setOrigin(const Vec2& new_origin)
{
	m_origin = new_origin;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::setPosition(const Vec2& new_position)
{
	m_position = new_position;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::setScale(const Vec2& new_scale)
{
	m_scale = new_scale;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::setRotation(float new_angle)
{
	m_rotation = new_angle;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable2::setParent(Transformable2& parent)
{
	m_parent = &parent;
}


mth::Vec2 mth::Transformable2::getPosition()
{
	return m_position;
}


mth::Transformable2* mth::Transformable2::getParent()
{
	return m_parent;
}

mth::Transform2 mth::Transformable2::getLocalTransform()
{
	computeTransform();
	return m_transform;
}

mth::Transform2 mth::Transformable2::getGlobalTransform()
{
	if (m_parent == nullptr)
		return getLocalTransform();
	return m_parent->getGlobalTransform().getMatrix()*getLocalTransform().getMatrix();
}

mth::Transform2 mth::Transformable2::getInvertLocalTransform()
{
	if (m_invert_transform_need_update)
	{
		computeTransform();
		

		m_invert_transform_need_update = false;
	}
	return m_invert_transform;
}



void mth::Transformable2::computeTransform()
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