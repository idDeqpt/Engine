#include <Engine/Graphics/CanvasItem.hpp>

#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Shader.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>

#include <glad/glad.h>
#include <vector>


namespace eng
{

gfx::CanvasItem::CanvasItem():
	m_vertices_count(false),
	m_primitive_type(PrimitiveType::TRIANGLE_FAN),
	m_texture(nullptr), Drawable(), core::Node2D()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

gfx::CanvasItem::~CanvasItem()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}


void gfx::CanvasItem::setColor(const gfx::Color& new_color)
{
	m_color = new_color;
}

void gfx::CanvasItem::setTexture(gfx::Texture& new_texture)
{
	m_texture = &new_texture;
}

void gfx::CanvasItem::setPrimitiveType(const gfx::PrimitiveType& new_primitive_type)
{
	m_primitive_type = new_primitive_type;
}


bool gfx::CanvasItem::loadData(gfx::CanvasItem::Vertex* vertices, unsigned int vertices_count)
{
	if (!vertices || !vertices_count) return false;

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CanvasItem::Vertex)*vertices_count, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CanvasItem::Vertex), (GLvoid*)offsetof(CanvasItem::Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CanvasItem::Vertex), (GLvoid*)offsetof(CanvasItem::Vertex, tex_coord));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	m_vertices_count = vertices_count;
	return true;
}

void gfx::CanvasItem::unloadData()
{
	m_vertices_count = 0;
}


void gfx::CanvasItem::draw(gfx::RenderTarget* target, gfx::RenderStates& states)
{
	if (!m_visible || !m_vertices_count) return;

	static constexpr GLenum OPENGL_PRIMITIVE_TYPES[] = {
		GL_POINTS,
		GL_LINES,
		GL_LINE_STRIP,
		GL_LINE_LOOP,
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN
	};
	const GLenum primitive_type = OPENGL_PRIMITIVE_TYPES[int(m_primitive_type)];

	Shader* active_shader = gfx::Shader::getActive();

	active_shader->use();
	active_shader->setUniformMatrix3fv("uModel", getGlobalTransform2D().value().getMatrix().getValuesPtr());

	float color[4];
	for (unsigned int i = 0; i < 4; i++)
		color[i] = float(m_color[i])/COLOR_MAX_VALUE;
	active_shader->setUniform4fv("uColor", 1, color);

	bool use_texture = m_texture != nullptr;
	active_shader->setUniform1i("uUseTexture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		m_texture->bind();
		active_shader->setUniform1i("uTexture", 0);
	}

	glBindVertexArray(m_VAO);
	glDrawArrays(primitive_type, 0, m_vertices_count);
	glBindVertexArray(0);
}

} //namespace eng