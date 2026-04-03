#include <Engine/Physics/2D/CollisionData.hpp>


namespace eng
{

phy::CollisionData::CollisionData(const CollisionData& cd):
	has_collision(cd.has_collision),
	penetration_depth(cd.penetration_depth),
	normal(cd.normal),
	contact_point(cd.contact_point)
{
	colliders[0] = cd.colliders[0];
	colliders[1] = cd.colliders[1];
	bodies[0] = cd.bodies[0];
	bodies[1] = cd.bodies[1];
	bodies_velocities[0] = cd.bodies_velocities[0];
	bodies_velocities[1] = cd.bodies_velocities[1];
}

phy::CollisionData phy::CollisionData::swapped()
{
	CollisionData copy(*this);
	copy.normal = -normal;
	copy.colliders[0] = colliders[1];
	copy.colliders[1] = colliders[0];
	copy.bodies[0] = bodies[1];
	copy.bodies[1] = bodies[0];
	copy.bodies_velocities[0] = bodies_velocities[1];
	copy.bodies_velocities[1] = bodies_velocities[0];

	return copy;
}

} //namespace eng