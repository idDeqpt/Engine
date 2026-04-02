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

		void update(float delta) override;
		void onCollision(const CollisionData& data) override;

	protected:
		float m_mass;
		float m_damping;
		mth::Vec2 m_linear_velocity;
		mth::Vec2 m_accumulated_force;
	};
}

#endif //RIGID_BODY_2D_CLASS_HEADER