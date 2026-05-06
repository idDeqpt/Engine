#ifndef ROOT_CLASS_HEADER
#define ROOT_CLASS_HEADER

#include <scenes/game/UIScene.hpp>
#include <scenes/game/GameScene.hpp>

#include <Engine/Core/Node.hpp>

#include <Engine/Core/ResourceManager.hpp>

#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Math/Vec2.hpp>

class Root : public eng::core::Node
{
public:
	void onSetup()
	{
		eng::gfx::Font* font = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Font>({"resources/GameFont.ttf"}).second;

		eng::mth::Vec2 v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
		auto camera2d = addChild<eng::gfx::Camera2D>("main_camera");
		camera2d->setSize(v_size);
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		UIScene* ui = addChild<UIScene>("ui_scene", *font);
		ui->setLayer(100);
		m_context.get<eng::gfx::RenderScene>().addObject(*ui);

		GameScene* game = addChild<GameScene>("game_scene");
		game->setLayer(10);
		m_context.get<eng::gfx::RenderScene>().addObject(*game);

		m_viewport_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				auto cam = static_cast<eng::gfx::Camera2D*>(getChildByName("camera"));
				cam->setSize(size);
		});

		m_key_pressed_signal_id = m_context.get<eng::core::SignalBus>().subscribe("keyboard_repeated",
			[this](eng::sys::Keyboard::Key key){
				eng::mth::Vec2 centering = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_centering");
				if (key == eng::sys::Keyboard::Key::LEFT)
					centering.x -= 0.01;
				if (key == eng::sys::Keyboard::Key::RIGHT)
					centering.x += 0.01;
				if (key == eng::sys::Keyboard::Key::UP)
					centering.y += 0.01;
				if (key == eng::sys::Keyboard::Key::DOWN)
					centering.y -= 0.01;
				m_context.get<eng::core::ConfigManager>().set("window_viewport_centering", centering);
		});
	}

	void onDestroy()
	{
		m_context.get<eng::core::SignalBus>().unsubscribe(m_viewport_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_key_pressed_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_key_just_pressed_signal_id);
	}

protected:
	eng::core::SubscriptionId m_viewport_signal_id;
	eng::core::SubscriptionId m_key_pressed_signal_id;
	eng::core::SubscriptionId m_key_just_pressed_signal_id;
};

#endif //ROOT_CLASS_HEADER