#include <Engine/Physics/2D/SimpleCollisionDetectors2D.hpp>

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/BVH.hpp>

#include <thread>
#include <vector>


namespace eng
{

phy::BVHCollisionDetector2D::BVHCollisionDetector2D():
	m_frame(0) {}


void phy::BVHCollisionDetector2D::rebuildTree(std::vector<PhysicsBody2D*>& bodies)
{
	m_bvh.rebuild(bodies);
}

void phy::BVHCollisionDetector2D::updateTree()
{
	m_bvh.expand();
}


void phy::BVHCollisionDetector2D::updateCollisions()
{
	m_collisions_buffer.clear();
	m_bvh.updateAABBCollisions();
	std::vector<BVH::Pair> pairs = m_bvh.getLastAABBCollisions();
	
	for (unsigned int i = 0; i < pairs.size(); i++)
	{
		CollisionData data = pairs[i].first->getCollider()->collideWith(*pairs[i].second->getCollider());
		if (data.has_collision)
		{
			data.bodies[0] = pairs[i].first;
			data.bodies[1] = pairs[i].second;
			m_collisions_buffer.push_back(data);
		}
	}
}



phy::MultiThreadCollisionDetector2D::MultiThreadCollisionDetector2D(unsigned int threads_count)
{
	m_threads.resize(threads_count);
}


void phy::MultiThreadCollisionDetector2D::rebuildTree(std::vector<PhysicsBody2D*>& bodies)
{
	m_bodies = bodies;
}

void phy::MultiThreadCollisionDetector2D::updateTree()
{
}


void phy::MultiThreadCollisionDetector2D::updateCollisions()
{
	if (m_bodies.size() != m_last_bodies_count)
	{
		m_last_bodies_count = m_bodies.size();
		updateThreadsIndexes(m_last_bodies_count);
	}
	
	m_collisions_buffer.clear();
	m_collisions_buffer.reserve(m_bodies.size());
	
	if (m_bodies.size() <= 1) return;
	
	std::vector<std::vector<CollisionData>> thread_results(m_threads.size());
	
	auto process_range = [&](unsigned int thread_id, unsigned int start_pair, unsigned int end_pair)
	{
		for (unsigned int i = start_pair; i < end_pair; i++)
			for (unsigned int j = i + 1; j < m_bodies.size(); j++)
				if (m_bodies[i]->getAABB().checkCollision(m_bodies[j]->getAABB()))
				{
					CollisionData data = m_bodies[i]->getCollider()->collideWith(*m_bodies[j]->getCollider());
					if (data.has_collision)
					{
						data.bodies[0] = m_bodies[i];
						data.bodies[1] = m_bodies[j];
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

void phy::MultiThreadCollisionDetector2D::updateThreadsIndexes(unsigned int bodies_count)
{
	m_threads_last_indexes.resize(m_threads.size() + 1);
	m_threads_last_indexes[0] = 0;

	unsigned int pairs_per_thread = bodies_count/m_threads.size();
	unsigned int remainder = bodies_count%m_threads.size();
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