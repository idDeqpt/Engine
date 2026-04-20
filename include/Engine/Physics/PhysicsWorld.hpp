#ifndef PHYSICS_WORLD_CLASS_HEADER
#define PHYSICS_WORLD_CLASS_HEADER

#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <vector>
#include <thread>

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
		std::vector<CollisionData>  m_collisions_buffer;
		unsigned int m_threads_count;
		std::vector<std::thread>  m_threads;
		std::vector<unsigned int> m_threads_last_indexes;
		void (PhysicsWorld::*m_update_collisions_handler)();

		bool checkCollisionAABB(Collider2D& first, Collider2D& second);
		void updateCollisionsSingleThread();
		void updateCollisionsMultiThread();
		void updateThreadsIndexes();
	};
}

#endif //PHYSICS_WORLD_CLASS_HEADER