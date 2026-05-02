#ifndef GAME_SCENE_CLASS_HEADER
#define GAME_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>
#include <scenes/game/Camera.hpp>
#include <scenes/game/Box2D.hpp>
#include <scenes/game/BallsController.hpp>
#include <scenes/game/sandbox/SandboxBallsController.hpp>
#include <scenes/game/classic/ClassicBallsController.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Math/Vec2.hpp>

class GameScene : public SceneLayer
{
public:
	void onSetup()
	{
		SceneLayer::onSetup();

		auto camera2d = addChild<Camera>("Camera2d");
		camera2d->setSize(eng::mth::Vec2(900, 600));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		eng::mth::Vec2 box_pos(0, 250);
		auto box = addChild<Box2D>("box");
		box->setPosition(box_pos);
		addChild<ClassicBallsController>("controller", box_pos + box->getLeftBound(), box_pos + box->getRightBound(), box->getBottomBound());
		//addChild<SandboxBallsController>("controller");
	}

	void onUpdate(float delta)
	{
		auto* controller = static_cast<BallsController*>(getChildByName("controller"));
		if (controller && controller->isGameOver())
		{
			eng::core::Logger::info("Game over");
		}

		SceneLayer::onUpdate(delta);
	}
};

#endif //GAME_SCENE_CLASS_HEADER