#include <Engine/Physics/2D/BVH.hpp>

#include <Engine/Physics/2D/BVHNode.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>


namespace eng
{

phy::BVH::~BVH()
{
	clear();
}


void phy::BVH::rebuild(std::vector<PhysicsBody2D*>& bodies)
{
	clear();
	m_nodes_pool.reserve(bodies.size()*3);
	m_last_collisions.reserve(bodies.size()*5);

	buildRecursive(bodies, 0, bodies.size());
}

void phy::BVH::expand()
{
	if (m_nodes_pool.size()) expandRecursive(0);
}

void phy::BVH::clear()
{
	m_nodes_pool.clear();
	m_last_collisions.clear();
}

void phy::BVH::updateAABBCollisions()
{
	m_last_collisions.clear();
	if (m_nodes_pool.size()) checkSelfCollisionsRecursive(0);
}

std::vector<phy::BVH::Pair>& phy::BVH::getLastAABBCollisions()
{
	return m_last_collisions;
}



int phy::BVH::buildRecursive(std::vector<PhysicsBody2D*>& bodies, unsigned int start, unsigned int end)
{
	m_nodes_pool.emplace_back();
	BVHNode& node = m_nodes_pool.back();
	unsigned int self_id = m_nodes_pool.size() - 1;

	unsigned int count = end - start;

	if (count == 1)
	{
		node.setBody(bodies[start]);
		return self_id;
	}

	Collider2D::AABB group_aabb = bodies[start]->getAABB();
	for (unsigned int i = start + 1; i < end; i++)
		group_aabb.expand(bodies[i]->getAABB());
	node.setAABB(group_aabb);

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

	node.setLeftId(buildRecursive(bodies, start, middle));
	node.setRightId(buildRecursive(bodies, middle, end));

	return self_id;
}

void phy::BVH::expandRecursive(int node_id)
{
	if (node_id == -1) return;
	BVHNode& node = m_nodes_pool[node_id];

	if (node.isLeaf())
	{
		node.setAABB(node.getBody()->getAABB());
		return;
	}

	expandRecursive(node.getLeftId());
	expandRecursive(node.getRightId());

	Collider2D::AABB aabb = m_nodes_pool[node.getLeftId()].getAABB();
	aabb.expand(m_nodes_pool[node.getRightId()].getAABB());
	node.setAABB(aabb);
}


void phy::BVH::checkSelfCollisionsRecursive(int node_id)
{
	if (node_id == -1) return;
	BVHNode& node = m_nodes_pool[node_id];

	if (node.isLeaf()) return;

	checkSelfCollisionsRecursive(node.getLeftId());
	checkSelfCollisionsRecursive(node.getRightId());

	checkCollisionsRecursive(node.getLeftId(), node.getRightId());
}

void phy::BVH::checkCollisionsRecursive(int left_id, int right_id)
{
	if ((left_id == -1) || (right_id == -1)) return;
	BVHNode& left  = m_nodes_pool[left_id];
	BVHNode& right = m_nodes_pool[right_id];

	if (!left.getAABB().checkCollision(right.getAABB()))
		return;

	if (left.isLeaf() && right.isLeaf())
	{
	    auto* a = left.getBody();
	    auto* b = right.getBody();
	    if (a > b) std::swap(a, b);
	    m_last_collisions.push_back({a, b});
		return;
	}

	if (left.isLeaf())
	{
		checkCollisionsRecursive(left_id, right.getLeftId());
		checkCollisionsRecursive(left_id, right.getRightId());
		return;
	}

	if (right.isLeaf())
	{
		checkCollisionsRecursive(right_id, left.getLeftId());
		checkCollisionsRecursive(right_id, left.getRightId());
		return;
	}

	checkCollisionsRecursive(left.getLeftId(),  right.getLeftId());
	checkCollisionsRecursive(left.getLeftId(),  right.getRightId());
	checkCollisionsRecursive(left.getRightId(), right.getLeftId());
	checkCollisionsRecursive(left.getRightId(), right.getRightId());
}

} //namespace eng