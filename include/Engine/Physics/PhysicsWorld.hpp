#ifndef PHYSICS_WORLD_CLASS_HEADER
#define PHYSICS_WORLD_CLASS_HEADER

#include <Engine/Physics/2D/CollisionDetector2D.hpp>

#include <utility>
#include <vector>
#include <memory>


namespace eng::phy
{
	class PhysicsBody2D;

	class PhysicsWorld
	{
	public:
		using Pair2D = std::pair<PhysicsBody2D*, PhysicsBody2D*>;
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
		std::vector<Pair2D> m_previous_frame_bodies2d;

		bool m_in_step;
		std::vector<PhysicsBody2D*> m_pending_add;
		std::vector<PhysicsBody2D*> m_pending_remove;

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