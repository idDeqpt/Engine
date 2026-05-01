#ifndef CLASSIC_BALLS_CONTROLLER_CLASS_HEADER
#define CLASSIC_BALLS_CONTROLLER_CLASS_HEADER

#include <scenes/game/BallsController.hpp>
#include <scenes/game/BallsCollection.hpp>

#include <Engine/System/EventManager.hpp>
#include <Engine/Math/Vec2.hpp>

class ClassicBallsController : public BallsController
{
public:
	ClassicBallsController(eng::mth::Vec2 left_bound, eng::mth::Vec2 right_bound, float bottom_bound):
		m_left_bound(left_bound),
		m_right_bound(right_bound),
		m_bottom_bound(bottom_bound) {}


	bool isGameOver() override
	{
		return m_context.get<eng::sys::EventManager>().getMouse().isJustPressed(eng::sys::Mouse::RIGHT);
	}

protected:
	eng::mth::Vec2 m_left_bound;
	eng::mth::Vec2 m_right_bound;
	float          m_bottom_bound;

	virtual eng::mth::Vec2 computeBallPosition() override
	{
		eng::mth::Vec2 mouse_point = m_context.get<eng::gfx::RenderScene>().getActiveCamera2D().convertWindowPoint(m_context.get<eng::sys::EventManager>().getMouse().getPosition());
		eng::mth::Vec2 lr = m_left_bound - m_right_bound;
		eng::mth::Vec2 lp = m_left_bound - mouse_point;
		float t = lp.dot(lr)/lr.dot(lr);
		if (t < 0)      t = 0;
		else if (t > 1) t = 1;
		eng::mth::Vec2 res = m_left_bound + (-lr)*t;
		return res;
	}
};

#endif CLASSIC_BALLS_CONTROLLER_CLASS_HEADER