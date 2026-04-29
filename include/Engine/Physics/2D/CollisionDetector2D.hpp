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
		virtual void rebuildTree(const std::vector<PhysicsBody2D*>& bodies) = 0;
		virtual void updateTree(const std::vector<PhysicsBody2D*>& bodies) = 0;

		virtual void updateCollisions() = 0;
		std::vector<CollisionData>& getCollisions();

	protected:
		std::vector<CollisionData> m_collisions_buffer;
	};
}

#endif //COLLISION_DETECTOR_2D_CLASS_HEADER