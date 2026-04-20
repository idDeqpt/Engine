#include <Engine/Physics/PhysicsManager.hpp>

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Core/Timer.hpp>
#include <Engine/Core/Logger.hpp>

#include <vector>
#include <thread>


namespace eng
{

phy::PhysicsManager::PhysicsManager()
{
	m_bodies2d.clear();
	setThreadsCount(0);
}


void phy::PhysicsManager::setThreadsCount(unsigned int count)
{
	if (count == 0)
	{
		unsigned int c = std::thread::hardware_concurrency();
		setThreadsCount((c == 0) ? 1 : c);
		updateThreadsIndexes();
		return;
	}
	m_threads_count = count;
	
	for (unsigned int i = 0; i < m_threads.size(); i++)
		if (m_threads[i].joinable())
			m_threads[i].join();

	if (count == 1)
	{
		m_threads.clear();
		m_update_collisions_handler = &PhysicsManager::updateCollisionsSingleThread;
	}
	else
	{
		m_threads.resize(count);
		m_update_collisions_handler = &PhysicsManager::updateCollisionsMultiThread;
		updateThreadsIndexes();
	}
}


void phy::PhysicsManager::addBody(PhysicsBody2D& body)
{
	auto it = std::find(m_bodies2d.begin(), m_bodies2d.end(), &body);
	if (it == m_bodies2d.end())
		m_bodies2d.push_back(&body);
	if (m_threads_count > 1)
		updateThreadsIndexes();
}


void phy::PhysicsManager::update(float delta)
{
	//core::Timer t, tl;
	(this->*m_update_collisions_handler)();
	//core::Logger::info(core::String("Collisions: ") << tl.getElapsedSeconds());

	//tl.restart();
	unsigned int VELOCITY_ITERATIONS = 4;
	for (unsigned int i = 0; i < VELOCITY_ITERATIONS; i++)
		for (unsigned int j = 0; j < m_collisions_buffer.size(); j++)
			m_collisions_buffer[j].bodies[0]->resolveCollisionVelWith(m_collisions_buffer[j], *m_collisions_buffer[j].bodies[1]);
	//core::Logger::info(core::String("Velocity: ") << tl.getElapsedSeconds());

	//tl.restart();
	for (unsigned int i = 0; i < m_bodies2d.size(); i++)
		m_bodies2d[i]->updateState(delta);
	//core::Logger::info(core::String("Update: ") << tl.getElapsedSeconds());

	//tl.restart();
	unsigned int POSITION_ITERATIONS = 4;
	for (unsigned int p = 0; p < POSITION_ITERATIONS; p++)
	{
		(this->*m_update_collisions_handler)();
		
		if (m_collisions_buffer.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < m_collisions_buffer.size(); i++)
			m_collisions_buffer[i].bodies[0]->resolveCollisionPosWith(m_collisions_buffer[i], iter_ratio, *m_collisions_buffer[i].bodies[1]);
	}
	//core::Logger::info(core::String("Position: ") << tl.getElapsedSeconds());

	//core::Logger::info(core::String("Total: ") << t.getElapsedSeconds());
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


void phy::PhysicsManager::updateCollisionsSingleThread()
{
	m_collisions_buffer.clear();
	m_collisions_buffer.reserve(m_bodies2d.size());

	for (unsigned int i = 0; i < (m_bodies2d.size() - 1); i++)
		for (unsigned int j = i + 1; j < m_bodies2d.size(); j++)
			if (checkCollisionAABB(*m_bodies2d[i]->getCollider(), *m_bodies2d[j]->getCollider()))
			{
				CollisionData data = m_bodies2d[i]->getCollider()->collideWith(*m_bodies2d[j]->getCollider());
				if (data.has_collision)
				{
					data.bodies[0] = m_bodies2d[i];
					data.bodies[1] = m_bodies2d[j];
					m_collisions_buffer.push_back(data);
				}
			}
}


void phy::PhysicsManager::updateCollisionsMultiThread()
{
	m_collisions_buffer.clear();
	m_collisions_buffer.reserve(m_bodies2d.size());
	
	if (m_bodies2d.size() <= 1) return;
	
	std::vector<std::vector<CollisionData>> thread_results(m_threads.size());
	
	auto process_range = [&](unsigned int thread_id, unsigned int start_pair, unsigned int end_pair)
	{
		for (unsigned int i = start_pair; i < end_pair; i++)
			for (unsigned int j = i + 1; j < m_bodies2d.size(); j++)
				if (checkCollisionAABB(*m_bodies2d[i]->getCollider(), *m_bodies2d[j]->getCollider()))
				{
					CollisionData data = m_bodies2d[i]->getCollider()->collideWith(*m_bodies2d[j]->getCollider());
					if (data.has_collision)
					{
						data.bodies[0] = m_bodies2d[i];
						data.bodies[1] = m_bodies2d[j];
						thread_results[thread_id].push_back(data);
					}
				}
	};
	
	for (unsigned int i = 0; i < m_threads.size(); i++)
		m_threads[i] = std::thread(process_range, i, m_threads_last_indexes[i], m_threads_last_indexes[i + 1]);
	
	for (unsigned int i = 0; i < m_threads.size(); i++)
		if (m_threads[i].joinable())
			m_threads[i].join();
	
	for (unsigned int i = 0; i < thread_results.size(); i++)
		m_collisions_buffer.insert(
			m_collisions_buffer.end(),
			thread_results[i].begin(),
			thread_results[i].end()
		);
}

void phy::PhysicsManager::updateThreadsIndexes()
{
	m_threads_last_indexes.resize(m_threads.size() + 1);
	m_threads_last_indexes[0] = 0;

	unsigned int pairs_per_thread = m_bodies2d.size()/m_threads.size();
	unsigned int remainder = m_bodies2d.size()%m_threads.size();
	unsigned int current_start = 0;
	
	for (unsigned int i = 0; i < m_threads.size(); i++)
	{
		unsigned int pairs_for_this_thread = pairs_per_thread + (i < remainder ? 1 : 0);
		unsigned int current_end = current_start + pairs_for_this_thread;
		m_threads_last_indexes[i + 1] = current_end;
		current_start = current_end;
	}
}

} //namespace eng