#ifndef GAME_SCENE_CLASS_HEADER
#define GAME_SCENE_CLASS_HEADER

#include <scenes/Scene.hpp>
#include <scenes/Camera.hpp>
#include <scenes/Box2D.hpp>
#include <scenes/BallsController.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Math/Vec2.hpp>

class GameScene : public Scene
{
public:
	void onSetup()
	{
		Scene::onSetup();

		auto camera2d = addChild<Camera>("Camera2d");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		addChild<Box2D>("box")->setPosition(eng::mth::Vec2(300, 550));
		addChild<BallsController>("controller");
	}
};

#endif //GAME_SCENE_CLASS_HEADER