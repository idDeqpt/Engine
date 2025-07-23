#include <Graphics/Shape.hpp>

#include <Graphics/Color.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Drawable.hpp>
#include <Graphics/RenderStates.hpp>

#include <vector>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Transformable3.hpp>


gfx::Shape::Shape() : mth::Transformable3()
{
	m_vertices.clear();
	m_vertices_buffer.create();
	m_texture = 0;
}


void gfx::Shape::setColor(const Color& color)
{
	m_color = color;
}

void gfx::Shape::setTexture(TextureId texture)
{
	m_texture = texture;
}


void gfx::Shape::addPoint(const mth::Vec3& vertex_pos, const mth::Vec2& vertex_tex_pos)
{
	m_vertices.push_back({vertex_pos, m_color, vertex_tex_pos});
}

void gfx::Shape::update()
{
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_vertices[i].color = m_color;
	m_vertices_buffer.update(m_vertices.data(), m_vertices.size());
}


void gfx::Shape::draw(Window* window, RenderStates& states)
{
	states.m_texture = m_texture;
	states.m_transform = getGlobalTransform();
	window->draw(m_vertices_buffer, states);
}