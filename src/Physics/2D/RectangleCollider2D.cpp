#include <Engine/Physics/2D/RectangleCollider2D.hpp>

#include <Engine/Math/Vec2.hpp>

#include <vector>


namespace eng
{

phy::RectangleCollider2D::RectangleCollider2D():
	m_size(1),
	phy::Collider2D() {}


void phy::RectangleCollider2D::setSize(const mth::Vec2& size)
{
	m_size = size;
}


phy::Collider2D::Type phy::RectangleCollider2D::getType()
{
	return Collider2D::Type::RECTANGLE;
}

mth::Vec2 phy::RectangleCollider2D::getSize()
{
	return m_size;
}

} //namespace eng