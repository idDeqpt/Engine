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

		auto camera2d = addChild<eng::gfx::Camera2D>("MainCamera");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		UIScene* ui = addChild<UIScene>("ui_scene", *font);
		ui->setLayer(100);
		m_context.get<eng::gfx::RenderScene>().addObject(*ui);

		GameScene* game = addChild<GameScene>("game_scene");
		game->setLayer(10);
		m_context.get<eng::gfx::RenderScene>().addObject(*game);
	}
};

#endif //ROOT_CLASS_HEADER