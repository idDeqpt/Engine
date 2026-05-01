#ifndef UI_SCENE_CLASS_HEADER
#define UI_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>

#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/2D/Text2D.hpp>
#include <Engine/Graphics/Font.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Math/Vec2.hpp>

class UIScene : public SceneLayer
{
public:
	UIScene(eng::gfx::Font& font):
		m_font(&font),
		SceneLayer() {}

	void onSetup()
	{
		SceneLayer::onSetup();

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

		SceneLayer::onUpdate(delta);
	}

protected:
	eng::gfx::Font* m_font;
};

#endif //UI_SCENE_CLASS_HEADER