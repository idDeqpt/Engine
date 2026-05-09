#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Physics/2D/SimpleCollisionDetectors2D.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>

#include <unordered_set>
#include <utility>
#include <vector>
#include <memory>


namespace eng
{

struct PairHash
{
	unsigned int operator()(const phy::PhysicsWorld::Pair2D& p) const
	{
		auto h1 = std::hash<phy::PhysicsBody2D*>{}(p.first);
		auto h2 = std::hash<phy::PhysicsBody2D*>{}(p.second);

		if (p.first < p.second)
			return h1 ^ (h2 << 1);
		else
			return h2 ^ (h1 << 1);
	}
};

struct PairEqual
{
	bool operator()(const phy::PhysicsWorld::Pair2D& lhs, const phy::PhysicsWorld::Pair2D& rhs) const
	{
		return (((lhs.first == rhs.first) && (lhs.second == rhs.second)) ||
				((lhs.first == rhs.second) && (lhs.second == rhs.first)));
	}
};


phy::PhysicsWorld::PhysicsWorld():
	m_accumulator(0),
	m_fixed_delta(1/50.0),
	m_max_steps_per_frame(1),
	m_in_step(false)
{
	m_bodies2d.clear();
	setCollisionDetector<BVHCollisionDetector2D>();
	//setCollisionDetector<MultiThreadCollisionDetector2D>(std::thread::hardware_concurrency());
}


void phy::PhysicsWorld::setFixedDelta(float delta)
{
	m_fixed_delta = delta;
}

void phy::PhysicsWorld::setMaxStepsPerFrame(unsigned int steps)
{
	m_max_steps_per_frame = steps;
}


void phy::PhysicsWorld::addBody(PhysicsBody2D& body)
{
	if (m_in_step)
		m_pending_add.push_back(&body);
	else
		if (std::find(m_bodies2d.begin(), m_bodies2d.end(), &body) == m_bodies2d.end())
			m_bodies2d.push_back(&body);
}

void phy::PhysicsWorld::removeBody(PhysicsBody2D& body)
{
	if (m_in_step)
		m_pending_remove.push_back(&body);
	else
	{
		auto it = std::find(m_bodies2d.begin(), m_bodies2d.end(), &body);
		if (it != m_bodies2d.end())
			m_bodies2d.erase(it);

		for (auto it = m_previous_frame_bodies2d.begin(); it != m_previous_frame_bodies2d.end(); )
			if ((it->first == &body) || (it->second == &body))
				it = m_previous_frame_bodies2d.erase(it);
			else
				it++;
	}
}


void phy::PhysicsWorld::update(float delta)
{
	m_accumulator += delta;
	unsigned int steps_count = 0;
	while ((steps_count < m_max_steps_per_frame) && (m_accumulator >= m_fixed_delta))
	{
		step(m_fixed_delta);
		m_accumulator -= m_fixed_delta;
		steps_count++;
	}
}


void phy::PhysicsWorld::step(float delta)
{
	m_in_step = true;
	std::unordered_set<Pair2D, PairHash, PairEqual> current_frame_bodies_set;

	//physics
	m_collision_detector->rebuildTree(m_bodies2d);
	m_collision_detector->updateCollisions();

	std::vector<CollisionData> first_collisions = m_collision_detector->getCollisions();
	for (unsigned int i = 0; i < first_collisions.size(); i++)
		current_frame_bodies_set.insert({first_collisions[i].bodies[0], first_collisions[i].bodies[1]});

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
		m_collision_detector->updateTree();
		m_collision_detector->updateCollisions();
		
		std::vector<CollisionData>& collisions = m_collision_detector->getCollisions();
		for (unsigned int i = 0; i < collisions.size(); i++)
			current_frame_bodies_set.insert({collisions[i].bodies[0], collisions[i].bodies[1]});
		
		if (collisions.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < collisions.size(); i++)
			collisions[i].bodies[0]->resolveCollisionPosWith(collisions[i], iter_ratio, *collisions[i].bodies[1]);
	}

	//triggers
	std::unordered_set<Pair2D, PairHash, PairEqual> prev_set;
	for (const auto& p : m_previous_frame_bodies2d)
		prev_set.insert(p);

	for (const auto& p : prev_set)
	{
		if (p.first->isDestroyed() || p.second->isDestroyed()) continue;
		if (current_frame_bodies_set.find(p) == current_frame_bodies_set.end())
		{
			p.first->onCollisionExit(*p.second);
			p.second->onCollisionExit(*p.first);
		}
	}

	for (const auto& p : current_frame_bodies_set)
	{
		if (p.first->isDestroyed() || p.second->isDestroyed()) continue;
		if (prev_set.find(p) != prev_set.end())
		{
			p.first->onCollisionStay(*p.second);
			p.second->onCollisionStay(*p.first);
		}
		else 
		{
			p.first->onCollisionEnter(*p.second);
			p.second->onCollisionEnter(*p.first);
		}
	}

	m_in_step = false;
	for (auto b : m_pending_add) addBody(*b);
	for (auto b : m_pending_remove)
	{
		for (auto it = current_frame_bodies_set.begin(); it != current_frame_bodies_set.end(); )
			if ((b->isDestroyed()) || (it->first == b) || (it->second == b))
				current_frame_bodies_set.erase(it);
			else
				it++;
		removeBody(*b);
	}

	m_previous_frame_bodies2d.assign(current_frame_bodies_set.begin(), current_frame_bodies_set.end());
}

} //namespace eng