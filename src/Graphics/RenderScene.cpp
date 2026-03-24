#include <Engine/Graphics/RenderScene.hpp>

#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/View.hpp>
#include <Engine/Graphics/Color.hpp>

#include <glad/glad.h>
#include <iostream>


namespace eng
{

gfx::RenderScene::RenderScene():
	m_clear_color(Color(0))
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_quad_view.setOrtho(0, 1, 0, 1, -10, 10);

	m_objects2d.clear();
	m_objects3d.clear();
	m_pipeline2d.clear();
	m_pipeline3d.clear();
	m_framebuffers2d.clear();
	m_framebuffers3d.clear();
}

gfx::RenderScene::~RenderScene() {}


void gfx::RenderScene::setClearColor(Color color)
{
	m_clear_color = color;
}

void gfx::RenderScene::addObject(CanvasItem& object)
{
	addObject(object, m_objects2d);
}

void gfx::RenderScene::removeObject(CanvasItem& object)
{
	removeObject(object, m_objects2d);
}

bool gfx::RenderScene::setRenderPipeline2d(const std::vector<RenderPass>& new_pipeline)
{
	return setRenderPipeline(new_pipeline, m_pipeline2d, m_framebuffers2d);
}


void gfx::RenderScene::addObject3d(Drawable& object)
{
	addObject(object, m_objects3d);
}

void gfx::RenderScene::removeObject3d(Drawable& object)
{
	removeObject(object, m_objects3d);
}

bool gfx::RenderScene::setRenderPipeline3d(const std::vector<RenderPass>& new_pipeline)
{
	return setRenderPipeline(new_pipeline, m_pipeline3d, m_framebuffers3d);
}


void gfx::RenderScene::draw2d(RenderTarget& target)
{
	if (m_objects2d.empty() || m_pipeline2d.empty())
		return;

	m_framebuffers2d.back() = &target;

	//render scene
	auto& first_pass = m_pipeline2d.front();
	Shader::setActive(first_pass.shader);

	first_pass.shader->use();
	if (first_pass.uniforms_handler) first_pass.uniforms_handler(first_pass.shader);
	
	View* active_view = gfx::View::getActive2d();
	first_pass.shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());

	if (m_framebuffers2d.size() > 1) m_framebuffers2d[0]->clear(m_clear_color);
	for (Drawable* obj : m_objects2d)
		if (obj && obj->isVisible())
			m_framebuffers2d.front()->draw(*obj, RenderStates());

	// deferred render
	for (unsigned int i = 1; i < m_pipeline2d.size(); i++)
	{
		m_framebuffers2d[i]->clear(m_clear_color);
		auto& pass = m_pipeline2d[i];
		Shader::setActive(pass.shader);

		pass.shader->use();

		// bind last textures
		for (unsigned int j = 0; j < pass.input_buffers_names.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);
			m_framebuffers2d[i - 1]->getTexture(j).bind();
			pass.shader->setUniform1i(pass.input_buffers_names[j], j);
		}

		// bind user uniforms
		if (pass.uniforms_handler) pass.uniforms_handler(pass.shader);

		// draw computed quad
		eng::gfx::Shape2D light_shape(eng::gfx::Shape2D::Type::RECTANGLE);

		pass.shader->setUniformMatrix4fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix4fv("uView", m_quad_view.getViewMatrix().getValuesPtr());

		m_framebuffers2d[i]->draw(light_shape, RenderStates());
	}
}

void gfx::RenderScene::draw3d(RenderTarget& target)
{
	if (m_objects3d.empty() || m_pipeline3d.empty())
		return;

	m_framebuffers3d.back() = &target;

	//render scene
	auto& first_pass = m_pipeline3d.front();
	Shader::setActive(first_pass.shader);

	first_pass.shader->use();
	if (first_pass.uniforms_handler) first_pass.uniforms_handler(first_pass.shader);

	View* active_view = gfx::View::getActive3d();
	first_pass.shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());

	m_framebuffers3d[0]->clear(m_clear_color);
	for (Drawable* obj : m_objects3d)
		if (obj && obj->isVisible())
			m_framebuffers3d[0]->draw(*obj, RenderStates());

	// deferred render
	for (unsigned int i = 1; i < m_pipeline3d.size(); i++)
	{
		m_framebuffers3d[i]->clear(m_clear_color);
		auto& pass = m_pipeline3d[i];
		Shader::setActive(pass.shader);

		pass.shader->use();

		// bind past data
		for (unsigned int j = 0; j < pass.input_buffers_names.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);
			m_framebuffers3d[i - 1]->getTexture(j).bind();
			pass.shader->setUniform1i(pass.input_buffers_names[j], j);
		}

		// bind user uniforms
		if (pass.uniforms_handler) pass.uniforms_handler(pass.shader);

		// draw computed quad
		eng::gfx::Shape2D light_shape(eng::gfx::Shape2D::Type::RECTANGLE);

		pass.shader->setUniformMatrix4fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix4fv("uView", m_quad_view.getViewMatrix().getValuesPtr());

		m_framebuffers3d[i]->draw(light_shape, RenderStates());
	}
}

void gfx::RenderScene::render(RenderTarget& target)
{
	draw3d(target);

	glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    draw2d(target);
    
    glDepthFunc(GL_LESS);
}



void gfx::RenderScene::addObject(Drawable& object, std::vector<Drawable*>& vector)
{
	for (unsigned int i = 0; i < vector.size(); i++)
		if (vector[i] == &object)
			return;
	vector.push_back(&object);
}

void gfx::RenderScene::removeObject(Drawable& object, std::vector<Drawable*>& vector)
{
	for (unsigned int i = 0; i < vector.size(); i++)
		if (vector[i] == &object)
			vector.erase(vector.begin() + i--);
}

bool gfx::RenderScene::setRenderPipeline(const std::vector<RenderPass>& new_pipeline, std::vector<RenderPass>& old_pipeline, std::vector<RenderTarget*>& framebuffers)
{
	old_pipeline = new_pipeline;

	for (const auto& pass : old_pipeline)
		if (!pass.shader) return false;

	for (unsigned int i = 0; i < framebuffers.size(); i++)
		if (framebuffers[i]) delete framebuffers[i];

	framebuffers.reserve(old_pipeline.size());
	for (unsigned int i = 0; i < (old_pipeline.size() - 1); i++)
		framebuffers.push_back(new RenderTarget(
			old_pipeline[i].color_attachments.size(),
			old_pipeline[i].color_attachments.data()
		));
	framebuffers.push_back(nullptr); //for final target in .render(RenderTarget)
	
	return true;
}

} // namespace eng