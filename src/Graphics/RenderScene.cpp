#include <Engine/Graphics/RenderScene.hpp>

#include <Engine/Graphics/2D/CanvasItem.hpp>
#include <Engine/Graphics/2D/Shape2D.hpp>
#include <Engine/Graphics/2D/Camera2D.hpp>
#include <Engine/Graphics/3D/Camera3D.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Color.hpp>

#include <glad/glad.h>
#include <algorithm>


namespace eng
{

gfx::RenderScene::RenderScene():
	m_clear_color(Color(0)),
	m_active_camera_2d(&m_default_camera_2d),
	m_active_camera_3d(&m_default_camera_3d)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_quad_view.setRect(0, 1, 0, 1);

	m_objects2d.clear();
	m_objects3d.clear();
	m_pipeline2d.clear();
	m_pipeline3d.clear();
	m_framebuffers2d.clear();
	m_framebuffers3d.clear();
}

gfx::RenderScene::~RenderScene() {}


void gfx::RenderScene::addObject(CanvasItem& object)
{
	addObject(object, m_objects2d);
}

void gfx::RenderScene::addObject3D(Drawable& object)
{
	addObject(object, m_objects3d);
}


void gfx::RenderScene::removeObject(CanvasItem& object)
{
	removeObject(object, m_objects2d);
}

void gfx::RenderScene::removeObject3D(Drawable& object)
{
	removeObject(object, m_objects3d);
}


void gfx::RenderScene::setClearColor(Color color)
{
	m_clear_color = color;
}


void gfx::RenderScene::setActiveCamera(Camera2D& camera)
{
	m_active_camera_2d = &camera;
}

void gfx::RenderScene::setActiveCamera(Camera3D& camera)
{
	m_active_camera_3d = &camera;
}


bool gfx::RenderScene::setRenderPipeline2D(const std::vector<RenderPass>& new_pipeline)
{
	return setRenderPipeline(new_pipeline, m_pipeline2d, m_framebuffers2d);
}

bool gfx::RenderScene::setRenderPipeline3D(const std::vector<RenderPass>& new_pipeline)
{
	return setRenderPipeline(new_pipeline, m_pipeline3d, m_framebuffers3d);
}


gfx::Camera2D& gfx::RenderScene::getActiveCamera2D()
{
	return *m_active_camera_2d;
}

gfx::Camera3D& gfx::RenderScene::getActiveCamera3D()
{
	return *m_active_camera_3d;
}


std::vector<gfx::RenderScene::RenderPass>& gfx::RenderScene::getRenderPipeline2D()
{
	return m_pipeline2d;
}

std::vector<gfx::RenderScene::RenderPass>& gfx::RenderScene::getRenderPipeline3D()
{
	return m_pipeline3d;
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
	
	Camera2D& active_camera = *m_active_camera_2d;
	first_pass.shader->setUniformMatrix3fv("uProjection", active_camera.getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix3fv("uView",       active_camera.getViewMatrix().getValuesPtr());

	if ((m_framebuffers2d.size() > 1) && (m_framebuffers3d.empty())) m_framebuffers2d[0]->clear(m_clear_color);

	std::vector<CanvasItem*> opaque, transparent;
	opaque.reserve(m_objects2d.size());
	transparent.reserve(m_objects2d.size());

	//check transparency
	for (Drawable* obj : m_objects2d)
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
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	for (CanvasItem* obj : opaque)
		m_framebuffers2d.front()->draw(*obj, RenderStates());

	//draw opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (CanvasItem* obj : transparent)
		m_framebuffers2d.front()->draw(*obj, RenderStates());

	// deferred render
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
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

		pass.shader->setUniformMatrix3fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix3fv("uView",       m_quad_view.getViewMatrix().getValuesPtr());

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

	Camera3D& active_camera = *m_active_camera_3d;
	first_pass.shader->setUniformMatrix4fv("uProjection", active_camera.getProjectionMatrix().getValuesPtr());
	first_pass.shader->setUniformMatrix4fv("uView",       active_camera.getViewMatrix().getValuesPtr());

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

		pass.shader->setUniformMatrix3fv("uProjection", m_quad_view.getProjectionMatrix().getValuesPtr());
		pass.shader->setUniformMatrix3fv("uView",       m_quad_view.getViewMatrix().getValuesPtr());

		m_framebuffers3d[i]->draw(light_shape, RenderStates());
	}
}

void gfx::RenderScene::render(RenderTarget& target)
{
	target.clear(m_clear_color);
	
	glDepthFunc(GL_LESS);
	draw3d(target);

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	draw2d(target);
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
	{
		framebuffers.push_back(new RenderTarget(
			old_pipeline[i].color_attachments.size(),
			old_pipeline[i].color_attachments.data()
		));
		framebuffers.back()->setViewport(old_pipeline[i].viewport_position.x, old_pipeline[i].viewport_position.y,
										 old_pipeline[i].viewport_size.x,     old_pipeline[i].viewport_size.y);
	}
	framebuffers.push_back(nullptr); //for final target in .render(RenderTarget)
	
	return true;
}

} // namespace eng