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

struct BodiesCollection
{
	struct Pair
	{
		phy::PhysicsBody2D* first;
		phy::PhysicsBody2D* second;

		Pair(phy::PhysicsBody2D* f, phy::PhysicsBody2D* s): first(f), second(s) {}
	};

	std::vector<Pair> bodies;

	void add(std::vector<phy::CollisionData>& collisions)
	{
		for (unsigned int i = 0; i < collisions.size(); i++)
			bodies.push_back(Pair(collisions[i].bodies[0], collisions[i].bodies[1]));
	}

	void removeCopies()
	{
		if (bodies.size() < 2) return;

		for (unsigned int i = 0; i < bodies.size() - 1; i++)
			for (unsigned int j = i + 1; j < bodies.size(); j++)
				if (((bodies[j].first  == bodies[i].first)  && (bodies[j].second == bodies[i].second)) ||
					((bodies[j].first  == bodies[i].second) && (bodies[j].second == bodies[i].first)))
						bodies.erase(bodies.begin() + j--);
	}
};


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
	BodiesCollection bodies_collection;

	m_collision_detector->updateCollisions(m_bodies2d);
	std::vector<CollisionData> first_collisions = m_collision_detector->getCollisions();
	bodies_collection.add(first_collisions);

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
		bodies_collection.add(collisions);
		
		if (collisions.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < collisions.size(); i++)
			collisions[i].bodies[0]->resolveCollisionPosWith(collisions[i], iter_ratio, *collisions[i].bodies[1]);
	}

	bodies_collection.removeCopies();
	for (unsigned int i = 0; i < bodies_collection.bodies.size(); i++)
	{
		bodies_collection.bodies[i].first->onCollision(*bodies_collection.bodies[i].second);
		bodies_collection.bodies[i].second->onCollision(*bodies_collection.bodies[i].first);
	}
}

} //namespace eng