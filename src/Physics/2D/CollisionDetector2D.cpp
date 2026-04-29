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

} //namespace eng