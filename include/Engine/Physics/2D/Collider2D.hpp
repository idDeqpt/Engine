#ifndef COLLIDER_2D_CLASS_HEADER
#define COLLIDER_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>

namespace eng::phy
{
	class CircleCollider2D;

	class Collider2D : public core::Node2D
	{
	public:
		enum class Type
		{
			CIRCLE,
			RECTANGLE,
			CUSTOM,
		};

		Collider2D() : core::Node2D() {}

		virtual Type getType() {return Type::CUSTOM;}

		virtual CollisionData collideWith(Collider2D& other) = 0;
		virtual CollisionData collideWithCircle(CircleCollider2D& other) = 0;
	};
}

#endif //COLLIDER_2D_CLASS_HEADER