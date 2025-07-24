#include <Graphics/Object.hpp>

#include <glad/glad.h>
#include <iostream>

#include <Graphics/RenderStates.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/Color.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Transformable3.hpp>


gfx::Object::Object() : m_VAO(0), m_VBO{0, 0, 0}, m_EBO(0), m_inited(false), m_has_tex_coords(false), m_texture(0), mth::Transformable3()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(3, m_VBO);
	glGenBuffers(1, &m_EBO);
}

gfx::Object::~Object()
{
	if (m_VAO)
		glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO)
		glDeleteBuffers(3, m_VBO);
	if (m_EBO)
		glDeleteBuffers(1, &m_EBO);
}


bool gfx::Object::loadData(ObjectData data)
{
	if (!data.points || !(data.colors || data.tex_coords) || !data.indexes) return m_inited = false;
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Vec3)*data.vertices_count, data.points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mth::Vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	if (data.colors)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Color)*data.vertices_count, data.colors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color), (GLvoid*)0);
		glEnableVertexAttribArray(1);
	}

	m_has_tex_coords = data.tex_coords != nullptr;
	if (m_has_tex_coords)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Vec2)*data.vertices_count, data.tex_coords, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mth::Vec2), (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*data.indexes_count, data.indexes, GL_STATIC_DRAW);
	m_indexes_count = data.indexes_count;
	
	glBindVertexArray(0);
	return m_inited = true;
}

void gfx::Object::setTexture(TextureId texture)
{
	m_texture = texture;
}


void gfx::Object::draw(Window* window, RenderStates& states)
{
	if (!m_inited) return;

	states.m_transform = getGlobalTransform();
	states.m_texture = m_texture;

	bool use_texture = states.m_texture && m_has_tex_coords;

	states.m_shader->use();
	states.m_shader->setUniform1i("use_texture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		TextureManager::bind(states.m_texture);
		states.m_shader->setUniformMatrix4fv("u_texture", 0);
	}
	states.m_shader->setUniformMatrix4fv("projection", states.m_view.getProjectionMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("view", states.m_view.getGlobalTransform().getMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("model", states.m_transform.getMatrix().getValuesPtr());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexes_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}