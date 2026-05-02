#include <Engine/Physics/2D/RectangleCollider2D.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Math/Vec3.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>

#include <vector>
#include <algorithm>


namespace eng
{

phy::RectangleCollider2D::RectangleCollider2D():
	m_size(1),
	phy::Collider2D() {}


void phy::RectangleCollider2D::setSize(const mth::Vec2& size)
{
	m_size = size;
	m_aabb_need_update = true;
}

mth::Vec2 phy::RectangleCollider2D::getSize()
{
	return m_size;
}


phy::Collider2D::AABB phy::RectangleCollider2D::getAABB()
{
	updateAABB();
	return m_cached_aabb;
}


phy::CollisionData phy::RectangleCollider2D::collideWith(Collider2D& other)
{
	std::unique_lock<std::mutex> lock1(m_aabb_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock2(other.getMutex(), std::defer_lock);
    std::lock(lock1, lock2);

	CollisionData data = other.collideWithRectangle(*this);
	if (data.colliders[0] == this) 
		return data;
	return data.swapped();
}

phy::CollisionData phy::RectangleCollider2D::collideWithCircle(CircleCollider2D& other)
{
	CollisionData result;

	float     rect_angle  = getGlobalRotation();
	mth::Vec2 rect_origin = getOrigin();
	mth::Vec2 rect_center = getGlobalPosition() + rect_origin;

	float circle_radius = other.getRadius();
	float circle_angle  = other.getGlobalRotation();
	float cos_c  = std::cos(circle_angle);
	float sin_c  = std::sin(circle_angle);
	mth::Vec2 circle_local_center = other.getOrigin();
	mth::Vec2 circle_rotated_center = mth::Vec2(
		circle_local_center.x*cos_c - circle_local_center.y*sin_c,
		circle_local_center.x*sin_c + circle_local_center.y*cos_c
	);
	mth::Vec2 circle_center = other.getGlobalPosition() + circle_rotated_center;

	mth::Vec2 delta = circle_center - rect_center;
	float cos_a = std::cos(-rect_angle);
	float sin_a = std::sin(-rect_angle);
	mth::Vec2 local_delta;
	local_delta.x = delta.x*cos_a - delta.y*sin_a;
	local_delta.y = delta.x*sin_a + delta.y*cos_a;

	mth::Vec2 half_size = m_size*0.5;
	mth::Vec2 closest_local(
		std::max(-half_size.x, std::min(half_size.x, local_delta.x)),
		std::max(-half_size.y, std::min(half_size.y, local_delta.y))
	);

	cos_a = std::cos(rect_angle);
	sin_a = std::sin(rect_angle);
	mth::Vec2 closest_world;
	closest_world.x = closest_local.x*cos_a - closest_local.y*sin_a;
	closest_world.y = closest_local.x*sin_a + closest_local.y*cos_a;
	closest_world   = closest_world + rect_center;

	mth::Vec2 diff = circle_center - closest_world;
	float distance_squared = diff.x*diff.x + diff.y*diff.y;
	if (distance_squared > circle_radius*circle_radius)
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
		float left_dist   = std::abs(closest_local.x + rect_origin.x);
		float right_dist  = std::abs(closest_local.x - (m_size.x - rect_origin.x));
		float top_dist    = std::abs(closest_local.y + rect_origin.y);
		float bottom_dist = std::abs(closest_local.y - (m_size.y - rect_origin.y));
		float min_dist = std::min({left_dist, right_dist, top_dist, bottom_dist});
		
		mth::Vec2 local_normal;
		if (min_dist == left_dist)
			local_normal = mth::Vec2(-1, 0);
		else if (min_dist == right_dist)
			local_normal = mth::Vec2(1, 0);
		else if (min_dist == top_dist)
			local_normal = mth::Vec2(0, -1);
		else
			local_normal = mth::Vec2(0, 1);
		
		result.normal.x = local_normal.x*cos_a - local_normal.y*sin_a;
		result.normal.y = local_normal.x*sin_a + local_normal.y*cos_a;
		result.penetration_depth = circle_radius - min_dist;
		result.contact_point = closest_world;
		return result;
	}
	
	result.normal = diff/distance;
	result.penetration_depth = circle_radius - distance;
	result.contact_point = closest_world;

	return result;
}

phy::CollisionData phy::RectangleCollider2D::collideWithRectangle(RectangleCollider2D& other)
{
	CollisionData result;
	return result;
}


void phy::RectangleCollider2D::updateAABB()
{
	if (m_aabb_need_update.load())
	{
		std::lock_guard<std::mutex> lock(m_aabb_mutex);
		if (m_aabb_need_update.load())
		{
			mth::Vec2 local_points[4] = {
				mth::Vec2(-m_size.x*0.5, -m_size.y*0.5),
				mth::Vec2( m_size.x*0.5, -m_size.y*0.5),
				mth::Vec2( m_size.x*0.5,  m_size.y*0.5),
				mth::Vec2(-m_size.x*0.5,  m_size.y*0.5)
			};
			
			mth::Vec2 global_points[4];
			for (int i = 0; i < 4; ++i)
			{
				mth::Vec3 point(local_points[i].x, local_points[i].y, 1.0f);
				mth::Vec3 transformed = getGlobalTransform2D()->getMatrix()*point;
				global_points[i] = mth::Vec2(transformed.x, transformed.y);
			}
			
			mth::Vec2 min = global_points[0];
			mth::Vec2 max = global_points[0];
			
			for (int i = 1; i < 4; ++i)
			{
				min.x = std::min(min.x, global_points[i].x);
				min.y = std::min(min.y, global_points[i].y);
				max.x = std::max(max.x, global_points[i].x);
				max.y = std::max(max.y, global_points[i].y);
			}
			
			m_cached_aabb = {min, max};
			m_aabb_need_update = false;
		}
	}
}

} //namespace eng