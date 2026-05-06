#ifndef GAME_SCENE_CLASS_HEADER
#define GAME_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>
#include <scenes/game/Camera.hpp>
#include <scenes/game/Box2D.hpp>
#include <scenes/game/BallsController.hpp>
#include <scenes/game/sandbox/SandboxBallsController.hpp>
#include <scenes/game/classic/ClassicBallsController.hpp>

#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Core/Logger.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Math/Vec2.hpp>

class GameScene : public SceneLayer
{
public:
	void onSetup()
	{
		SceneLayer::onSetup();

		auto camera2d = addChild<Camera>("camera");
		eng::mth::Vec2 v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
		camera2d->setSize(eng::mth::Vec2(1000*(v_size.x/v_size.y), 1000));
		m_context.get<eng::gfx::RenderScene>().setActiveCamera(*camera2d);

		eng::mth::Vec2 box_pos(0, 250);
		auto box = addChild<Box2D>("box");
		box->setPosition(box_pos);
		addChild<ClassicBallsController>("controller", box_pos + box->getLeftBound(), box_pos + box->getRightBound(), box->getBottomBound());
		//addChild<SandboxBallsController>("controller");

		m_camera_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				auto cam = static_cast<Camera*>(getChildByName("camera"));
				cam->setSize(eng::mth::Vec2(1000*(size.x/size.y), 1000));
		});
	}

	void onDestroy()
	{
		SceneLayer::onDestroy();
		m_context.get<eng::core::SignalBus>().unsubscribe(m_camera_signal_id);
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

protected:
	eng::core::SubscriptionId m_camera_signal_id;
};

#endif //GAME_SCENE_CLASS_HEADER