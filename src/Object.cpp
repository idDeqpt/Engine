#include <Graphics/Object.hpp>

#include <glad/glad.h>

#include <Graphics/RenderStates.hpp>
#include <Graphics/TextureManager.hpp>
#include <Graphics/Vertex.hpp>
#include <Graphics/Window.hpp>
#include <Math/Transformable3.hpp>


gfx::Object::Object() : m_VAO(0), m_VBO(0), m_EBO(0), m_texture(0), mth::Transformable3() {}

gfx::Object::~Object()
{
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if (m_EBO)
        glDeleteBuffers(1, &m_EBO);
}


bool gfx::Object::create()
{
	if (m_VAO) return false;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    return true;
}

bool gfx::Object::updateData(Vertex* vertices, unsigned int vertices_count, unsigned int* indexes, unsigned int indexies_count)
{
	if ((vertices_count == 0) || (indexies_count == 0)) return false;
	m_indexes_count = indexies_count;

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices_count, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexies_count, indexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT) + 4*sizeof(GLubyte)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void gfx::Object::setTexture(TextureId texture)
{
	m_texture = texture;
}


void gfx::Object::draw(Window* window, RenderStates& states)
{
	states.m_transform = getGlobalTransform();
	states.m_texture = m_texture;

	bool use_texture = states.m_texture != 0;

	states.m_shader->use();
	states.m_shader->setUniform1i("use_texture", use_texture);
	if (use_texture)
	{
		glActiveTexture(GL_TEXTURE0);
		TextureManaget::bind(states.m_texture);
		states.m_shader->setUniformMatrix4fv("u_texture", 0);
	}
	states.m_shader->setUniformMatrix4fv("projection", states.m_view.getProjectionMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("view", states.m_view.getGlobalTransform().getMatrix().getValuesPtr());
	states.m_shader->setUniformMatrix4fv("model", states.m_transform.getMatrix().getValuesPtr());

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glDrawElements(GL_TRIANGLES, m_indexes_count, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
}