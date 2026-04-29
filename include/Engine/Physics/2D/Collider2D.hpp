#ifndef COLLIDER_2D_CLASS_HEADER
#define COLLIDER_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>
#include <Engine/Physics/2D/CollisionData.hpp>
#include <Engine/Math/Vec2.hpp>

#include <atomic>
#include <mutex>

namespace eng::phy
{
	class CircleCollider2D;
	class RectangleCollider2D;

	class Collider2D : public core::Node2D
	{
	public:
		class AABB
		{
		public:
			AABB() = default;
			AABB(const mth::Vec2& min, const mth::Vec2& max);
			
			void expand(const AABB& other);

			void set(const mth::Vec2& new_min, const mth::Vec2& new_max);

			const mth::Vec2& getMin() const;
			const mth::Vec2& getMax() const;
			const mth::Vec2& getCenter() const;

			bool checkCollision(const AABB& other) const;

		protected:
			mth::Vec2 m_min, m_max;
			mth::Vec2 m_center;
		};

		Collider2D() : core::Node2D() {}

		std::mutex& getMutex() {return m_aabb_mutex;}

		virtual AABB getAABB() = 0;

		virtual CollisionData collideWith(Collider2D& other) = 0;
		virtual CollisionData collideWithCircle(CircleCollider2D& other) = 0;
		virtual CollisionData collideWithRectangle(RectangleCollider2D& other) = 0;

	protected:
		std::atomic<bool> m_aabb_need_update = true;
		AABB m_cached_aabb;
		std::mutex m_aabb_mutex;

		virtual void updateAABB() = 0;

		void computeTransform() override;
	};
}

#endif //COLLIDER_2D_CLASS_HEADER