#ifndef COLLISION_DETECTOR_2D_CLASS_HEADER
#define COLLISION_DETECTOR_2D_CLASS_HEADER

#include <Engine/Physics/2D/CollisionData.hpp>
#include <vector>

namespace eng::phy
{
	class Collider2D;
	class PhysicsBody2D;

	class CollisionDetector2D
	{
	public:
		virtual void updateCollisions(const std::vector<PhysicsBody2D*>& bodies) = 0;
		std::vector<CollisionData>& getCollisions();

	protected:
		std::vector<CollisionData> m_collisions_buffer;

		static bool checkCollisionAABB(Collider2D& first, Collider2D& second);
	};
}

#endif //COLLISION_DETECTOR_2D_CLASS_HEADER