#include <Engine/Core/Node2D.hpp>

#include <Engine/Math/Transform2.hpp>
#include <Engine/Math/Vec.hpp>


namespace eng::core
{

void Node2D::onTreeChanged()
{
	m_nearest_2d = getNearestParent<Node2D>();
	invalidateTransform();
}


void Node2D::move(const mth::Vec2& offset)
{
	m_position += offset;
	m_local_transform_need_update = true;
	invalidateTransform();
}

void Node2D::scale(const mth::Vec2& scale_v)
{
	m_scale.x *= scale_v.x;
	m_scale.y *= scale_v.y;
	m_local_transform_need_update = true;
	invalidateTransform();
}

void Node2D::rotate(float angle)
{
	m_rotation += angle;
	m_local_transform_need_update = true;
	invalidateTransform();
}


void Node2D::setOrigin(const mth::Vec2& new_origin)
{
	m_origin = new_origin;
	m_local_transform_need_update = true;
	invalidateTransform();
}

void Node2D::setPosition(const mth::Vec2& new_position)
{
	m_position = new_position;
	m_local_transform_need_update = true;
	invalidateTransform();
}

void Node2D::setScale(const mth::Vec2& new_scale)
{
	m_scale = new_scale;
	m_local_transform_need_update = true;
	invalidateTransform();
}

void Node2D::setRotation(float new_angle)
{
	m_rotation = new_angle;
	m_local_transform_need_update = true;
	invalidateTransform();
}


const mth::Vec2& Node2D::getOrigin() const
{
	return m_origin;
}

const mth::Vec2& Node2D::getPosition() const
{
	return m_position;
}

const mth::Vec2& Node2D::getScale() const
{
	return m_scale;
}

float Node2D::getRotation() const
{
	return m_rotation;
}


mth::Vec2 Node2D::getGlobalPosition()
{
	if (!m_parent) return m_position;

	mth::Vec3 pos = getGlobalTransform().getMatrix()*mth::Vec3(m_position.x, m_position.y, 1);
	return mth::Vec2(pos.x, pos.y);
}

float Node2D::getGlobalRotation()
{
	if (!m_parent) 
		return m_rotation;
	
	if (m_nearest_2d)
		return m_nearest_2d->getGlobalRotation() + m_rotation;
	return m_rotation;
}


const mth::Transform2& Node2D::getLocalTransform()
{
	computeTransform();
	return m_local_transform;
}

const mth::Transform2& Node2D::getGlobalTransform()
{
	if (!m_nearest_2d)
		return getLocalTransform();

	(void)m_nearest_2d->getGlobalTransform();

	if (!m_global_valid || (m_cached_against_revision != m_nearest_2d->m_subtree_revision))
	{
		m_global_transform = m_nearest_2d->getGlobalTransform().getMatrix()*getLocalTransform().getMatrix();
		m_cached_against_revision = m_nearest_2d->m_subtree_revision;
		m_subtree_revision++;
		m_global_valid = true;
	}
	return m_global_transform;
}



void Node2D::computeTransform()
{
	if (m_local_transform_need_update)
	{
		m_local_transform = mth::Mat3::getIdentity();
		m_local_transform.translate(m_position);
		m_local_transform.rotate(m_rotation);
		m_local_transform.scale(m_scale);
		m_local_transform.translate(-m_origin);

		m_local_transform_need_update = false;
	}
}

void Node2D::invalidateTransform()
{
	m_subtree_revision++;
	m_global_valid = false;
}

} //namespace eng::core