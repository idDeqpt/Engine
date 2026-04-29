#ifndef SCENE_CLASS_HEADER
#define SCENE_CLASS_HEADER

#include <Engine/Graphics/2D/Shape2D.hpp>

#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/RenderScene.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <memory>

class Scene : public eng::gfx::Shape2D
{
public:
	Scene() : eng::gfx::Shape2D()
	{
		eng::gfx::Texture::PixelFormat pf = eng::gfx::Texture::PixelFormat::RGBA;
		m_target = std::make_unique<eng::gfx::RenderTarget>(1, &pf);
		m_target->setViewport(0, 0, 900, 600);

		m_scene = std::make_shared<eng::gfx::RenderScene>();
		m_scene->setClearColor(eng::gfx::Color(0, 0, 0, 0));
	}

	void onSetup()
	{
		setType(eng::gfx::Shape2D::Type::RECTANGLE);
		setSize(eng::mth::Vec2(900, 600));
		setTexture(m_target->getTexture(0));
		getTexture()->setFlip(false, true);

		m_scene->setRenderPipeline2D(m_context.get<eng::gfx::RenderScene>().getRenderPipeline2D());
		m_context.replace<eng::gfx::RenderScene>(m_scene);
	}

	void onUpdate(float delta)
	{
		m_scene->render(*m_target);
	}

protected:
	std::shared_ptr<eng::gfx::RenderScene> m_scene;
	std::unique_ptr<eng::gfx::RenderTarget> m_target;
};

#endif //SCENE_CLASS_HEADER