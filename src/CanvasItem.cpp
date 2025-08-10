#include <Graphics/CanvasItem.hpp>

#include <Graphics/Color.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/Drawable.hpp>
#include <Math/Transformable2.hpp>

#include <glad/glad.h>
#include <vector>


gfx::CanvasItem::CanvasItem() : m_vertices_count(false), m_visible(true), m_texture(0), Drawable(), mth::Transformable2()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

gfx::CanvasItem::~CanvasItem()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}


void gfx::CanvasItem::setColor(const Color& new_color)
{
	m_color = new_color;
}

void gfx::CanvasItem::setTexture(const TextureId& new_texture)
{
	m_texture = new_texture;
}


bool gfx::CanvasItem::loadData(CanvasItem::Vertex* vertices, unsigned int vertices_count)
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


void gfx::CanvasItem::draw(Window* window, RenderStates& states)
{
	if (!m_visible || !m_vertices_count) return;

	View* active_view = gfx::View::getActive();
	Shader* active_shader = gfx::Shader::getActive();
	mth::Mat4 view_transform_mat = active_view->getGlobalTransform().getMatrix();

	active_shader->use();
	active_shader->setUniformMatrix4fv("uProjection", active_view->getProjectionMatrix().getValuesPtr());
	active_shader->setUniformMatrix4fv("uView", active_view->getViewMatrix().getValuesPtr());
	active_shader->setUniformMatrix3fv("uModel", getGlobalTransform().getMatrix().getValuesPtr());

	float color[4];
	for (unsigned int i = 0; i < 4; i++)
		color[i] = float(m_color[i])/COLOR_MAX_VALUE;
	active_shader->setUniform4fv("uColor", 1, color);

	bool use_texture = m_texture != 0;
	active_shader->setUniform1i("uUseTexture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		TextureManager::bind(m_texture);
		active_shader->setUniform1i("uTexture", 0);
	}

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_vertices_count);
	glBindVertexArray(0);


}