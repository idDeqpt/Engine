#ifndef GAME_SCENE_CLASS_HEADER
#define GAME_SCENE_CLASS_HEADER

#include <scenes/SceneLayer.hpp>
#include <scenes/game/Camera.hpp>

#include <Engine/Physics/2D/RectangleCollider2D.hpp>
#include <Engine/Graphics/2D/RenderCanvas.hpp>
#include <Engine/Graphics/2D/Sprite2D.hpp>
#include <Engine/Core/ResourceManager.hpp>
#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Core/Delta.hpp>
#include <Engine/Math/Vec.hpp>

class GameScene : public SceneLayer
{
protected:
	Camera* m_camera2d = nullptr;
	std::vector<eng::gfx::Sprite2D*> m_sprites;

public:
	void onBuild() override
	{
		eng::mth::Vec2 box_pos(0, 50);

		m_camera2d   = addChild<Camera>("camera");

		const int left = -800;
		const int right = 800;
		const int top = -400;
		const int bottom = 400;
		const unsigned int count = 200;
		for (unsigned int i = 0; i < count; i++)
			for (unsigned int j = 0; j < count; j++)
			{
				m_sprites.push_back(addChild<eng::gfx::Sprite2D>("sprite"));
				m_sprites.back()->setPosition(eng::mth::Vec2(left + (right - left)*(static_cast<float>(i)/count), top + (bottom - top)*(static_cast<float>(j)/count)));
				m_sprites.back()->setScale(0.05);
			}
	}

	void onSetup() override
	{
		SceneLayer::onSetup();

		eng::mth::Vec2 v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");
		eng::mth::Vec2 c_size(1000*(v_size.x/v_size.y), 1000);
		m_camera2d->setSize(c_size);
		m_context.get<eng::gfx::RenderCanvas>().setActiveCamera(*m_camera2d);

		m_camera_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				m_camera2d->setSize(eng::mth::Vec2(1000*(size.x/size.y), 1000));
		});

		auto* tex = m_context.get<eng::core::ResourceManager>().load<eng::gfx::Texture>({"resources/fruit0.png"}).second;
		for (auto* sprite : m_sprites)
		{
			tex->setSmooth(true);
			sprite->setTexture(tex);
			m_context.get<eng::gfx::RenderCanvas>().addObject(*sprite);
		}
	}

	void onDestroy() override
	{
		SceneLayer::onDestroy();
		m_context.get<eng::core::SignalBus>().unsubscribe(m_camera_signal_id);

		for (auto sprite : m_sprites)
			m_context.get<eng::gfx::RenderCanvas>().removeObject(*sprite);
	}

	void onUpdate(const eng::core::Delta& delta) override
	{
		SceneLayer::onUpdate(delta);
	}

protected:
	eng::core::SubscriptionId m_camera_signal_id;
};

#endif //GAME_SCENE_CLASS_HEADER