#include <Engine/Physics/PhysicsManager.hpp>

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>

#include <vector>


namespace eng
{

std::vector<phy::PhysicsBody2D*> phy::PhysicsManager::s_bodies2d;


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
	std::vector<CollisionData> collisions;
	collisions.reserve(s_bodies2d.size());

	for (unsigned int i = 0; i < (s_bodies2d.size() - 1); i++)
		for (unsigned int j = i + 1; j < s_bodies2d.size(); j++)
		{
			CollisionData data = s_bodies2d[i]->getCollider()->collideWith(*s_bodies2d[j]->getCollider());
			if (data.has_collision)
			{
				data.bodies[0] = s_bodies2d[i];
				data.bodies[1] = s_bodies2d[j];
				collisions.push_back(data);
			}
		}

	for (unsigned int i = 0; i < collisions.size(); i++)
	{
		collisions[i].bodies[0]->onCollision(collisions[i]);
		collisions[i].bodies[1]->onCollision(collisions[i].swap());
	}

	for (unsigned int i = 0; i < s_bodies2d.size(); i++)
		s_bodies2d[i]->update(delta);
}

} //namespace eng