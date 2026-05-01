#ifndef BALL_CLASS_HEADER
#define BALL_CLASS_HEADER

#include <Engine/Physics/2D/RigidBody2D.hpp>

#include <Engine/Physics/2D/AreaBody2D.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/Logger.hpp>

#include <Engine/Graphics/2D/Sprite2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Math/Vec2.hpp>

#include <random>
#include <string>

class Ball;

class BallArea : public eng::phy::AreaBody2D
{
public:
	void onCollision(eng::phy::PhysicsBody2D& other);
};

class Ball : public eng::phy::RigidBody2D
{
public:
	Ball(unsigned int start_level): m_level(start_level) {}

	void onSetup()
	{
		auto a = addChild<BallArea>("area");
		a->setCollider<eng::phy::CircleCollider2D>();

		auto s = addChild<eng::gfx::Sprite2D>("sprite");
		m_context.get<eng::gfx::RenderScene>().addObject(*s);

		auto col = setCollider<eng::phy::CircleCollider2D>();
		if (m_level)
		{
			m_context.get<eng::phy::PhysicsWorld>().addBody(*a);
			m_context.get<eng::phy::PhysicsWorld>().addBody(*this);
			setRestitution(0.8);
		}

		setLevel(m_level);
	}

	void onDestroy()
	{
		m_context.get<eng::gfx::RenderScene>().removeObject(*(dynamic_cast<eng::gfx::Sprite2D*>(getChildByName("sprite"))));
		m_context.get<eng::phy::PhysicsWorld>().removeBody(*(dynamic_cast<eng::phy::AreaBody2D*>(getChildByName("area"))));
		m_context.get<eng::phy::PhysicsWorld>().removeBody(*this);
	}

	void onUpdate(float delta)
	{
		applyForce(eng::mth::Vec2(0, 200)*getMass());
	}

	void setLevel(unsigned int level)
	{
		if (!level) return;

		m_level = level;

		unsigned int rad = level*10 + 10;
		auto sprite = static_cast<eng::gfx::Sprite2D*>(getChildByName("sprite"));

		auto* tex = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Texture>({"resources/fruit" + std::to_string(level - 1) + ".png"}).second;
		if (tex)
		{
			tex->setSmooth(false);
			sprite->setTexture(tex);
			sprite->setScale((rad*2)/tex->getSize().x);
			sprite->setOrigin(tex->getSize()/2);
		}

		auto area = static_cast<eng::phy::AreaBody2D*>(getChildByName("area"));
		auto area_collider = static_cast<eng::phy::CircleCollider2D*>(area->getCollider());
		area_collider->setRadius(rad*1.15);
		area_collider->setOrigin(rad*1.15);

		auto collider = static_cast<eng::phy::CircleCollider2D*>(getCollider());
		collider->setRadius(rad*0.95);
		collider->setOrigin(rad*0.95);
		setMass(rad*3);
	}

	unsigned int getLevel()
	{
		return m_level;
	}

protected:
	unsigned int m_level;
};

void BallArea::onCollision(eng::phy::PhysicsBody2D& other)
{
	BallArea* other_area = dynamic_cast<BallArea*>(&other);
	if (other_area)
	{
		Ball* self_ball = static_cast<Ball*>(getParent());
		Ball* other_ball = static_cast<Ball*>(other_area->getParent());
		if (self_ball->getLevel() == other_ball->getLevel())
			m_context.get<eng::core::SignalBus>().emit("balls_collided", self_ball, other_ball);
	}
}

#endif //BALL_CLASS_HEADER