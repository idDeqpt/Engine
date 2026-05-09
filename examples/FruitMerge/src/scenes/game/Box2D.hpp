#ifndef BOX_2D_CLASS_HEADER
#define BOX_2D_CLASS_HEADER

#include <scenes/game/Ball.hpp>

#include <Engine/Core/Node2D.hpp>

#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/2D/StaticBody2D.hpp>
#include <Engine/Physics/2D/AreaBody2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Math/Vec2.hpp>

class Box2D : public eng::phy::AreaBody2D
{
public:
	void onSetup()
	{
		eng::phy::PhysicsWorld& PW = m_context.get<eng::phy::PhysicsWorld>();
		eng::gfx::RenderScene&  RS = m_context.get<eng::gfx::RenderScene>();

		PW.addBody(*this);
		auto col = setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(400, 500));
		auto sh = addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(400, 500));
		sh->setColor(eng::gfx::Color(255, 255, 255, 64));

		auto b = addChild<eng::phy::StaticBody2D>("floor");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 200));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(300, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(300, 10));

		b = addChild<eng::phy::StaticBody2D>("left_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(-150, 0));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 410));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 410));

		b = addChild<eng::phy::StaticBody2D>("right_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(150, 0));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 410));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 410));
	}

	eng::mth::Vec2 getLeftBound()
	{
		return eng::mth::Vec2(-140, -210);
	}

	eng::mth::Vec2 getRightBound()
	{
		return eng::mth::Vec2(140, -210);
	}

	void onCollisionExit(eng::phy::PhysicsBody2D& other)
	{
		Ball* ball = dynamic_cast<Ball*>(&other);
		if (ball)
			m_context.get<eng::core::SignalBus>().emit("ball_exit");
	}
};

#endif //BOX_2D_CLASS_HEADER