#ifndef GAME_SCENE_CLASS_HEADER
#define GAME_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>
#include <scenes/game/Camera.hpp>
#include <scenes/game/Box2D.hpp>
#include <scenes/game/BallsController.hpp>
#include <scenes/game/sandbox/SandboxBallsController.hpp>
#include <scenes/game/classic/ClassicBallsController.hpp>

#include <Engine/Graphics/2D/RenderCanvas.hpp>
#include <Engine/Core/ConfigManager.hpp>
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
		m_context.get<eng::gfx::RenderCanvas>().setActiveCamera(*camera2d);

		eng::mth::Vec2 box_pos(0, 50);
		m_box = addChild<Box2D>("box");
		m_box->setPosition(box_pos);
		addChild<ClassicBallsController>("controller", box_pos + m_box->getLeftBound(), box_pos + m_box->getRightBound());
		m_context.get<eng::core::SignalBus>().emit("game_mode_changed", std::string("classic"));

		m_camera_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				auto cam = getChildByName("camera");
				if (cam) static_cast<Camera*>(cam)->setSize(eng::mth::Vec2(1000*(size.x/size.y), 1000));
		});
	}

	void onDestroy()
	{
		SceneLayer::onDestroy();
		m_context.get<eng::core::SignalBus>().unsubscribe(m_camera_signal_id);
	}

	void onUpdate(float delta)
	{
		auto event_manager = m_context.get<eng::sys::EventManager>();
		auto* controller = static_cast<BallsController*>(getChildByName("controller"));

		if (controller && controller->isGameOver())
			m_context.get<eng::core::SignalBus>().emit("game_over");

		if (event_manager.getKeyboard().isPressed(eng::sys::Keyboard::Key::R))
		{
			if (event_manager.getKeyboard().isJustPressed(eng::sys::Keyboard::Key::F))
			{
				removeChild(controller);
				addChild<SandboxBallsController>("controller");
				m_context.get<eng::core::SignalBus>().emit("game_restart");
				m_context.get<eng::core::SignalBus>().emit("game_mode_changed", std::string("free"));
			}
			else if (event_manager.getKeyboard().isJustPressed(eng::sys::Keyboard::Key::C))
			{
				removeChild(controller);
				addChild<ClassicBallsController>("controller", m_box->getPosition().x + m_box->getLeftBound(), m_box->getPosition().x + m_box->getRightBound());
				m_context.get<eng::core::SignalBus>().emit("game_restart");
				m_context.get<eng::core::SignalBus>().emit("game_mode_changed", std::string("classic"));
			}
		}

		SceneLayer::onUpdate(delta);
	}

protected:
	eng::core::SubscriptionId m_camera_signal_id;
	Box2D* m_box;
};

#endif //GAME_SCENE_CLASS_HEADER