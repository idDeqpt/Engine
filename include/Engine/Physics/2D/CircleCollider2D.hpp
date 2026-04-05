#ifndef CIRCLE_COLLIDER_2D_CLASS_HEADER
#define CIRCLE_COLLIDER_2D_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng::phy
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D();

		void setRadius(float radius);
		float getRadius();

		AABB getAABB() override;

		CollisionData collideWith(Collider2D& other) override;
		CollisionData collideWithCircle(CircleCollider2D& other) override;
		CollisionData collideWithRectangle(RectangleCollider2D& other) override;

	protected:
		float m_radius;

		void updateAABB() override;
	};
}

#endif //CIRCLE_COLLIDER_2D_CLASS_HEADER