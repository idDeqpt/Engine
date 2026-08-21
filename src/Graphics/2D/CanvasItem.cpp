#include <Engine/Graphics/2D/CanvasItem.hpp>

#include <Engine/Core/Node2D.hpp>
#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Graphics/RenderTarget.hpp>
#include <Engine/Graphics/RenderStates.hpp>

#include <vector>
#include <memory>


namespace eng
{

gfx::CanvasItem::CanvasItem():
	m_vertices_count(0),
	m_layer(0),
	m_primitive_type(PrimitiveType::TRIANGLE_FAN),
	m_texture(nullptr), Drawable(), core::Node2D()
{
	m_array_buffer = gl::Api::getInstance()->createArrayBuffer();
}


void gfx::CanvasItem::setColor(const gfx::Color& new_color)
{
	m_color = new_color;
}

void gfx::CanvasItem::setLayer(unsigned int new_layer)
{
	m_layer = new_layer;
}

void gfx::CanvasItem::setTexture(gfx::Texture* new_texture)
{
	m_texture = new_texture;
}

void gfx::CanvasItem::setPrimitiveType(const gfx::PrimitiveType& new_primitive_type)
{
	m_primitive_type = new_primitive_type;
}


unsigned int gfx::CanvasItem::getLayer() const
{
	return m_layer;
}

gfx::Texture* gfx::CanvasItem::getTexture()
{
	return m_texture;
}


bool gfx::CanvasItem::loadData(gfx::CanvasItem::Vertex* vertices, unsigned int vertices_count)
{
	if (!vertices || !vertices_count) return false;

	m_array_buffer->loadData(vertices, sizeof(CanvasItem::Vertex)*vertices_count, gl::ArrayBuffer::Usage::STATIC_DRAW);
	m_array_buffer->configureAttrib(0, 2, false, sizeof(CanvasItem::Vertex), offsetof(CanvasItem::Vertex, position));
	m_array_buffer->configureAttrib(1, 2, false, sizeof(CanvasItem::Vertex), offsetof(CanvasItem::Vertex, tex_coord));

	m_vertices_count = vertices_count;
	return true;
}

void gfx::CanvasItem::unloadData()
{
	m_vertices_count = 0;
}


void gfx::CanvasItem::draw(gfx::RenderTarget* target, const gfx::RenderStates& states)
{
	if (!m_visible || !m_vertices_count) return;

	Shader* active_shader = gfx::Shader::getActive();

	active_shader->use();
	active_shader->setUniformMatrix3fv("uModel", getGlobalTransform2D().value().getMatrix().getValuesPtr());
	active_shader->setUniform1ui("uLayer", m_layer);

	float color[4];
	for (unsigned int i = 0; i < 4; i++)
		color[i] = float(m_color[i])/COLOR_MAX_VALUE;
	active_shader->setUniform4fv("uColor", color);

	bool use_texture = m_texture != nullptr;
	active_shader->setUniform1i("uUseTexture", use_texture);
	if (use_texture)
	{
		active_shader->setUniform1i("uTextureFlipX", m_texture->getFlipX());
		active_shader->setUniform1i("uTextureFlipY", m_texture->getFlipY());
		gl::Api::getInstance()->setActiveTexture(0);
		m_texture->bind();
		active_shader->setUniform1i("uTexture", 0);
	}

	m_array_buffer->draw(m_primitive_type, 0, m_vertices_count);
}

} //namespace eng