#include <Engine/Physics/2D/CollisionDetector2D.hpp>

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <vector>


namespace eng
{

std::vector<phy::CollisionData>& phy::CollisionDetector2D::getCollisions()
{
	return m_collisions_buffer;
}


bool phy::CollisionDetector2D::checkCollisionAABB(Collider2D& first, Collider2D& second)
{
	Collider2D::AABB aabb1 = first.getAABB();
	Collider2D::AABB aabb2 = second.getAABB();
	
	if (aabb1.max.x < aabb2.min.x || aabb2.max.x < aabb1.min.x)
		return false;
	
	if (aabb1.max.y < aabb2.min.y || aabb2.max.y < aabb1.min.y)
		return false;
	
	return true;
}

} //namespace eng