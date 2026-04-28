#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Physics/2D/SimpleCollisionDetectors2D.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Core/Timer.hpp>
#include <Engine/Core/Logger.hpp>

#include <vector>
#include <memory>


namespace eng
{

phy::PhysicsWorld::PhysicsWorld():
	m_accumulator(0),
	m_fixed_delta(1/50.0)
{
	m_bodies2d.clear();
	setThreadsCount(0);
}


void phy::PhysicsWorld::setThreadsCount(unsigned int count)
{
	if (count == 0)
	{
		unsigned int c = std::thread::hardware_concurrency();
		setThreadsCount((c == 0) ? 1 : c);
		return;
	}

	if (count == 1)
		m_collision_detector = std::make_unique<SingleThreadCollisionDetector2D>();
	else
		m_collision_detector = std::make_unique<MultiThreadCollisionDetector2D>(count);
}

void phy::PhysicsWorld::setFixedDelta(float delta)
{
	m_fixed_delta = delta;
}


void phy::PhysicsWorld::addBody(PhysicsBody2D& body)
{
	auto it = std::find(m_bodies2d.begin(), m_bodies2d.end(), &body);
	if (it == m_bodies2d.end())
		m_bodies2d.push_back(&body);
}

void phy::PhysicsWorld::removeBody(PhysicsBody2D& body)
{
	auto it = std::find(m_bodies2d.begin(), m_bodies2d.end(), &body);
	if (it != m_bodies2d.end())
		m_bodies2d.erase(it);
}


void phy::PhysicsWorld::update(float delta)
{
	m_accumulator += delta;
	while (m_accumulator >= m_fixed_delta)
	{
		step(m_fixed_delta);

		m_accumulator -= m_fixed_delta;
	}
}


void phy::PhysicsWorld::step(float delta)
{
	m_collision_detector->updateCollisions(m_bodies2d);
	std::vector<CollisionData> first_collisions = m_collision_detector->getCollisions();

	unsigned int VELOCITY_ITERATIONS = 8;
	for (unsigned int v = 0; v < VELOCITY_ITERATIONS; v++)
		for (unsigned int j = 0; j < first_collisions.size(); j++)
			first_collisions[j].bodies[0]->resolveCollisionVelWith(first_collisions[j], *first_collisions[j].bodies[1]);

	for (unsigned int i = 0; i < m_bodies2d.size(); i++)
		m_bodies2d[i]->integrateVel(delta);

	for (unsigned int i = 0; i < m_bodies2d.size(); i++)
		m_bodies2d[i]->integratePos(delta);
	
	unsigned int POSITION_ITERATIONS = 8;
	for (unsigned int p = 0; p < POSITION_ITERATIONS; p++)
	{
		m_collision_detector->updateCollisions(m_bodies2d);
		std::vector<CollisionData>& collisions = m_collision_detector->getCollisions();
		
		if (collisions.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < collisions.size(); i++)
			collisions[i].bodies[0]->resolveCollisionPosWith(collisions[i], iter_ratio, *collisions[i].bodies[1]);
	}

	for (unsigned int i = 0; i < first_collisions.size(); i++)
	{
		first_collisions[i].bodies[0]->onCollision(*first_collisions[i].bodies[1]);
		first_collisions[i].bodies[1]->onCollision(*first_collisions[i].bodies[0]);
	}
}

} //namespace eng