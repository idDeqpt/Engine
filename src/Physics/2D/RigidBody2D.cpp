#include <Engine/Physics/2D/RigidBody2D.hpp>

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>


namespace eng
{

phy::RigidBody2D::RigidBody2D():
	m_mass(1),
	m_damping(1),
	phy::PhysicsBody2D() {}


void phy::RigidBody2D::impulse(const mth::Vec2 impulse)
{
	m_linear_velocity += impulse;
}

void phy::RigidBody2D::addForce(const mth::Vec2 force)
{
	m_accumulated_force += force;
}


void phy::RigidBody2D::setMass(float mass)
{
	m_mass = mass;
}


mth::Vec2 phy::RigidBody2D::getLinearVelocity()
{
	return m_linear_velocity;
}


float phy::RigidBody2D::getMass()
{
	return m_mass;
}


void phy::RigidBody2D::update(float delta)
{
	if ((delta <= 0) || (m_mass == 0)) return;

	mth::Vec2 acceleration = m_accumulated_force/m_mass;
	
	m_linear_velocity += acceleration*delta;
	m_linear_velocity = m_linear_velocity*m_damping;
	
	move(m_linear_velocity*delta);
	m_accumulated_force = mth::Vec2(0);
}

void phy::RigidBody2D::onCollision(const CollisionData& data)
{
	if (!data.has_collision) return;

	core::Logger::info(getNamePath() + " collided with " + data.bodies[1]->getNamePath());

	mth::Vec2 neg_normal = -data.normal;
	
	float mass_coef = 1;
	if (data.bodies[1]->getMass())
		mass_coef = data.bodies[1]->getMass()/(m_mass + data.bodies[1]->getMass());
	
	if (data.penetration_depth)
    {
        mth::Vec2 position_correction = neg_normal*(data.penetration_depth*mass_coef);
        move(position_correction);
    }

	mth::Vec2 relative_velocity = getLinearVelocity() - data.bodies_velocities[1];
	float velocity_along_normal = relative_velocity.x*neg_normal.x + relative_velocity.y*neg_normal.y; //dot
	if (velocity_along_normal > 0) return;

	float restitution = 0.6f;
	float impulse_magnitude = -(1 + restitution)*velocity_along_normal;

	float self_impulse_share = impulse_magnitude * (mass_coef);
	
	impulse(neg_normal*self_impulse_share);
}

} //namespace eng
