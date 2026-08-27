#ifndef SCENE_LAYER_CLASS_HEADER
#define SCENE_LAYER_CLASS_HEADER

#include <Engine/Graphics/2D/Shape2D.hpp>

#include <Engine/Core/ConfigManager.hpp>
#include <Engine/Core/SignalBus.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <memory>

class SceneLayer : public eng::gfx::Shape2D
{
public:
	void onSetup()
	{
		eng::mth::Vec2 v_size = m_context.get<eng::core::ConfigManager>().get<eng::mth::Vec2>("window_viewport_size");

		eng::gfx::gl::PixelFormat pf = eng::gfx::gl::PixelFormat::RGBA;
		m_target = std::make_unique<eng::gfx::RenderTarget>(1, &pf);
		m_target->setViewport(0, 0, v_size.x, v_size.y);

		m_scene = std::make_shared<eng::gfx::RenderScene>();
		m_scene->setClearColor(eng::gfx::Color(0, 0, 0, 0));

		setType(eng::gfx::Shape2D::Type::RECTANGLE);
		setSize(v_size);
		setTexture(m_target->getTexture(0));
		getTexture()->setFlip(false, true);

		m_scene->setRenderPipeline2D(m_context.get<eng::gfx::RenderScene>().getRenderPipeline2D());
		m_context.replace<eng::gfx::RenderScene>(m_scene);

		m_viewport_signal_id = m_context.get<eng::core::SignalBus>().subscribe("on_change_config_window_viewport_size",
			[this](eng::mth::Vec2 size){
				setSize(size);
				m_target->setViewport(0, 0, size.x, size.y);
		});
	}

	void onDestroy()
	{
		m_context.get<eng::core::SignalBus>().unsubscribe(m_viewport_signal_id);
	}

	void onUpdate(float delta)
	{
		m_scene->render(*m_target);
	}

protected:
	std::shared_ptr<eng::gfx::RenderScene> m_scene;
	std::unique_ptr<eng::gfx::RenderTarget> m_target;
	eng::core::SubscriptionId m_viewport_signal_id;
};

#endif //SCENE_LAYER_CLASS_HEADER