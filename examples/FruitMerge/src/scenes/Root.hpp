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

#include <string>

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
				auto cam = static_cast<eng::gfx::Camera2D*>(getChildByName("main_camera"));
				cam->setSize(size);
		});

		m_key_pressed_signal_id = m_context.get<eng::core::SignalBus>().subscribe("keyboard_just_pressed",
			[this](eng::sys::Keyboard::Key key){
				if (key == eng::sys::Keyboard::Key::S)
				{
					auto scaling = m_context.get<eng::core::ConfigManager>().get<std::string>("window_viewport_scaling");
					m_context.get<eng::core::ConfigManager>().set<std::string>("window_viewport_scaling", (scaling == "fixed") ? "stretch" : "fixed");
				}

				if (key == eng::sys::Keyboard::Key::MINUS)
				{
					auto v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
					v_size = v_size/1.1;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_size", v_size);
				}
				if (key == eng::sys::Keyboard::Key::EQUAL)
				{
					auto v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
					v_size = v_size*1.1;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_size", v_size);
				}
		});

		m_key_pressed_signal_id = m_context.get<eng::core::SignalBus>().subscribe("keyboard_repeated",
			[this](eng::sys::Keyboard::Key key){
				if (key == eng::sys::Keyboard::Key::LEFT)
				{
					auto centering = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_centering");
					centering.x -= 0.01;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_centering", centering);
				}
				if (key == eng::sys::Keyboard::Key::RIGHT)
				{
					auto centering = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_centering");
					centering.x += 0.01;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_centering", centering);
				}
				if (key == eng::sys::Keyboard::Key::UP)
				{
					auto centering = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_centering");
					centering.y += 0.01;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_centering", centering);
				}
				if (key == eng::sys::Keyboard::Key::DOWN)
				{
					auto centering = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_centering");
					centering.y -= 0.01;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_centering", centering);
				}

				if (key == eng::sys::Keyboard::Key::MINUS)
				{
					auto v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
					v_size = v_size/1.1;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_size", v_size);
				}
				if (key == eng::sys::Keyboard::Key::EQUAL)
				{
					auto v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
					v_size = v_size*1.1;
					m_context.get<eng::core::ConfigManager>().set<eng::mth::Vec2>("window_viewport_size", v_size);
				}
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