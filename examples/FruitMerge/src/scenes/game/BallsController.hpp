#ifndef BALLS_CONTROLLER_CLASS_HEADER
#define BALLS_CONTROLLER_CLASS_HEADER

#include <scenes/game/Ball.hpp>
#include <scenes/game/BallsCollection.hpp>

#include <Engine/Core/Node.hpp>

#include <Engine/System/EventManager.hpp>

#include <random>

class BallsController : public eng::core::Node
{
public:
	void onSetup()
	{
		srand(0);

		m_ball_image = addChild<Ball>("ball_image", m_collection, 0);
		//computeNextBallLevel();
	}

	void onUpdate(float delta)
	{
		m_collection.removeCopies();
		for (unsigned int i = 0; i < m_collection.balls.size(); i++)
		{
			mergeBalls(*m_collection.balls[i].first, *m_collection.balls[i].second);
			m_collection.balls.erase(m_collection.balls.begin() + i--);
		}
		m_collection.balls.clear();

		eng::mth::Vec2 camera_point(-1);
		if (m_context.get<eng::sys::EventManager>().getMouse().moved())
		{
			camera_point = computeBallPosition();
			m_ball_image->setPosition(camera_point);
		}

		if (m_context.get<eng::sys::EventManager>().getMouse().isJustPressed(eng::sys::Mouse::LEFT))
		{
			if ((camera_point.x == -1) && (camera_point.y == -1))
				camera_point = computeBallPosition();
			auto ball = addChild<Ball>("ball", m_collection, m_next_ball_level);
			ball->setPosition(camera_point);

			computeNextBallLevel();
		}
	}

	virtual bool isGameOver() = 0;

protected:
	BallsCollection m_collection;
	unsigned int m_next_ball_level;
	Ball* m_ball_image;

	void mergeBalls(Ball& first, Ball& second)
	{
		if (first.getLevel() != second.getLevel()) return;

		auto ball = addChild<Ball>("ball", m_collection, first.getLevel() + 1);
		ball->setPosition((first.getPosition() + second.getPosition())*0.5);
		removeChild(&first);
		removeChild(&second);
	}

	void computeNextBallLevel()
	{
		m_next_ball_level = (rand()%2 + 1);
		m_ball_image->setLevel(m_next_ball_level);
	}

	virtual eng::mth::Vec2 computeBallPosition() = 0;
};

#endif BALLS_CONTROLLER_CLASS_HEADER