#ifndef BALLS_CONTROLLER_CLASS_HEADER
#define BALLS_CONTROLLER_CLASS_HEADER

#include <scenes/game/Ball.hpp>
#include <scenes/game/BallsCollection.hpp>

#include <Engine/Core/Node.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/Logger.hpp>

#include <Engine/System/EventManager.hpp>

#include <random>

class BallsController : public eng::core::Node
{
public:
	void onSetup()
	{
		srand(0);
		m_ball_signal_id = m_context.get<eng::core::SignalBus>().subscribe("balls_collided",
			[this](Ball* left, Ball* right) {
				m_collection.balls.push_back(BallsCollection::Pair(left, right));
		});

		m_ball_image = addChild<Ball>("ball_image", 0);
		//computeNextBallLevel();

		m_mouse_signal_id = m_context.get<eng::core::SignalBus>().subscribe("mouse_just_clicked",
			[this](eng::sys::Mouse::Button button) {
				if (button == eng::sys::Mouse::LEFT)
				{
					auto ball = addChild<Ball>("ball", m_next_ball_level);
					ball->setPosition(computeBallPosition(m_context.get<eng::sys::EventManager>().getMouse().getPosition()));
					computeNextBallLevel();
				}
		});

		m_mouse_signal_id = m_context.get<eng::core::SignalBus>().subscribe("mouse_moved",
			[this](const eng::mth::Vec2& pos, const eng::mth::Vec2& delta) {
				m_ball_image->setPosition(computeBallPosition(pos));
		});
	}

	void onDestroy()
	{
		m_context.get<eng::core::SignalBus>().unsubscribe(m_ball_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_mouse_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_mouse_moved_signal_id);
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
	}

	virtual bool isGameOver() = 0;

protected:
	BallsCollection m_collection;
	unsigned int m_next_ball_level;
	Ball* m_ball_image;
	eng::core::SubscriptionId m_ball_signal_id;
	eng::core::SubscriptionId m_mouse_signal_id;
	eng::core::SubscriptionId m_mouse_moved_signal_id;

	void mergeBalls(Ball& first, Ball& second)
	{
		if (first.getLevel() != second.getLevel()) return;

		auto ball = addChild<Ball>("ball", first.getLevel() + 1);
		ball->setPosition((first.getPosition() + second.getPosition())*0.5);
		removeChild(&first);
		removeChild(&second);
	}

	void computeNextBallLevel()
	{
		m_next_ball_level = (rand()%2 + 1);
		m_ball_image->setLevel(m_next_ball_level);
	}

	virtual eng::mth::Vec2 computeBallPosition(const eng::mth::Vec2& mouse_pos) = 0;
};

#endif BALLS_CONTROLLER_CLASS_HEADER