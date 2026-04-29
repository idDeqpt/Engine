#ifndef BVH_NODE_CLASS_HEADER
#define BVH_NODE_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>

namespace eng::phy
{
	class PhysicsBody2D;

	class BVHNode
	{
	public:
		BVHNode();

		bool isLeaf();

		void expand(const Collider2D::AABB& aabb);

		void setLeft(BVHNode* left);
		void setRight(BVHNode* right);
		void setBody(PhysicsBody2D* body);
		void setAABB(const Collider2D::AABB& aabb);

		BVHNode* getLeft();
		BVHNode* getRight();
		PhysicsBody2D* getBody();
		const Collider2D::AABB& getAABB() const;

	protected:
		BVHNode* m_left;
		BVHNode* m_right;
		Collider2D::AABB m_aabb;
		PhysicsBody2D* m_body;
	};
}

#endif //BVH_NODE_CLASS_HEADER