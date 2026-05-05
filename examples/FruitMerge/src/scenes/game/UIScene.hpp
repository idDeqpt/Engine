#ifndef UI_SCENE_CLASS_HEADER
#define UI_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>

#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/RenderScene.hpp>
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
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		auto t_ft = addChild<eng::gfx::Text2D>("text_frametime");
		m_context.get<eng::gfx::RenderScene>().addObject(*t_ft);
		t_ft->setFont(*m_font);
		t_ft->setCharacterSize(24);

		m_camera_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				auto cam = static_cast<eng::gfx::Camera2D*>(getChildByName("camera"));
				cam->setRect(0, size.x, 0, size.y);
		});
	}

	void onDestroy()
	{
		SceneLayer::onDestroy();
		m_context.get<eng::core::SignalBus>().unsubscribe(m_camera_signal_id);
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
};

#endif //UI_SCENE_CLASS_HEADER