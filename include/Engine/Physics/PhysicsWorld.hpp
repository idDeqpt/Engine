#ifndef PHYSICS_WORLD_CLASS_HEADER
#define PHYSICS_WORLD_CLASS_HEADER

#include <Engine/Physics/2D/CollisionDetector2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>

#include <vector>
#include <memory>


namespace eng::phy
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();

		void setThreadsCount(unsigned int count);

		void addBody(PhysicsBody2D& coll);

		void update(float delta);

	protected:
		std::vector<PhysicsBody2D*> m_bodies2d;
		std::unique_ptr<CollisionDetector2D> m_collision_detector;
	};
}

#endif //PHYSICS_WORLD_CLASS_HEADER