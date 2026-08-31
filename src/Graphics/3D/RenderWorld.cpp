#include <Engine/Graphics/3D/RenderWorld.hpp>

#include <Engine/Graphics/GL/Api.hpp>
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

RenderWorld::RenderWorld():
	RenderScene(),
	m_active_camera(&m_default_camera)
{}


void RenderWorld::setActiveCamera(Camera3D& camera)
{
	m_active_camera = &camera;
}

gfx::Camera3D& RenderWorld::getActiveCamera()
{
	return *m_active_camera;
}


void RenderWorld::draw(RenderTarget& target)
{
	if (m_objects.empty() || m_pipeline.empty())
		return;
	
	gl::Api::getInstance()->setDepthFunction(gl::DepthFunction::LESS);

	m_framebuffers.back() = &target;

	//render scene
	auto& first_pass = m_pipeline.front();
	Shader::setActive(first_pass.shader);

	first_pass.shader->use();
	if (first_pass.uniforms_handler) first_pass.uniforms_handler(first_pass.shader);

	Camera3D& active_camera = *m_active_camera;
	first_pass.shader->setUniformMatrix4fv("uProjection", active_camera.getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix4fv("uView",       active_camera.getViewMatrix().getValuesPtr());

	if (m_framebuffers.size() > 1) m_framebuffers[0]->clear(Color(255, 0, 0, 0));

	for (Drawable* obj : m_objects)
		if (obj && obj->isVisible())
			m_framebuffers[0]->draw(*obj, RenderStates());

	// deferred render
	for (unsigned int i = 1; i < m_pipeline.size(); i++)
	{
		m_framebuffers[i]->clear(Color(0, 0, 255, 0));
		auto& pass = m_pipeline[i];
		Shader::setActive(pass.shader);

		pass.shader->use();

		// bind past data
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
		light_shape.setPosition(light_shape.getSize()/2);

		pass.shader->setUniformMatrix3fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix3fv("uView",       m_quad_view.getViewMatrix().getValuesPtr());

		m_framebuffers[i]->draw(light_shape, RenderStates());
	}
}

} // namespace eng::gfx