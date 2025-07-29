#include <Math/Transformable3.hpp>

#include <Math/Transform3.hpp>
#include <Math/Vec3.hpp>


mth::Transformable3::Transformable3() : mth::Transformable3::Transformable3(nullptr) {}

mth::Transformable3::Transformable3(Transformable3* parent)
{
	m_parent = parent;
	m_scale = Vec3(1);
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void mth::Transformable3::move(const Vec3& offset)
{
	m_position += offset;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::relativeMove(const Vec3& offset)
{
	move(m_rotation.rotateVec(offset));
}

void mth::Transformable3::scale(const Vec3& scale_v)
{
	m_scale += scale_v;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::rotate(const Quaternion& quat)
{
	m_rotation = m_rotation*quat;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void mth::Transformable3::setOrigin(const Vec3& new_origin)
{
	m_origin = new_origin;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::setPosition(const Vec3& new_position)
{
	m_position = new_position;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::setScale(const Vec3& new_scale)
{
	m_scale = new_scale;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::setRotation(const Quaternion& quat)
{
	m_rotation = quat;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void mth::Transformable3::setParent(Transformable3& parent)
{
	m_parent = &parent;
}


mth::Vec3 mth::Transformable3::getPosition()
{
	return m_position;
}

mth::Quaternion mth::Transformable3::getRotation()
{
	return m_rotation;
}


mth::Transformable3* mth::Transformable3::getParent()
{
	return m_parent;
}

mth::Transform3 mth::Transformable3::getLocalTransform()
{
	computeTransform();
	return m_transform;
}

mth::Transform3 mth::Transformable3::getGlobalTransform()
{
	if (m_parent == nullptr)
		return getLocalTransform();
	return m_parent->getGlobalTransform().getMatrix()*getLocalTransform().getMatrix();
}

mth::Transform3 mth::Transformable3::getInvertLocalTransform()
{
	if (m_invert_transform_need_update)
	{
		computeTransform();
		

		m_invert_transform_need_update = false;
	}
	return m_invert_transform;
}



void mth::Transformable3::computeTransform()
{
	if (m_transform_need_update)
	{
		m_transform = mth::Transform3::getIdentity();
		m_transform.translate(m_position);
		m_transform.rotate(m_rotation);
		m_transform.scale(m_scale);
		m_transform.translate(-m_origin);

		m_transform_need_update = false;
	}
}