#ifndef PHYSICS_MANAGER_STATIC_CLASS_HEADER
#define PHYSICS_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <vector>
#include <thread>

namespace eng::phy
{
	class PhysicsManager
	{
	public:
		static void initialize();

		static void setThreadsCount(unsigned int count);

		static void addBody(PhysicsBody2D& coll);

		static void update(float delta);

	protected:
		static std::vector<PhysicsBody2D*> s_bodies2d;
		static std::vector<CollisionData> s_collisions_buffer;
		static unsigned int s_threads_count;
		static std::vector<std::thread> s_threads;
		static std::vector<unsigned int> s_threads_last_indexes;
		static void (*s_update_collisions_handler)();

		static bool checkCollisionAABB(Collider2D& first, Collider2D& second);
		static void updateCollisionsSingleThread();
		static void updateCollisionsMultiThread();
		static void updateThreadsIndexes();
	};
}

#endif //PHYSICS_MANAGER_STATIC_CLASS_HEADER