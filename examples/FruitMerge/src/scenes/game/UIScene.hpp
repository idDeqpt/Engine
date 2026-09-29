#ifndef UI_SCENE_CLASS_HEADER
#define UI_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>

#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/2D/RenderCanvas.hpp>
#include <Engine/Math/Vec2.hpp>

class UIScene : public SceneLayer
{
public:
	UIScene(eng::gfx::Font& font):
		m_font(&font) {}

	void onSetup()
	{
		SceneLayer::onSetup();

		auto camera2d = addChild<eng::gfx::Camera2D>("camera");
		eng::mth::Vec2 v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
		camera2d->setRect(0, v_size.x, 0, v_size.y);
		m_context.get<eng::gfx::RenderCanvas>().setActiveCamera(*camera2d);

		auto text = addChild<eng::gfx::Text2D>("text_game_over");
		m_context.get<eng::gfx::RenderCanvas>().addObject(*text);
		text->setFont(*m_font);
		text->setCharacterSize(24);
		text->setString("Game over");
		text->setPosition(eng::mth::Vec2(v_size.x/2, 200));
		text->setOrigin(eng::mth::Vec2(100, 0));
		text->setVisible(false);
		text->setColor(eng::gfx::Color(255, 0, 0));

		text = addChild<eng::gfx::Text2D>("text_frametime");
		m_context.get<eng::gfx::RenderCanvas>().addObject(*text);
		text->setFont(*m_font);
		text->setCharacterSize(24);
		text->setPosition(eng::mth::Vec2(10, 0));

		text = addChild<eng::gfx::Text2D>("text_game_mode");
		m_context.get<eng::gfx::RenderCanvas>().addObject(*text);
		text->setFont(*m_font);
		text->setCharacterSize(16);
		text->setPosition(eng::mth::Vec2(10, 30));

		text = addChild<eng::gfx::Text2D>("tip_free_mode");
		m_context.get<eng::gfx::RenderCanvas>().addObject(*text);
		text->setFont(*m_font);
		text->setCharacterSize(12);
		text->setPosition(eng::mth::Vec2(10, 50));
		text->setString("Press (R + F) for restart in free mode");

		text = addChild<eng::gfx::Text2D>("tip_classic_mode");
		m_context.get<eng::gfx::RenderCanvas>().addObject(*text);
		text->setFont(*m_font);
		text->setCharacterSize(12);
		text->setPosition(eng::mth::Vec2(10, 70));
		text->setString("Press (R + C) for restart in classic mode");

		m_camera_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				auto cam = getChildByName("camera");
				if (cam) static_cast<eng::gfx::Camera2D*>(cam)->setRect(0, size.x, 0, size.y);
		});

		m_game_over_signal_id = m_context.get<eng::core::SignalBus>().subscribe("game_over",
			[this](){
				auto text = static_cast<eng::gfx::Text2D*>(getChildByName("text_game_over"));
				text->setVisible(true);
		});

		m_game_restart_signal_id = m_context.get<eng::core::SignalBus>().subscribe("game_restart",
			[this](){
				auto text = static_cast<eng::gfx::Text2D*>(getChildByName("text_game_over"));
				text->setVisible(false);
		});

		m_game_mode_changed_signal_id = m_context.get<eng::core::SignalBus>().subscribe("game_mode_changed",
			[this](std::string mode){
				auto text = static_cast<eng::gfx::Text2D*>(getChildByName("text_game_mode"));
				text->setString("Game mode: " + mode);
		});
	}

	void onDestroy()
	{
		SceneLayer::onDestroy();
		m_context.get<eng::core::SignalBus>().unsubscribe(m_camera_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_game_over_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_game_restart_signal_id);
		m_context.get<eng::core::SignalBus>().unsubscribe(m_game_mode_changed_signal_id);
	}

	void onUpdate(float delta)
	{
		auto t_ft = static_cast<eng::gfx::Text2D*>(getChildByName("text_frametime"));
		if (t_ft) t_ft->setString(std::to_string(delta));

		SceneLayer::onUpdate(delta);
	}

protected:
	eng::gfx::Font* m_font;
	eng::core::SubscriptionId m_camera_signal_id;
	eng::core::SubscriptionId m_game_over_signal_id;
	eng::core::SubscriptionId m_game_restart_signal_id;
	eng::core::SubscriptionId m_game_mode_changed_signal_id;
};

#endif //UI_SCENE_CLASS_HEADER