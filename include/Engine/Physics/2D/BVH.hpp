#ifndef BVH_CLASS_HEADER
#define BVH_CLASS_HEADER

#include <Engine/Physics/2D/BVHNode.hpp>
#include <vector>

namespace eng::phy
{
	class PhysicsBody2D;

	class BVH
	{
	public:
		struct Pair
		{
			PhysicsBody2D* first;
			PhysicsBody2D* second;
		};

		BVH() = default;
		~BVH();

		void rebuild(std::vector<PhysicsBody2D*>& bodies);
		void expand();
		void clear();

		void updateAABBCollisions();
		std::vector<Pair>& getLastAABBCollisions();

	protected:
		std::vector<BVHNode> m_nodes_pool;
		std::vector<Pair>    m_last_collisions;

		int buildRecursive(std::vector<PhysicsBody2D*>& bodies, unsigned int start, unsigned int end);
		void expandRecursive(int node_id);
		
		void checkSelfCollisionsRecursive(int node_id);
		void checkCollisionsRecursive(int left_id, int right_id);
	};
}

#endif //BVH_CLASS_HEADER