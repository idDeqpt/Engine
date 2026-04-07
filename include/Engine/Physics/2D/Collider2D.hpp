#ifndef COLLIDER_2D_CLASS_HEADER
#define COLLIDER_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>
#include <mutex>

namespace eng::phy
{
	class CircleCollider2D;
	class RectangleCollider2D;

	class Collider2D : public core::Node2D
	{
	public:
		struct AABB
		{
			mth::Vec2 min, max;
		};

		Collider2D() : core::Node2D() {}

		std::mutex& getMutex() {return m_aabb_mutex;}

		virtual AABB getAABB() = 0;

		virtual CollisionData collideWith(Collider2D& other) = 0;
		virtual CollisionData collideWithCircle(CircleCollider2D& other) = 0;
		virtual CollisionData collideWithRectangle(RectangleCollider2D& other) = 0;

	protected:
		bool m_aabb_need_update = true;
		AABB m_cached_aabb;
		std::mutex m_aabb_mutex;

		virtual void updateAABB() = 0;

		void computeTransform()
		{
			core::Node2D::computeTransform();
			m_aabb_need_update = true;
			updateAABB();
		}
	};
}

#endif //COLLIDER_2D_CLASS_HEADER