#ifndef RIGID_BODY_2D_CLASS_HEADER
#define RIGID_BODY_2D_CLASS_HEADER

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng::phy
{
	class RigidBody2D : public PhysicsBody2D
	{
	public:
		RigidBody2D();

		void impulse(const mth::Vec2 impulse);
		void addForce(const mth::Vec2 force);

		void setMass(float mass);

		mth::Vec2 getLinearVelocity() override;
		float getMass() override;

		void updateState(float delta) override;

		void resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other) override;
		void resolveCollisionVelWithRigid(const CollisionData& data, RigidBody2D& other) override;

		void resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other) override;
		void resolveCollisionPosWithRigid(const CollisionData& data, float iter_ratio, RigidBody2D& other) override;

	protected:
		float m_mass, m_mass_inv;
		float m_damping;
		mth::Vec2 m_linear_velocity;
		mth::Vec2 m_accumulated_force;
	};
}

#endif //RIGID_BODY_2D_CLASS_HEADER