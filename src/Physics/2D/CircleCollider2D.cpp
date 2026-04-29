#include <Engine/Physics/2D/CircleCollider2D.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Core/Logger.hpp>

#include <cmath>


namespace eng
{

phy::CircleCollider2D::CircleCollider2D():
	m_radius(1),
	phy::Collider2D() {}


void phy::CircleCollider2D::setRadius(float radius)
{
	m_radius = radius;
	m_aabb_need_update = true;
}

float phy::CircleCollider2D::getRadius()
{
	return m_radius;
}


phy::Collider2D::AABB phy::CircleCollider2D::getAABB()
{
	updateAABB();
	return m_cached_aabb;
}


phy::CollisionData phy::CircleCollider2D::collideWith(Collider2D& other)
{
	std::unique_lock<std::mutex> lock1(m_aabb_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(other.getMutex(), std::defer_lock);
    std::lock(lock1, lock2);
    
	CollisionData data = other.collideWithCircle(*this);
	if (data.colliders[0] == this) 
		return data;
	return data.swapped();
}

phy::CollisionData phy::CircleCollider2D::collideWithCircle(CircleCollider2D& other)
{
	CollisionData result;

	float radius1 = getRadius();
	float radius2 = other.getRadius();

	mth::Vec2 local_center1 = radius1 - getOrigin();
	mth::Vec2 local_center2 = radius2 - other.getOrigin();

	float angle1 = getGlobalRotation();
	float angle2 = other.getGlobalRotation();
	float cos1 = std::cos(angle1);
	float sin1 = std::sin(angle1);
	float cos2 = std::cos(angle2);
	float sin2 = std::sin(angle2);
	
	mth::Vec2 rotated_center1 = mth::Vec2(local_center1.x*cos1 - local_center1.y*sin1, local_center1.x*sin1 + local_center1.y*cos1);
	mth::Vec2 rotated_center2 = mth::Vec2(local_center2.x*cos2 - local_center2.y*sin2, local_center2.x*sin2 + local_center2.y*cos2);
	
	mth::Vec2 center1 = getGlobalPosition()       + rotated_center1;
	mth::Vec2 center2 = other.getGlobalPosition() + rotated_center2;

	mth::Vec2 delta = center2 - center1;
	float distance_squared = delta.x*delta.x + delta.y*delta.y;
	float radius_sum = radius1 + radius2;
	
	if (distance_squared > (radius_sum*radius_sum))
	{
		result.has_collision = false;
		return result;
	}
	
	result.has_collision = true;
	result.colliders[0] = this;
	result.colliders[1] = &other;
	
	float distance = std::sqrt(distance_squared);
	
	if (distance < 0.0001f)
	{
		result.normal = mth::Vec2(1.0f, 0.0f);
		result.penetration_depth = radius_sum;
		result.contact_point = center1;
		return result;
	}
	
	result.normal = delta / distance;
	result.penetration_depth = radius_sum - distance;
	
	result.contact_point = center1 + result.normal*((radius1 + radius2)*0.5);
	
	return result;
}

phy::CollisionData phy::CircleCollider2D::collideWithRectangle(RectangleCollider2D& other)
{
	return other.collideWithCircle(*this);
}


void phy::CircleCollider2D::updateAABB()
{
	if (m_aabb_need_update.load())
	{
		std::lock_guard<std::mutex> lock(m_aabb_mutex);
		if (m_aabb_need_update.load())
		{
			mth::Vec2 local_center = m_radius - getOrigin();
			float cos = std::cos(getGlobalRotation());
			float sin = std::sin(getGlobalRotation());
			mth::Vec2 rotated_center = mth::Vec2(local_center.x*cos - local_center.y*sin, local_center.x*sin + local_center.y*cos);
			mth::Vec2 global_center = getGlobalPosition() + rotated_center;
			m_cached_aabb = AABB(global_center - m_radius, global_center + m_radius);
			m_aabb_need_update = false;
		}
	}
}

} //namespace eng