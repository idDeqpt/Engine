#ifndef PHYSICS_BODY_2D_CLASS_HEADER
#define PHYSICS_BODY_2D_CLASS_HEADER

#include <Engine/Physics/2D/Collider2D.hpp>
#include <Engine/Core/Node2D.hpp>

#include <memory>


namespace eng::phy
{
	class PhysicsBody2D : public core::Node2D
	{
	public:
		virtual ~PhysicsBody2D() = default;

		void setEnable(bool flag) {m_enabled = flag;}
		bool isEnable() {return m_enabled;}

		virtual float getMass() {return 0;}
		virtual mth::Vec2 getLinearVelocity() {return 0;}

		template<typename T, typename... Args>
		T* setCollider(Args&&... args);

		Collider2D* getCollider() {return m_collider.get();}

		virtual void update(float delta) {};
		virtual void onCollision(const CollisionData& data) {};

	protected:
		bool m_enabled = true;
		std::unique_ptr<Collider2D> m_collider;
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