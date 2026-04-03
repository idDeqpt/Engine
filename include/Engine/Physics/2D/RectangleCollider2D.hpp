#ifndef RECTANGLE_COLLIDER_2D_CLASS_HEADER
#define RECTANGLE_COLLIDER_2D_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>

#include <vector>


namespace eng::phy
{

	class RectangleCollider2D : public Collider2D
	{
	public:
		RectangleCollider2D();

		void setSize(const mth::Vec2& size);
		mth::Vec2 getSize();

		CollisionData collideWith(Collider2D& other) override;
		CollisionData collideWithCircle(CircleCollider2D& other) override;
		CollisionData collideWithRectangle(RectangleCollider2D& other) override;

	protected:
		mth::Vec2 m_size;
	};
}

#endif //RECTANGLE_COLLIDER_2D_CLASS_HEADER