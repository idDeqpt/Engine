#ifndef PHYSICS_MANAGER_STATIC_CLASS_HEADER
#define PHYSICS_MANAGER_STATIC_CLASS_HEADER

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
	};
}

#endif //PHYSICS_MANAGER_STATIC_CLASS_HEADER