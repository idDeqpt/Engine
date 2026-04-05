#ifndef COLLISION_DATA_2D_CLASS_HEADER
#define COLLISION_DATA_2D_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>

namespace eng::phy
{
	class Collider2D;
	class PhysicsBody2D;

	struct CollisionData
	{
		bool has_collision = false;
		float penetration_depth = 0;
		mth::Vec2 normal;
		mth::Vec2 contact_point;

		Collider2D*    colliders[2] = {nullptr, nullptr};
		PhysicsBody2D* bodies[2]    = {nullptr, nullptr};

		CollisionData() = default;
		CollisionData(const CollisionData& cd);
		CollisionData swapped() const;
	};
}

#endif //COLLISION_DATA_2D_CLASS_HEADER