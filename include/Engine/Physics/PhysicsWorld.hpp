#ifndef PHYSICS_WORLD_CLASS_HEADER
#define PHYSICS_WORLD_CLASS_HEADER

#include <Engine/Physics/2D/CollisionDetector2D.hpp>

#include <vector>
#include <memory>


namespace eng::phy
{
	class PhysicsBody2D;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();

		template <class T, typename... Args>
		void setCollisionDetector(Args&&... args);

		void setFixedDelta(float delta);
		void setMaxStepsPerFrame(unsigned int steps);

		void addBody(PhysicsBody2D& body);
		void removeBody(PhysicsBody2D& body);

		void update(float delta);

	protected:
		float m_accumulator;
		float m_fixed_delta;
		unsigned int m_max_steps_per_frame;
		std::vector<PhysicsBody2D*> m_bodies2d;
		std::unique_ptr<CollisionDetector2D> m_collision_detector;

		void step(float delta);
	};

	template <class T, typename... Args>
	void PhysicsWorld::setCollisionDetector(Args&&... args)
	{
		static_assert(std::is_base_of_v<CollisionDetector2D, T>, "T must be derived from CollisionDetector2D");
		m_collision_detector = std::make_unique<T>(std::forward<Args>(args)...);
	}
}

#endif //PHYSICS_WORLD_CLASS_HEADER