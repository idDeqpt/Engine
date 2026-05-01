#ifndef BALL_CLASS_HEADER
#define BALL_CLASS_HEADER

#include <scenes/game/BallsCollection.hpp>

#include <Engine/Physics/2D/RigidBody2D.hpp>

#include <Engine/Physics/2D/CircleCollider2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>
#include <Engine/Core/ResourceManager.hpp>

#include <Engine/Graphics/2D/Sprite2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Math/Vec2.hpp>

#include <random>
#include <string>

class Ball : public eng::phy::RigidBody2D
{
public:
	Ball(BallsCollection& collection, unsigned int start_level):
		m_collection(collection),
		m_level(start_level) {}

	void onSetup()
	{
		auto s = addChild<eng::gfx::Sprite2D>("sprite");
		m_context.get<eng::gfx::RenderScene>().addObject(*s);

		auto col = setCollider<eng::phy::CircleCollider2D>();
		if (m_level)
		{
			m_context.get<eng::phy::PhysicsWorld>().addBody(*this);
			setRestitution(0.8);
		}

		setLevel(m_level);
	}

	void onDestroy()
	{
		m_context.get<eng::gfx::RenderScene>().removeObject(*(dynamic_cast<eng::gfx::Sprite2D*>(getChildByName("sprite"))));
		m_context.get<eng::phy::PhysicsWorld>().removeBody(*this);
	}

	void onUpdate(float delta)
	{
		applyForce(eng::mth::Vec2(0, 100)*getMass());
	}

	void onCollision(eng::phy::PhysicsBody2D& other)
	{
		Ball* other_body = dynamic_cast<Ball*>(&other);
		if (other_body)
			m_collection.balls.push_back(BallsCollection::Pair(this, other_body));
	}

	void setLevel(unsigned int level)
	{
		if (!level) return;

		m_level = level;

		unsigned int rad = level*15;
		auto sprite = static_cast<eng::gfx::Shape2D*>(getChildByName("sprite"));

		auto* tex = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Texture>({"resources/fruit" + std::to_string(level - 1) + ".png"}).second;
		if (tex)
		{
			sprite->setTexture(tex);
			sprite->setScale((rad*2)/tex->getSize().x);
			sprite->setOrigin(tex->getSize()/2);
		}

		auto collider = static_cast<eng::phy::CircleCollider2D*>(getCollider());
		collider->setRadius(rad);
		collider->setOrigin(rad);
		setMass(rad*3);
	}

	unsigned int getLevel()
	{
		return m_level;
	}

protected:
	BallsCollection& m_collection;
	unsigned int m_level;
};

#endif //BALL_CLASS_HEADER