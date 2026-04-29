#include <Engine/Physics/2D/BVH.hpp>

#include <Engine/Physics/2D/BVHNode.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>


namespace eng
{

phy::BVH::BVH():
	m_root(nullptr) {}

phy::BVH::~BVH()
{
	clear();
}


void phy::BVH::rebuild(std::vector<PhysicsBody2D*>& bodies)
{
	clear();
	m_last_collisions.reserve(bodies.size());
	m_root = buildRecursive(bodies, 0, bodies.size());
}

void phy::BVH::expand()
{
	if (m_root) expandRecursive(m_root);
}

void phy::BVH::clear()
{
	clearNode(m_root);
}

void phy::BVH::updateAABBCollisions()
{
	m_last_collisions.clear();
	if (m_root) checkSelfCollisionsRecursive(m_root);
}

std::vector<phy::BVH::Pair>& phy::BVH::getLastAABBCollisions()
{
	return m_last_collisions;
}



void phy::BVH::clearNode(BVHNode* node)
{
	if (node)
	{
		clearNode(node->getLeft());
		clearNode(node->getRight());
		delete node;
	}
}


phy::BVHNode* phy::BVH::buildRecursive(std::vector<PhysicsBody2D*>& bodies, unsigned int start, unsigned int end)
{
	BVHNode* node = new BVHNode();

	unsigned int count = end - start;

	if (count == 1)
	{
		node->setBody(bodies[start]);
		return node;
	}

	Collider2D::AABB group_aabb = bodies[start]->getAABB();
	for (unsigned int i = start + 1; i < end; i++)
		group_aabb.expand(bodies[i]->getAABB());
	node->setAABB(group_aabb);

	unsigned int middle = start + 1;
	if (count > 2)
	{
		float dx = group_aabb.getMax().x - group_aabb.getMin().x;
		float dy = group_aabb.getMax().y - group_aabb.getMin().y;
		int axis = (dx > dy) ? 0 : 1;
		middle = start + (end - start)/2;

		std::nth_element(bodies.begin() + start, bodies.begin() + middle, bodies.begin() + end, 
			[axis](PhysicsBody2D* first, PhysicsBody2D* second) {
				mth::Vec2 center_first  = first->getAABB().getCenter();
				mth::Vec2 center_second = second->getAABB().getCenter();
				return axis ? (center_first.y < center_second.y) : (center_first.x < center_second.x);
		});
	}

	node->setLeft(buildRecursive(bodies, start, middle));
	node->setRight(buildRecursive(bodies, middle, end));

	return node;
}

void phy::BVH::expandRecursive(phy::BVHNode* node)
{
	if (node->isLeaf())
	{
		node->setAABB(node->getBody()->getAABB());
		return;
	}

	expandRecursive(node->getLeft());
	expandRecursive(node->getRight());

	node->setAABB(node->getLeft()->getAABB());
	node->expand(node->getRight()->getAABB());
}


void phy::BVH::checkSelfCollisionsRecursive(BVHNode* node)
{
	if (!node || node->isLeaf()) return;

	checkSelfCollisionsRecursive(node->getLeft());
	checkSelfCollisionsRecursive(node->getRight());

	checkCollisionsRecursive(node->getLeft(), node->getRight());
}

void phy::BVH::checkCollisionsRecursive(BVHNode* left, BVHNode* right)
{
	if (!left->getAABB().checkCollision(right->getAABB()))
		return;

	if (left->isLeaf() && right->isLeaf())
	{
		m_last_collisions.push_back({left->getBody(), right->getBody()});
		return;
	}

	if (left->isLeaf())
	{
		checkCollisionsRecursive(left, right->getLeft());
		checkCollisionsRecursive(left, right->getRight());
		return;
	}

	if (right->isLeaf())
	{
		checkCollisionsRecursive(right, left->getLeft());
		checkCollisionsRecursive(right, left->getRight());
		return;
	}

	checkCollisionsRecursive(left->getLeft(), right->getLeft());
	checkCollisionsRecursive(left->getLeft(), right->getRight());
	checkCollisionsRecursive(left->getRight(), right->getLeft());
	checkCollisionsRecursive(left->getRight(), right->getRight());
}

} //namespace eng