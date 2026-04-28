#ifndef BALLS_CONTROLLER_CLASS_HEADER
#define BALLS_CONTROLLER_CLASS_HEADER

#include <scenes/Ball.hpp>
#include <scenes/BallsCollection.hpp>

#include <Engine/Core/Node.hpp>
#include <Engine/Core/Logger.hpp>

#include <Engine/System/EventManager.hpp>

#include <random>

class BallsController : public eng::core::Node
{
public:
	void onSetup()
	{
		srand(0);
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

		if (m_context.get<eng::sys::EventManager>().getMouse().isJustPressed(eng::sys::Mouse::LEFT))
		{
			auto ball = addChild<Ball>("ball", m_collection);
			ball->setPosition(m_context.get<eng::sys::EventManager>().getMouse().getPosition());
		}
	}

	void mergeBalls(Ball& first, Ball& second)
	{
		if (first.getLevel() != second.getLevel()) return;

		auto ball = addChild<Ball>("ball", m_collection);
		ball->setLevel(first.getLevel() + 1);
		ball->setPosition((first.getPosition() + second.getPosition())*0.5);
		removeChild(&first);
		removeChild(&second);
	}

protected:
	BallsCollection m_collection;
};

#endif //BALLS_CONTROLLER_CLASS_HEADER