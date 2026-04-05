#include <Engine/Physics/PhysicsManager.hpp>

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>

#include <vector>


namespace eng
{

std::vector<phy::PhysicsBody2D*> phy::PhysicsManager::s_bodies2d;
std::vector<phy::CollisionData> phy::PhysicsManager::s_collisions_buffer;


void phy::PhysicsManager::initialize()
{
	s_bodies2d.clear();
}


void phy::PhysicsManager::addBody(PhysicsBody2D& body)
{
	auto it = std::find(s_bodies2d.begin(), s_bodies2d.end(), &body);
	if (it == s_bodies2d.end())
		s_bodies2d.push_back(&body);
}


void phy::PhysicsManager::update(float delta)
{
	updateCollisions();

	unsigned int VELOCITY_ITERATIONS = 4;
	for (unsigned int i = 0; i < VELOCITY_ITERATIONS; i++)
		for (unsigned int j = 0; j < s_collisions_buffer.size(); j++)
			s_collisions_buffer[j].bodies[0]->resolveCollisionVelWith(s_collisions_buffer[j], *s_collisions_buffer[j].bodies[1]);

	for (unsigned int i = 0; i < s_bodies2d.size(); i++)
		s_bodies2d[i]->updateState(delta);

	unsigned int POSITION_ITERATIONS = 4;
	for (unsigned int p = 0; p < POSITION_ITERATIONS; p++)
	{
		updateCollisions();
		
		if (s_collisions_buffer.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < s_collisions_buffer.size(); i++)
			s_collisions_buffer[i].bodies[0]->resolveCollisionPosWith(s_collisions_buffer[i], iter_ratio, *s_collisions_buffer[i].bodies[1]);
	}
}


bool phy::PhysicsManager::checkCollisionAABB(Collider2D& first, Collider2D& second)
{
    Collider2D::AABB aabb1 = first.getAABB();
    Collider2D::AABB aabb2 = second.getAABB();
    
    if (aabb1.max.x < aabb2.min.x || aabb2.max.x < aabb1.min.x)
        return false;
    
    if (aabb1.max.y < aabb2.min.y || aabb2.max.y < aabb1.min.y)
        return false;
    
    return true;
}


void phy::PhysicsManager::updateCollisions()
{
	s_collisions_buffer.clear();
	s_collisions_buffer.reserve(s_bodies2d.size());

	for (unsigned int i = 0; i < (s_bodies2d.size() - 1); i++)
		for (unsigned int j = i + 1; j < s_bodies2d.size(); j++)
		{
			if (checkCollisionAABB(*s_bodies2d[i]->getCollider(), *s_bodies2d[j]->getCollider()))
				continue;

			CollisionData data = s_bodies2d[i]->getCollider()->collideWith(*s_bodies2d[j]->getCollider());
			if (data.has_collision)
			{
				data.bodies[0] = s_bodies2d[i];
				data.bodies[1] = s_bodies2d[j];
				s_collisions_buffer.push_back(data);
			}
		}
}

} //namespace eng