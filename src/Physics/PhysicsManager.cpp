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

std::vector<phy::PhysicsBody2D*> phy::PhysicsManager::s_bodies2d;
std::vector<phy::CollisionData> phy::PhysicsManager::s_collisions_buffer;
unsigned int phy::PhysicsManager::s_threads_count;
std::vector<std::thread> phy::PhysicsManager::s_threads;
std::vector<unsigned int> phy::PhysicsManager::s_threads_last_indexes;
void (*phy::PhysicsManager::s_update_collisions_handler)();


void phy::PhysicsManager::initialize()
{
	s_bodies2d.clear();
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
	s_threads_count = count;
	
	for (unsigned int i = 0; i < s_threads.size(); i++)
		if (s_threads[i].joinable())
			s_threads[i].join();

	if (count == 1)
	{
		s_threads.clear();
		s_update_collisions_handler = updateCollisionsSingleThread;
	}
	else
	{
		s_threads.resize(count);
		s_update_collisions_handler = updateCollisionsMultiThread;
		updateThreadsIndexes();
	}
}


void phy::PhysicsManager::addBody(PhysicsBody2D& body)
{
	auto it = std::find(s_bodies2d.begin(), s_bodies2d.end(), &body);
	if (it == s_bodies2d.end())
		s_bodies2d.push_back(&body);
	if (s_threads_count > 1)
		updateThreadsIndexes();
}


void phy::PhysicsManager::update(float delta)
{
	//core::Timer t, tl;
	s_update_collisions_handler();
	//core::Logger::info(core::String("Collisions: ") << tl.getElapsedSeconds());

	//tl.restart();
	unsigned int VELOCITY_ITERATIONS = 4;
	for (unsigned int i = 0; i < VELOCITY_ITERATIONS; i++)
		for (unsigned int j = 0; j < s_collisions_buffer.size(); j++)
			s_collisions_buffer[j].bodies[0]->resolveCollisionVelWith(s_collisions_buffer[j], *s_collisions_buffer[j].bodies[1]);
	//core::Logger::info(core::String("Velocity: ") << tl.getElapsedSeconds());

	//tl.restart();
	for (unsigned int i = 0; i < s_bodies2d.size(); i++)
		s_bodies2d[i]->updateState(delta);
	//core::Logger::info(core::String("Update: ") << tl.getElapsedSeconds());

	//tl.restart();
	unsigned int POSITION_ITERATIONS = 4;
	for (unsigned int p = 0; p < POSITION_ITERATIONS; p++)
	{
		s_update_collisions_handler();
		
		if (s_collisions_buffer.empty()) break;
		
		float iter_ratio = 1.0 - (float(p)/POSITION_ITERATIONS);
		for (unsigned int i = 0; i < s_collisions_buffer.size(); i++)
			s_collisions_buffer[i].bodies[0]->resolveCollisionPosWith(s_collisions_buffer[i], iter_ratio, *s_collisions_buffer[i].bodies[1]);
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
	s_collisions_buffer.clear();
	s_collisions_buffer.reserve(s_bodies2d.size());

	for (unsigned int i = 0; i < (s_bodies2d.size() - 1); i++)
		for (unsigned int j = i + 1; j < s_bodies2d.size(); j++)
			if (checkCollisionAABB(*s_bodies2d[i]->getCollider(), *s_bodies2d[j]->getCollider()))
			{
				CollisionData data = s_bodies2d[i]->getCollider()->collideWith(*s_bodies2d[j]->getCollider());
				if (data.has_collision)
				{
					data.bodies[0] = s_bodies2d[i];
					data.bodies[1] = s_bodies2d[j];
					s_collisions_buffer.push_back(data);
				}
			}
}


void phy::PhysicsManager::updateCollisionsMultiThread()
{
	s_collisions_buffer.clear();
	s_collisions_buffer.reserve(s_bodies2d.size());
	
	if (s_bodies2d.size() <= 1) return;
	
	std::vector<std::vector<CollisionData>> thread_results(s_threads.size());
	
	auto process_range = [&](unsigned int thread_id, unsigned int start_pair, unsigned int end_pair)
	{
		for (unsigned int i = start_pair; i < end_pair; i++)
			for (unsigned int j = i + 1; j < s_bodies2d.size(); j++)
				if (checkCollisionAABB(*s_bodies2d[i]->getCollider(), *s_bodies2d[j]->getCollider()))
				{
					CollisionData data = s_bodies2d[i]->getCollider()->collideWith(*s_bodies2d[j]->getCollider());
					if (data.has_collision)
					{
						data.bodies[0] = s_bodies2d[i];
						data.bodies[1] = s_bodies2d[j];
						thread_results[thread_id].push_back(data);
					}
				}
	};
	
	for (unsigned int i = 0; i < s_threads.size(); i++)
		s_threads[i] = std::thread(process_range, i, s_threads_last_indexes[i], s_threads_last_indexes[i + 1]);
	
	for (unsigned int i = 0; i < s_threads.size(); i++)
		if (s_threads[i].joinable())
			s_threads[i].join();
	
	for (unsigned int i = 0; i < thread_results.size(); i++)
		s_collisions_buffer.insert(
			s_collisions_buffer.end(),
			thread_results[i].begin(),
			thread_results[i].end()
		);
}

void phy::PhysicsManager::updateThreadsIndexes()
{
	s_threads_last_indexes.resize(s_threads.size() + 1);
	s_threads_last_indexes[0] = 0;

	unsigned int pairs_per_thread = s_bodies2d.size()/s_threads.size();
	unsigned int remainder = s_bodies2d.size()%s_threads.size();
	unsigned int current_start = 0;
	
	for (unsigned int i = 0; i < s_threads.size(); i++)
	{
		unsigned int pairs_for_this_thread = pairs_per_thread + (i < remainder ? 1 : 0);
		unsigned int current_end = current_start + pairs_for_this_thread;
		s_threads_last_indexes[i + 1] = current_end;
		current_start = current_end;
	}
}

} //namespace eng