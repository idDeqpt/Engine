#include <Engine/Physics/2D/StaticBody2D.hpp>

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/RigidBody2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>


namespace eng
{

phy::StaticBody2D::StaticBody2D() : phy::PhysicsBody2D() {}


void phy::StaticBody2D::updateState(float delta)
{

}


void phy::StaticBody2D::resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other)
{
	other.resolveCollisionVelWithStatic(data, *this);
}

void phy::StaticBody2D::resolveCollisionVelWithRigid(const CollisionData& data, RigidBody2D& other)
{
	other.resolveCollisionVelWithStatic(data, *this);
}

void phy::StaticBody2D::resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other)
{
	other.resolveCollisionPosWithStatic(data, iter_ratio, *this);
}

void phy::StaticBody2D::resolveCollisionPosWithRigid(const CollisionData& data, float iter_ratio, RigidBody2D& other)
{
	other.resolveCollisionPosWithStatic(data, iter_ratio, *this);
}

} //namespace eng