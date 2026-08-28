#include <Engine/Graphics/2D/RenderCanvas.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Color.hpp>

#include <algorithm>


namespace eng::gfx
{

RenderCanvas::RenderCanvas():
	RenderScene(),
	m_active_camera(&m_default_camera)
{}


void RenderCanvas::setActiveCamera(Camera2D& camera)
{
	m_active_camera = &camera;
}

Camera2D& RenderCanvas::getActiveCamera()
{
	return *m_active_camera;
}


void RenderCanvas::draw(RenderTarget& target)
{
	if (m_objects.empty() || m_pipeline.empty())
		return;

	gl::Api::getInstance()->setDepthFunction(gl::DepthFunction::ALWAYS);
	gl::Api::getInstance()->enable(gl::Capability::BLEND);
	gl::Api::getInstance()->setBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);

	m_framebuffers.back() = &target;

	//render scene
	auto& first_pass = m_pipeline.front();
	Shader::setActive(first_pass.shader);

	first_pass.shader->use();
	if (first_pass.uniforms_handler) first_pass.uniforms_handler(first_pass.shader);
	
	Camera2D& active_camera = *m_active_camera;
	first_pass.shader->setUniformMatrix3fv("uProjection", active_camera.getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix3fv("uView",       active_camera.getViewMatrix().getValuesPtr());

	if (m_framebuffers.size() > 1) m_framebuffers[0]->clear(Color(0, 0, 0, 0));

	std::vector<CanvasItem*> opaque, transparent;
	opaque.reserve(m_objects.size());
	transparent.reserve(m_objects.size());

	//check transparency
	for (Drawable* obj : m_objects)
		if (obj && obj->isVisible())
		{
			CanvasItem* item = static_cast<CanvasItem*>(obj);
			Texture* texture = item->getTexture();
			if (texture && texture->isTransparent())
				transparent.push_back(item);
			else
				opaque.push_back(item);
		}
	std::sort(transparent.begin(), transparent.end(), [](CanvasItem* a, CanvasItem* b){
		return a->getLayer() < b->getLayer();
	});

	//draw transparent
	gl::Api::getInstance()->useDepthMask(true);
	gl::Api::getInstance()->disable(gl::Capability::BLEND);
	for (CanvasItem* obj : opaque)
		m_framebuffers.front()->draw(*obj, RenderStates());

	//draw opaque
	gl::Api::getInstance()->useDepthMask(false);
	gl::Api::getInstance()->enable(gl::Capability::BLEND);
	gl::Api::getInstance()->setBlendFunction(gl::BlendFactor::SRC_ALPHA, gl::BlendFactor::ONE_MINUS_SRC_ALPHA);

	for (CanvasItem* obj : transparent)
		m_framebuffers.front()->draw(*obj, RenderStates());

	// deferred render
	gl::Api::getInstance()->useDepthMask(false);
	gl::Api::getInstance()->disable(gl::Capability::BLEND);
	for (unsigned int i = 1; i < m_pipeline.size(); i++)
	{
		m_framebuffers[i]->clear(Color(0, 0, 0, 0));
		auto& pass = m_pipeline[i];
		Shader::setActive(pass.shader);

		pass.shader->use();

		// bind last textures
		for (unsigned int j = 0; j < pass.input_buffers_names.size(); j++)
		{
			gfx::gl::Api::getInstance()->setActiveTexture(j);
			m_framebuffers[i - 1]->getTexture(j)->bind();
			pass.shader->setUniform1i(pass.input_buffers_names[j], j);
		}

		// bind user uniforms
		if (pass.uniforms_handler) pass.uniforms_handler(pass.shader);

		// draw computed quad
		eng::gfx::Shape2D light_shape(eng::gfx::Shape2D::Type::RECTANGLE);

		pass.shader->setUniformMatrix3fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix3fv("uView",       m_quad_view.getViewMatrix().getValuesPtr());

		m_framebuffers[i]->draw(light_shape, RenderStates());
	}
}

} // namespace eng::gfx