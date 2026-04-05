#include <Engine/Physics/2D/RigidBody2D.hpp>

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/StaticBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng
{

phy::RigidBody2D::RigidBody2D():
	m_mass(1),
	m_mass_inv(1),
	m_damping(0.99),
	phy::PhysicsBody2D() {}


void phy::RigidBody2D::applyImpulse(const mth::Vec2 impulse)
{
	m_linear_velocity += impulse*m_mass_inv;
}

void phy::RigidBody2D::applyForce(const mth::Vec2 force)
{
	m_accumulated_force += force;
}


void phy::RigidBody2D::setMass(float mass)
{
	m_mass = mass;
	m_mass_inv = (mass > 0) ? 1.0/mass : 0;
}


float phy::RigidBody2D::getMass()
{
	return m_mass;
}

float phy::RigidBody2D::getMassInv()
{
	return m_mass_inv;
}

mth::Vec2 phy::RigidBody2D::getLinearVelocity()
{
	return m_linear_velocity;
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

	float inv_mass_a = getMassInv();
	float inv_mass_b = other.getMassInv();
	float inv_mass_sum = inv_mass_a + inv_mass_b;
	
	if (inv_mass_sum == 0) return;

	mth::Vec2 relative_velocity = getLinearVelocity() - other.getLinearVelocity();
	float velocity_along_normal = relative_velocity.dot(data.normal);
	
	if (velocity_along_normal < 0) return;

	float rest_a = getRestitution();
	float rest_b = other.getRestitution();
	float restitution = (rest_a < rest_b) ? rest_a : rest_b;
	float impulse_magnitude = -(1 + restitution)*velocity_along_normal;
	impulse_magnitude /= inv_mass_sum;

	mth::Vec2 impulse_v = data.normal*impulse_magnitude;
	
	if (inv_mass_a > 0)
		applyImpulse(impulse_v);
	if (inv_mass_b > 0)
		other.applyImpulse(-impulse_v);
}

void phy::RigidBody2D::resolveCollisionVelWithStatic(const CollisionData& d, StaticBody2D& other)
{
	if (!d.has_collision) return;

	CollisionData data;
	if (d.bodies[0] == this)
		data = d;
	else if (d.bodies[1] == this)
		data = d.swapped();
	else
		return;
	
	float inv_mass_a = getMassInv();
	if (inv_mass_a == 0) return;

	mth::Vec2 relative_velocity = getLinearVelocity();
	float velocity_along_normal = relative_velocity.dot(data.normal);
	
	if (velocity_along_normal < 0) return;

	float rest_a = getRestitution();
	float rest_b = other.getRestitution();
	float restitution = (rest_a < rest_b) ? rest_a : rest_b;
	float impulse_magnitude = -(1 + restitution)*velocity_along_normal;
	impulse_magnitude /= inv_mass_a;

	mth::Vec2 impulse_v = data.normal*impulse_magnitude;
	
	if (inv_mass_a > 0)
		applyImpulse(impulse_v);
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

	float inv_mass_sum = getMassInv() + other.getMassInv();
	
	if (inv_mass_sum == 0) return;

	const float ALLOWED_PENETRATION = 0.001f;
	if (data.penetration_depth > ALLOWED_PENETRATION)
	{
		float penetration = data.penetration_depth - ALLOWED_PENETRATION;
		mth::Vec2 correction = data.normal*(penetration*iter_ratio/inv_mass_sum);
		
		if (getMassInv() > 0)
			move(-correction*getMassInv());
		if (other.getMassInv() > 0)
			other.move(correction*other.getMassInv());
	}
}

void phy::RigidBody2D::resolveCollisionPosWithStatic(const CollisionData& d, float iter_ratio, StaticBody2D& other)
{
	if (!d.has_collision) return;

	CollisionData data;
	if (d.bodies[0] == this)
		data = d;
	else if (d.bodies[1] == this)
		data = d.swapped();
	else
		return;
	
	if (getMassInv() == 0) return;

	const float ALLOWED_PENETRATION = 0.001f;
	if (data.penetration_depth > ALLOWED_PENETRATION)
	{
		float penetration = data.penetration_depth - ALLOWED_PENETRATION;
		mth::Vec2 correction = data.normal*(penetration*iter_ratio/getMassInv());
		
		if (getMassInv() > 0)
			move(-correction*getMassInv());
	}
}

} //namespace eng
