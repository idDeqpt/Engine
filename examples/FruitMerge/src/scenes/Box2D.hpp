#ifndef BOX_2D_CLASS_HEADER
#define BOX_2D_CLASS_HEADER

#include <Engine/Core/Node2D.hpp>

#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/2D/StaticBody2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Math/Vec2.hpp>

class Box2D : public eng::core::Node2D
{
public:
	void onSetup()
	{
		eng::phy::PhysicsWorld& PW = m_context.get<eng::phy::PhysicsWorld>();
		eng::gfx::RenderScene& RS = m_context.get<eng::gfx::RenderScene>();

		auto b = addChild<eng::phy::StaticBody2D>("floor");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 0));
		auto sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(300, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		auto col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(300, 10));

		b = addChild<eng::phy::StaticBody2D>("left_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 0));
		b->setOrigin(eng::mth::Vec2(10, 400));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 410));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 410));

		b = addChild<eng::phy::StaticBody2D>("right_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(300, 0));
		b->setOrigin(eng::mth::Vec2(0, 400));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 410));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 410));
	}
};

#endif //BOX_2D_CLASS_HEADER