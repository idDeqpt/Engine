#include <Engine/Graphics/RenderScene.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Color.hpp>

#include <algorithm>


namespace eng::gfx
{

RenderScene::RenderScene()
{
	gl::Api::getInstance()->enable(gl::Capability::DEPTH_TEST);
	gl::Api::getInstance()->enable(gl::Capability::BLEND);
	gl::Api::getInstance()->setBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);

	m_quad_view.setRect(0, 1, 0, 1);
}


void RenderScene::addObject(Drawable& object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), &object);
	if (it != m_objects.end()) return;
	
	m_objects.push_back(&object);
}


void RenderScene::removeObject(Drawable& object)
{
	auto it = std::find(m_objects.begin(), m_objects.end(), &object);
	if (it == m_objects.end()) return;
	
	m_objects.erase(it);
}


bool RenderScene::setRenderPipeline(const std::vector<RenderPass>& new_pipeline)
{
	m_pipeline = new_pipeline;

	for (const auto& pass : m_pipeline)
		if (!pass.shader) return false;

	for (unsigned int i = 0; i < m_framebuffers.size(); i++)
		if (m_framebuffers[i]) delete m_framebuffers[i];

	m_framebuffers.reserve(m_pipeline.size());
	for (unsigned int i = 0; i < (m_pipeline.size() - 1); i++)
	{
		m_framebuffers.push_back(new RenderTarget(
			m_pipeline[i].color_attachments.size(),
			m_pipeline[i].color_attachments.data()
		));
		m_framebuffers.back()->setViewport(m_pipeline[i].viewport_position.x, m_pipeline[i].viewport_position.y,
		                                   m_pipeline[i].viewport_size.x,     m_pipeline[i].viewport_size.y);
	}
	m_framebuffers.push_back(nullptr); //for final target in .render(RenderTarget)
	
	return true;
}

std::vector<RenderScene::RenderPass>& RenderScene::getRenderPipeline()
{
	return m_pipeline;
}


void RenderScene::render(RenderTarget& target)
{
	draw(target);
}

} // namespace eng::gfx