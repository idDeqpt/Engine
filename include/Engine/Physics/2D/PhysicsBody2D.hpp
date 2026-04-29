#ifndef PHYSICS_BODY_2D_CLASS_HEADER
#define PHYSICS_BODY_2D_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Core/Node2D.hpp>

#include <memory>


namespace eng::phy
{
	class RigidBody2D;
	class StaticBody2D;
	
	class PhysicsBody2D : public core::Node2D
	{
	public:
		virtual ~PhysicsBody2D() = default;

		void setEnable(bool flag) {m_enabled = flag;}
		bool isEnable() {return m_enabled;}

		void setRestitution(float r) {m_restitution = r;}
		float getRestitution() {return m_restitution;}

		template<typename T, typename... Args>
		T* setCollider(Args&&... args);

		Collider2D* getCollider() {return m_collider.get();}
		Collider2D::AABB getAABB() {return m_collider->getAABB();}

		virtual void integrateVel(float delta) {};
		virtual void integratePos(float delta) {};
		virtual void onCollision(PhysicsBody2D& other) {};

		virtual void resolveCollisionVelWith(const CollisionData& data, PhysicsBody2D& other) {};
		virtual void resolveCollisionVelWithRigid(const CollisionData& data, RigidBody2D& other) {};
		virtual void resolveCollisionVelWithStatic(const CollisionData& data, StaticBody2D& other) {};

		virtual void resolveCollisionPosWith(const CollisionData& data, float iter_ratio, PhysicsBody2D& other) {};
		virtual void resolveCollisionPosWithRigid(const CollisionData& data, float iter_ratio, RigidBody2D& other) {};
		virtual void resolveCollisionPosWithStatic(const CollisionData& data, float iter_ratio, StaticBody2D& other) {};

	protected:
		bool m_enabled = true;
		float m_restitution = 0.9;
		std::unique_ptr<Collider2D> m_collider;

		void computeTransform() override
		{
			core::Node2D::computeTransform();
			m_collider->getLocalTransform();
		}
	};

	template<typename T, typename... Args>
	T* PhysicsBody2D::setCollider(Args&&... args)
	{
		static_assert(std::is_base_of_v<Collider2D, T>);

		m_collider = std::make_unique<T>(std::forward<Args>(args)...);
		m_collider->setParent(this);

		return static_cast<T*>(m_collider.get());
	}
}

#endif //PHYSICS_BODY_2D_CLASS_HEADER