#ifndef PHYSICS_MANAGER_STATIC_CLASS_HEADER
#define PHYSICS_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <vector>

namespace eng::phy
{
	class PhysicsManager
	{
	public:
		static void initialize();

		static void addBody(PhysicsBody2D& coll);

		static void update(float delta);

	protected:
		static std::vector<PhysicsBody2D*> s_bodies2d;

		static bool checkCollisionAABB(Collider2D& first, Collider2D& second);
	};
}

#endif //PHYSICS_MANAGER_STATIC_CLASS_HEADER