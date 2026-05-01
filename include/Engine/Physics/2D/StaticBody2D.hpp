#ifndef STATIC_BODY_2D_CLASS_HEADER
#define STATIC_BODY_2D_CLASS_HEADER

#include <Engine/Physics/2D/PhysicsBody2D.hpp>

namespace eng::phy
{
	class RigidBody2D;
	class CollisionData;

	class StaticBody2D : public PhysicsBody2D
	{
	public:
		StaticBody2D();

		void updateState(float delta);

		void resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other);
		void resolveCollisionVelWithRigid(const CollisionData& data, RigidBody2D& other);

		void resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other);
		void resolveCollisionPosWithRigid(const CollisionData& data, float iter_ratio, RigidBody2D& other);
	};
}

#endif //STATIC_BODY_2D_CLASS_HEADER