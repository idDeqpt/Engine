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
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/RenderTarget.hpp>

#include <Engine/Physics/2D/RigidBody2D.hpp>
#include <Engine/Physics/2D/StaticBody2D.hpp>
#include <Engine/Physics/2D/CircleCollider2D.hpp>
#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Physics/PhysicsWorld.hpp>

#include <Engine/Math/Vec2.hpp>

#include <memory>
#include <random>



class Camera : public eng::gfx::Camera2D
{
public:
	void onUpdate(float delta)
	{
		eng::sys::EventManager& EM = m_context.get<eng::sys::EventManager>();
		eng::mth::Vec2 vel;
		constexpr float speed = 500;
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::UP))
			vel.y -= speed;
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::DOWN))
			vel.y += speed;
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::LEFT))
			vel.x -= speed;
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::RIGHT))
			vel.x += speed;
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::Q))
			scale(1.1);
		if (EM.getKeyboard().isPressed(eng::sys::Keyboard::Key::E))
			scale(0.9);

		if (vel.x || vel.y)
			move(vel.norm(speed)*getScale().x*delta);
	}
};


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
		sh->setSize(eng::mth::Vec2(400, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		auto col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(400, 10));

		b = addChild<eng::phy::StaticBody2D>("left_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(0, 0));
		b->setRotation(3.1415*0.75);
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 200));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 200));

		b = addChild<eng::phy::StaticBody2D>("right_side");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(400, 0));
		b->setOrigin(eng::mth::Vec2(10, 0));
		b->setRotation(-3.1415*0.75);
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(10, 200));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(10, 200));

		b = addChild<eng::phy::StaticBody2D>("horizontal");
		PW.addBody(*b);
		b->setPosition(eng::mth::Vec2(150, -100));
		sh = b->addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::RECTANGLE);
		RS.addObject(*sh);
		sh->setSize(eng::mth::Vec2(100, 10));
		sh->setColor(eng::gfx::Color(0, 0, 255));
		col = b->setCollider<eng::phy::RectangleCollider2D>();
		col->setSize(eng::mth::Vec2(100, 10));
	}
};

class Ball : public eng::phy::RigidBody2D
{
public:
	void onSetup()
	{
		auto tex = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Texture>({"resources/image1.png"}).second;
		int rad = rand()%5 + 5;
		auto sh = addChild<eng::gfx::Shape2D>("shape", eng::gfx::Shape2D::Type::CIRCLE);
		m_context.get<eng::gfx::RenderScene>().addObject(*sh);
		sh->setSize(eng::mth::Vec2(rad*2, rad*2));
		sh->setTexture(*tex);

		auto col = setCollider<eng::phy::CircleCollider2D>();
		col->setRadius(rad);
		m_context.get<eng::phy::PhysicsWorld>().addBody(*this);
		setMass(rad);
		setRestitution(0.8);
	}

	void onUpdate(float delta)
	{
		applyForce(eng::mth::Vec2(0, 10000*delta));
	}
};


class Scene : public eng::gfx::Shape2D
{
public:
	Scene() : eng::gfx::Shape2D()
	{
		eng::gfx::Texture::PixelFormat pf = eng::gfx::Texture::PixelFormat::RGBA;
		m_target = std::make_unique<eng::gfx::RenderTarget>(1, &pf);
		m_target->setViewport(0, 0, 900, 600);

		m_scene = std::make_shared<eng::gfx::RenderScene>();
		m_scene->setClearColor(eng::gfx::Color(0, 0, 0, 0));
	}

	void onSetup()
	{
		setType(eng::gfx::Shape2D::Type::RECTANGLE);
		setSize(eng::mth::Vec2(900, 600));
		setTexture(m_target->getTexture(0));
		getTexture()->setFlip(false, true);
		eng::core::Logger::info(getTexture()->getFlipX());
		eng::core::Logger::info(getTexture()->getFlipY());

		m_scene->setRenderPipeline2D(m_context.get<eng::gfx::RenderScene>().getRenderPipeline2D());
		m_context.replace<eng::gfx::RenderScene>(m_scene);
	}

	void onUpdate(float delta)
	{
		m_scene->render(*m_target);
	}

protected:
	std::shared_ptr<eng::gfx::RenderScene> m_scene;
	std::unique_ptr<eng::gfx::RenderTarget> m_target;
};


class GameScene : public Scene
{
public:
	void onSetup()
	{
		Scene::onSetup();

		auto camera2d = addChild<Camera>("Camera2d");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		addChild<Box2D>("box")->setPosition(eng::mth::Vec2(200, 500));

		for (unsigned int i = 0; i < 20; i++)
			for (unsigned int j = 0; j < 25; j++)
			{
				auto ball = addChild<Ball>("ball");
				ball->setPosition(eng::mth::Vec2(200 + i*20, 100 + j*15));
			}
	}

	void onUpdate(float delta)
	{
		if (m_context.get<eng::sys::EventManager>().getMouse().isJustPressed(eng::sys::Mouse::LEFT))
		{
			auto ball = addChild<Ball>("ball");
			ball->setPosition(m_context.get<eng::sys::EventManager>().getMouse().getPosition());
			eng::core::Logger::info(eng::core::String("Spawn ") << m_context.get<eng::sys::EventManager>().getMouse().getPosition().x << " " << m_context.get<eng::sys::EventManager>().getMouse().getPosition().y);
		}

		Scene::onUpdate(delta);
	}
};


class UIScene : public Scene
{
public:
	UIScene(eng::gfx::Font& font):
		m_font(&font),
		Scene() {}

	void onSetup()
	{
		Scene::onSetup();

		auto camera2d = addChild<eng::gfx::Camera2D>("UICamera");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		auto t_ft = addChild<eng::gfx::Text2D>("text_frametime");
		m_context.get<eng::gfx::RenderScene>().addObject(*t_ft);
		t_ft->setFont(*m_font);
		t_ft->setCharacterSize(24);
	}

	void onUpdate(float delta)
	{
		auto t_ft = static_cast<eng::gfx::Text2D*>(getChildByName("text_frametime"));
		if (t_ft) t_ft->setString(std::to_string(delta));

		Scene::onUpdate(delta);
	}

protected:
	eng::gfx::Font* m_font;
};


class Root : public eng::core::Node
{
public:
	void onSetup()
	{
		srand(0);
		eng::gfx::Font* font = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Font>({"resources/GameFont.ttf"}).second;

		auto camera2d = addChild<eng::gfx::Camera2D>("MainCamera");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		UIScene* ui = addChild<UIScene>("ui_scene", *font);
		m_context.get<eng::gfx::RenderScene>().addObject(*ui);

		GameScene* game = addChild<GameScene>("game_scene");
		m_context.get<eng::gfx::RenderScene>().addObject(*game);
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