#ifndef BVH_CLASS_HEADER
#define BVH_CLASS_HEADER

#include <vector>

namespace eng::phy
{
	class PhysicsBody2D;
	class BVHNode;

	class BVH
	{
	public:
		struct Pair
		{
			PhysicsBody2D* first;
			PhysicsBody2D* second;
		};

		BVH();
		~BVH();

		void rebuild(std::vector<PhysicsBody2D*>& bodies);
		void expand();
		void clear();

		void updateAABBCollisions();
		std::vector<Pair>& getLastAABBCollisions();

	protected:
		BVHNode* m_root;
		std::vector<Pair> m_last_collisions;

		void clearNode(BVHNode* node);

		BVHNode* buildRecursive(std::vector<PhysicsBody2D*>& bodies, unsigned int start, unsigned int end);
		void expandRecursive(BVHNode* node);
		
		void checkSelfCollisionsRecursive(BVHNode* node);
		void checkCollisionsRecursive(BVHNode* left, BVHNode* right);
	};
}

#endif //BVH_CLASS_HEADER