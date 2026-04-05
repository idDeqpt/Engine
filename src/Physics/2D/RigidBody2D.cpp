#include <Engine/Physics/2D/RigidBody2D.hpp>

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>


namespace eng
{

phy::RigidBody2D::RigidBody2D():
	m_mass(1),
	m_mass_inv(1),
	m_damping(0.99),
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
	m_mass_inv = (mass > 0) ? 1.0/mass : 0;
}


mth::Vec2 phy::RigidBody2D::getLinearVelocity()
{
	return m_linear_velocity;
}


float phy::RigidBody2D::getMass()
{
	return m_mass;
}


void phy::RigidBody2D::updateState(float delta)
{
	if ((delta <= 0) || (m_mass == 0)) return;

	mth::Vec2 acceleration = m_accumulated_force/m_mass;
	
	m_linear_velocity += acceleration*delta;
	m_linear_velocity = m_linear_velocity*m_damping;
	
	move(m_linear_velocity*delta);
	m_accumulated_force = mth::Vec2(0);
}


void phy::RigidBody2D::resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other)
{
	if (!data.has_collision) return;
	other.resolveCollisionVelWithRigid(data, *this);
}

void phy::RigidBody2D::resolveCollisionVelWithRigid(const CollisionData& d, RigidBody2D& other)
{
	if (!d.has_collision) return;

	CollisionData data;
	if (d.bodies[0] == this)
		data = d;
	else if (d.bodies[1] == this)
		data = d.swapped();
	else
		return;

	float inv_mass_sum = m_mass_inv + other.m_mass_inv;
	
	if (inv_mass_sum == 0) return;

	mth::Vec2 relative_velocity = other.getLinearVelocity() - getLinearVelocity();
	float velocity_along_normal = relative_velocity.dot(data.normal);
	
	if (velocity_along_normal > 0) return;

	float restitution = 0.9f;
	float impulse_magnitude = -(1 + restitution) * velocity_along_normal;
	impulse_magnitude /= inv_mass_sum;
	
	constexpr float MAX_IMPULSE = 100.0f;
	impulse_magnitude = (impulse_magnitude < MAX_IMPULSE) ? impulse_magnitude : MAX_IMPULSE;
	impulse_magnitude = (impulse_magnitude > -MAX_IMPULSE) ? impulse_magnitude : -MAX_IMPULSE;

	mth::Vec2 impulse_v = data.normal*impulse_magnitude;
	
	if (m_mass_inv > 0)
		impulse(-impulse_v*m_mass_inv);
	if (other.m_mass_inv > 0)
		other.impulse(impulse_v*other.m_mass_inv);
}


void phy::RigidBody2D::resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other)
{
	if (!data.has_collision) return;
	other.resolveCollisionPosWithRigid(data, iter_ratio, *this);
}

void phy::RigidBody2D::resolveCollisionPosWithRigid(const CollisionData& d, float iter_ratio, RigidBody2D& other)
{
	if (!d.has_collision) return;

	CollisionData data;
	if (d.bodies[0] == this)
		data = d;
	else if (d.bodies[1] == this)
		data = d.swapped();
	else
		return;

	float inv_mass_sum = m_mass_inv + other.m_mass_inv;
	
	if (inv_mass_sum == 0) return;

	const float ALLOWED_PENETRATION = 0.001f;
	if (data.penetration_depth > ALLOWED_PENETRATION)
	{
		float penetration = data.penetration_depth - ALLOWED_PENETRATION;
		mth::Vec2 correction = data.normal*(penetration*iter_ratio/inv_mass_sum);
		
		if (m_mass_inv > 0)
			move(-correction*m_mass_inv);
		if (other.m_mass_inv > 0)
			other.move(correction*other.m_mass_inv);
	}
}

} //namespace eng
