#ifndef SANDBOX_BALLS_CONTROLLER_CLASS_HEADER
#define SANDBOX_BALLS_CONTROLLER_CLASS_HEADER

#include <scenes/game/BallsController.hpp>

#include <Engine/System/EventManager.hpp>
#include <Engine/Math/Vec2.hpp>

class SandboxBallsController : public BallsController
{
public:
	bool isGameOver() override
	{
		return false;
	}

protected:
	virtual eng::mth::Vec2 computeBallPosition() override
	{
		return m_context.get<eng::gfx::RenderScene>().getActiveCamera2D().convertWindowPoint(m_context.get<eng::sys::EventManager>().getMouse().getPosition());
	}
};

#endif SANDBOX_BALLS_CONTROLLER_CLASS_HEADER