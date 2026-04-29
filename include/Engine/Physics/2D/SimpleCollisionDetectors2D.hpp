#ifndef SIMPLE_COLLISION_DETECTOR_2D_CLASSES_HEADER
#define SIMPLE_COLLISION_DETECTOR_2D_CLASSES_HEADER

#include <Engine/Physics/2D/CollisionDetector2D.hpp>
#include <Engine/Physics/2D/BVH.hpp>

#include <thread>
#include <vector>


namespace eng::phy
{
	class PhysicsBody2D;

	class BVHCollisionDetector2D : public CollisionDetector2D
	{
	public:
		void rebuildTree(const std::vector<PhysicsBody2D*>& bodies) override;
		void updateTree() override;
		
		void updateCollisions() override;

	protected:
		BVH m_bvh;
	};


	class MultiThreadCollisionDetector2D : public CollisionDetector2D
	{
	public:
		MultiThreadCollisionDetector2D(unsigned int threads_count);

		void rebuildTree(const std::vector<PhysicsBody2D*>& bodies) override;
		void updateTree() override;
		
		void updateCollisions() override;

	protected:
		std::vector<PhysicsBody2D*> m_bodies;
		unsigned int m_last_bodies_count;
		std::vector<std::thread>  m_threads;
		std::vector<unsigned int> m_threads_last_indexes;

		void updateThreadsIndexes(unsigned int bodies_count);
	};
}

#endif //SIMPLE_COLLISION_DETECTOR_2D_CLASSES_HEADER