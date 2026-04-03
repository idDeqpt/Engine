#include <Engine/Physics/2D/CircleCollider2D.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>

#include <cmath>


namespace eng
{

phy::CircleCollider2D::CircleCollider2D():
	m_radius(1),
	phy::Collider2D() {}


void phy::CircleCollider2D::setRadius(float radius)
{
	m_radius = radius;
}

float phy::CircleCollider2D::getRadius()
{
	return m_radius;
}


phy::CollisionData phy::CircleCollider2D::collideWith(Collider2D& other)
{
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
	
	mth::Vec2 center1 = getGlobalPosition() + radius1;
	mth::Vec2 center2 = other.getGlobalPosition() + radius2;
	
	mth::Vec2 delta = center2 - center1;
	float distance_squared = delta.x * delta.x + delta.y * delta.y;
	float radius_sum = radius1 + radius2;
	
	if (distance_squared > (radius_sum * radius_sum))
	{
		result.has_collision = false;
		return result;
	}
	
	result.has_collision = true;
	result.colliders[0] = this;
	result.colliders[1] = &other;
	
	float distance = std::sqrt(distance_squared);
	
	if (distance < 0.0001f) {
		result.normal = mth::Vec2(1.0f, 0.0f);
		result.penetration_depth = radius_sum;
		result.contact_point = center1;
		return result;
	}
	
	result.normal = delta / distance;
	result.penetration_depth = radius_sum - distance;
	
	result.contact_point = center1 + result.normal * radius1;
	
	return result;
}

phy::CollisionData phy::CircleCollider2D::collideWithRectangle(RectangleCollider2D& other)
{
	return other.collideWithCircle(*this);
}

} //namespace eng