#include <iostream>

#include <Engine/Context.hpp>
#include <Engine/Core/Engine.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/Node.hpp>
#include <Engine/Core/ResourceManager.hpp>

#include <Engine/System/EventManager.hpp>
#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>

#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderManager.hpp>

#include <Engine/Physics/2D/RigidBody2D.hpp>
#include <Engine/Physics/2D/StaticBody2D.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>
#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Math/Vec2.hpp>

#include <random>



class Camera : public eng::gfx::Camera2D
{
public:
	void onUpdate(eng::Context& context, float delta)
	{
		eng::mth::Vec2 vel;
		constexpr float speed = 500;
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::UP))
			vel.y -= speed;
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::DOWN))
			vel.y += speed;
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::LEFT))
			vel.x -= speed;
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::RIGHT))
			vel.x += speed;
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::Q))
			scale(1.1);
		if (context.getEventManager().getKeyboard().isPressed(eng::sys::Keyboard::Key::E))
			scale(0.9);

		if (vel.x || vel.y)
			move(vel.norm(speed)*getScale().x*delta);
	}
};


class Box2D : public eng::core::Node2D
{
public:
	void onSetup(eng::Context& context)
	{
		auto b = addChild<eng::phy::StaticBody2D>(context, "floor");
		context.getPhysicsWorld().addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 0));
		auto sh = b->addChild<eng::gfx::Shape2D>(context, "shape", eng::gfx::Shape2D::Type::RECTANGLE);
		context.getRenderScene().addObject(*sh);
		sh->setSize(eng::mth::Vec2(400, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		auto col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(400, 10));

		b = addChild<eng::phy::StaticBody2D>(context, "left_side");
		context.getPhysicsWorld().addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 0));
		b->setRotation(3.1415*0.75);
		sh = b->addChild<eng::gfx::Shape2D>(context, "shape", eng::gfx::Shape2D::Type::RECTANGLE);
		context.getRenderScene().addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 200));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 200));

		b = addChild<eng::phy::StaticBody2D>(context, "right_side");
		context.getPhysicsWorld().addBody(*b);
		b->setPosition(eng::mth::Vec2(400, 0));
		b->setOrigin(eng::mth::Vec2(10, 0));
		b->setRotation(-3.1415*0.75);
		sh = b->addChild<eng::gfx::Shape2D>(context, "shape", eng::gfx::Shape2D::Type::RECTANGLE);
		context.getRenderScene().addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 200));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 200));

		b = addChild<eng::phy::StaticBody2D>(context, "horizontal");
		context.getPhysicsWorld().addBody(*b);
		b->setPosition(eng::mth::Vec2(150, -100));
		sh = b->addChild<eng::gfx::Shape2D>(context, "shape", eng::gfx::Shape2D::Type::RECTANGLE);
		context.getRenderScene().addObject(*sh);
		sh->setSize(eng::mth::Vec2(100, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(100, 10));
	}
};

class Ball : public eng::phy::RigidBody2D
{
public:
	void onSetup(eng::Context& context)
	{
		auto tex = context.getResourceManager().load<eng::gfx::Texture>({"resources/image1.png"}).second;
		int rad = rand()%5 + 5;
		auto sh = addChild<eng::gfx::Shape2D>(context, "shape", eng::gfx::Shape2D::Type::CIRCLE);
		context.getRenderScene().addObject(*sh);
		sh->setSize(eng::mth::Vec2(rad*2, rad*2));
		sh->setTexture(*tex);

		auto col = setCollider<eng::phy::CircleCollider2D>();
		col->setRadius(rad);
		context.getPhysicsWorld().addBody(*this);
		setMass(rad);
		setRestitution(0.8);
	}

	void onUpdate(eng::Context& context, float delta)
	{
		applyForce(eng::mth::Vec2(0, 10000*delta));
	}
};


class Root : public eng::core::Node
{
public:
	void onSetup(eng::Context& context)
	{
		//eng::phy::PhysicsManager::setThreadsCount(1);
		srand(0);
		eng::gfx::Font* font = context.getResourceManager().load<eng::gfx::Font>({"resources/GameFont.ttf"}).second;

		auto camera2d = addChild<Camera>(context, "Camera2d");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		context.getRenderScene().setActiveCamera(*camera2d);

		addChild<Box2D>(context, "box")->setPosition(eng::mth::Vec2(200, 500));

		for (unsigned int i = 0; i < 20; i++)
			for (unsigned int j = 0; j < 25; j++)
			{
				auto ball = addChild<Ball>(context, "ball");
				ball->setPosition(eng::mth::Vec2(200 + i*20, 100 + j*15));
			}

		auto t_ft = addChild<eng::gfx::Text2D>(context, "text_frametime");
		context.getRenderScene().addObject(*t_ft);
		t_ft->setFont(*font);
		t_ft->setCharacterSize(24);
	}

	void onUpdate(eng::Context& context, float delta)
	{
		auto t_ft = static_cast<eng::gfx::Text2D*>(getChildByName("text_frametime"));
		if (t_ft) t_ft->setString(std::to_string(delta));

		if (context.getEventManager().getMouse().isJustPressed(eng::sys::Mouse::LEFT))
		{
			auto ball = addChild<Ball>(context, "ball");
			ball->setPosition(context.getEventManager().getMouse().getPosition());
			eng::core::Logger::info(eng::core::String("Spawn ") << context.getEventManager().getMouse().getPosition().x << " " << context.getEventManager().getMouse().getPosition().y);
		}
	}
};


int main()
{
	Root r;
	eng::core::Engine engine(r);
	engine.setup();
	engine.mainLoop();

	return 0;
}