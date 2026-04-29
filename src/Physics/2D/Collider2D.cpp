#include <Engine/Physics/2D/Collider2D.hpp>

#include <Engine/Core/Node2D.hpp>


namespace eng
{

void phy::Collider2D::computeTransform()
{
	core::Node2D::computeTransform();
	m_aabb_need_update = true;
}


mth::Vec2 phy::Collider2D::AABB::getCenter()
{
	return (max - min)/2;
}

mth::Vec2 phy::Collider2D::AABB::getSize()
{
	return (max - min);
}

bool phy::Collider2D::AABB::checkCollision(const AABB& other)
{
	if (max.x < other.min.x || other.max.x < min.x)
		return false;
	
	if (max.y < other.min.y || other.max.y < min.y)
		return false;
	
	return true;
}

} //namespace eng