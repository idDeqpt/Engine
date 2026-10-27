#include <Engine/Core/Node3D.hpp>

#include <Engine/Math/Transform3.hpp>
#include <Engine/Math/Vec3.hpp>


namespace eng::core
{

Node3D::Node3D():
	m_scale(mth::Vec3(1)),
	m_transform_need_update(true),
	m_invert_transform_need_update(true), Node() {}


void Node3D::move(const mth::Vec3& offset)
{
	m_position += offset;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void Node3D::relativeMove(const mth::Vec3& offset)
{
	move(m_rotation.rotateVec(offset));
}

void Node3D::scale(const mth::Vec3& scale_v)
{
	m_scale.x *= scale_v.x;
	m_scale.y *= scale_v.y;
	m_scale.z *= scale_v.z;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void Node3D::rotate(const mth::Quaternion& quat)
{
	m_rotation = m_rotation*quat;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


void Node3D::setOrigin(const mth::Vec3& new_origin)
{
	m_origin = new_origin;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void Node3D::setPosition(const mth::Vec3& new_position)
{
	m_position = new_position;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void Node3D::setScale(const mth::Vec3& new_scale)
{
	m_scale = new_scale;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}

void Node3D::setRotation(const mth::Quaternion& quat)
{
	m_rotation = quat;
	m_transform_need_update = true;
	m_invert_transform_need_update = true;
}


mth::Vec3 Node3D::getPosition()
{
	return m_position;
}

mth::Quaternion Node3D::getRotation()
{
	return m_rotation;
}


mth::Transform3 Node3D::getLocalTransform()
{
	computeTransform();
	return m_transform;
}

mth::Transform3 Node3D::getInvertLocalTransform()
{
	if (m_invert_transform_need_update)
	{
		computeTransform();
		

		m_invert_transform_need_update = false;
	}
	return m_invert_transform;
}

mth::Transform3 Node3D::getGlobalTransform()
{
	if (auto nearest = getNearestParent<Node3D>())
		return nearest->getGlobalTransform().getMatrix()*getLocalTransform().getMatrix();
	return getLocalTransform();
}



void Node3D::computeTransform()
{
	if (m_transform_need_update)
	{
		m_transform = mth::Mat4::getIdentity();
		m_transform.translate(m_position);
		m_transform.rotate(m_rotation);
		m_transform.scale(m_scale);
		m_transform.translate(-m_origin);

		m_transform_need_update = false;
	}
}

} //namespace eng::core