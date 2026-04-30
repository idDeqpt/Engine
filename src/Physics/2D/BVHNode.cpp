#include <Engine/Physics/2D/BVHNode.hpp>

#include <Engine/Physics/2D/PhysicsBody2D.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>

#include <vector>


namespace eng
{

phy::BVHNode::BVHNode():
	m_left(-1),
	m_right(-1),
	m_body(nullptr) {}


bool phy::BVHNode::isLeaf()
{
	return (m_left == -1) && (m_right == -1);
}


void phy::BVHNode::expand(const Collider2D::AABB& aabb)
{
	m_aabb.expand(aabb);
}


void phy::BVHNode::setLeftId(const int& left)
{
	m_left = left;
}

void phy::BVHNode::setRightId(const int& right)
{
	m_right = right;
}

void phy::BVHNode::setBody(PhysicsBody2D* body)
{
	m_body = body;
	m_aabb = body->getAABB();
}

void phy::BVHNode::setAABB(const Collider2D::AABB& aabb)
{
	m_aabb = aabb;
}


const int& phy::BVHNode::getLeftId()
{
	return m_left;
}

const int& phy::BVHNode::getRightId()
{
	return m_right;
}

phy::PhysicsBody2D* phy::BVHNode::getBody()
{
	return m_body;
}

const phy::Collider2D::AABB& phy::BVHNode::getAABB() const
{
	return m_aabb;
}

} //namespace eng