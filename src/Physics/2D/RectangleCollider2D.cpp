#include <Engine/Physics/2D/RectangleCollider2D.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>

#include <vector>
#include <algorithm>
#include <Engine/Core/Logger.hpp>


namespace eng
{

phy::RectangleCollider2D::RectangleCollider2D():
	m_size(1),
	phy::Collider2D() {}


void phy::RectangleCollider2D::setSize(const mth::Vec2& size)
{
	m_size = size;
}

mth::Vec2 phy::RectangleCollider2D::getSize()
{
	return m_size;
}


phy::CollisionData phy::RectangleCollider2D::collideWith(Collider2D& other)
{
	CollisionData data = other.collideWithRectangle(*this);
	if (data.colliders[0] == this) 
		return data;
	return data.swapped();
}

phy::CollisionData phy::RectangleCollider2D::collideWithCircle(CircleCollider2D& other)
{
	CollisionData result;

	mth::Vec2 rect_top_left = getGlobalPosition();
	mth::Vec2 rect_origin   = getOrigin();
	float     rect_angle    = getGlobalRotation();
	mth::Vec2 rect_center   = rect_top_left + rect_origin;

	float     circle_radius = other.getRadius();
	mth::Vec2 circle_center = other.getGlobalPosition() + mth::Vec2(circle_radius);

	mth::Vec2 delta = circle_center - rect_center;
    float cos_a = std::cos(-rect_angle);
    float sin_a = std::sin(-rect_angle);
    mth::Vec2 local_delta;
    local_delta.x = delta.x*cos_a - delta.y*sin_a;
    local_delta.y = delta.x*sin_a + delta.y*cos_a;

    mth::Vec2 closest_local;
    closest_local.x = std::max(-rect_origin.x, std::min(m_size.x - rect_origin.x, local_delta.x));
    closest_local.y = std::max(-rect_origin.y, std::min(m_size.y - rect_origin.y, local_delta.y));

    cos_a = std::cos(rect_angle);
    sin_a = std::sin(rect_angle);
    mth::Vec2 closest_world;
    closest_world.x = closest_local.x * cos_a - closest_local.y * sin_a;
    closest_world.y = closest_local.x * sin_a + closest_local.y * cos_a;
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
        result.normal = diff.norm();
        result.penetration_depth = circle_radius;
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

} //namespace eng