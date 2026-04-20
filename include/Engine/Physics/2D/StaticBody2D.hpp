#ifndef STATIC_BODY_2D_CLASS_HEADER
#define STATIC_BODY_2D_CLASS_HEADER

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng::phy
{
	class RigidBody2D;

	class StaticBody2D : public PhysicsBody2D
	{
	public:
		StaticBody2D() : phy::PhysicsBody2D() {}

		void updateState(float delta) override {};

		void resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other) override {other.resolveCollisionVelWithStatic(data, *this);};
		void resolveCollisionVelWithRigid(const CollisionData& data, RigidBody2D& other) override {other.resolveCollisionVelWithStatic(data, *this);};
		void resolveCollisionVelWithStatic(const CollisionData& data, StaticBody2D& other) override {};

		void resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other) override {other.resolveCollisionPosWithStatic(data, iter_ratio, *this);};
		void resolveCollisionPosWithRigid(const CollisionData& data, float iter_ratio, RigidBody2D& other) override {other.resolveCollisionPosWithStatic(data, iter_ratio, *this);};
		void resolveCollisionPosWithStatic(const CollisionData& data, float iter_ratio, StaticBody2D& other) override {};
	};
}

#endif //STATIC_BODY_2D_CLASS_HEADER