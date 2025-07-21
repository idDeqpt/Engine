#include <Graphics/VertexBuffer.hpp>

#include <Graphics/Vertex.hpp>

#include <glad/glad.h>


gfx::VertexBuffer::VertexBuffer() : m_vao_id(0), m_vbo_id(0), m_ebo_id(0) {}

gfx::VertexBuffer::~VertexBuffer()
{
    if (m_vao_id)
    {
        glDeleteVertexArrays(1, &m_vao_id);
        glDeleteBuffers(1, &m_vbo_id);
    }
    if (m_ebo_id)
        glDeleteBuffers(1, &m_ebo_id);
}


bool gfx::VertexBuffer::create(unsigned int vertex_count)
{
	if (m_vao_id) return false;
	m_size = vertex_count;

    glGenVertexArrays(1, &m_vao_id);
    glGenBuffers(1, &m_vbo_id);

    return true;
}

bool gfx::VertexBuffer::update(Vertex* vertices)
{
	if (!m_vao_id) return false;

    glBindVertexArray(m_vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3*sizeof(GL_FLOAT) + 4*sizeof(GLubyte)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


unsigned int gfx::VertexBuffer::getVAOHandle()
{
    return m_vao_id;
}

unsigned int gfx::VertexBuffer::getVBOHandle()
{
    return m_vbo_id;
}

unsigned int gfx::VertexBuffer::getEBOHandle()
{
    return m_ebo_id;
}

unsigned int gfx::VertexBuffer::getSize()
{
    return m_size;
}


void gfx::VertexBuffer::bind(VertexBuffer* vertex_buffer)
{
    glBindVertexArray(vertex_buffer ? vertex_buffer->m_vao_id : 0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer ? vertex_buffer->m_vbo_id : 0);
}