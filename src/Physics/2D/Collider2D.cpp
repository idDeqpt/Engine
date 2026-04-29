#include <Engine/Physics/2D/Collider2D.hpp>

#include <Engine/Core/Node2D.hpp>


namespace eng
{

void phy::Collider2D::computeTransform()
{
	core::Node2D::computeTransform();
	m_aabb_need_update = true;
}


phy::Collider2D::AABB::AABB(const mth::Vec2& min, const mth::Vec2& max)
{
	set(min, max);
}


void phy::Collider2D::AABB::expand(const AABB& other)
{
	set(
		mth::Vec2(
			(m_min.x < other.m_min.x) ? m_min.x : other.m_min.x,
			(m_min.y < other.m_min.y) ? m_min.y : other.m_min.y
		),
		mth::Vec2(
			(m_max.x > other.m_max.x) ? m_max.x : other.m_max.x,
			(m_max.y > other.m_max.y) ? m_max.y : other.m_max.y
		)
	);
}


void phy::Collider2D::AABB::set(const mth::Vec2& new_min, const mth::Vec2& new_max)
{
	m_min = new_min;
	m_max = new_max;
	m_center = (m_max - m_min)/2;
}


const mth::Vec2& phy::Collider2D::AABB::getMin() const
{
	return m_min;
}

const mth::Vec2& phy::Collider2D::AABB::getMax() const
{
	return m_max;
}

const mth::Vec2& phy::Collider2D::AABB::getCenter() const
{
	return m_center;
}


bool phy::Collider2D::AABB::checkCollision(const AABB& other) const
{
	if (m_max.x < other.m_min.x || other.m_max.x < m_min.x)
		return false;
	
	if (m_max.y < other.m_min.y || other.m_max.y < m_min.y)
		return false;
	
	return true;
}

} //namespace eng