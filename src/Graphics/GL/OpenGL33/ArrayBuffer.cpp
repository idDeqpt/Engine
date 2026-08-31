#ifdef USE_OPENGL_3_3

#include "ArrayBuffer.hpp"

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>
#include <Engine/Math/Mat4.hpp>

#include <glad/glad.h>

namespace
{

GLenum usageToGlUsage(const eng::gfx::gl::ArrayBuffer::Usage& usage)
{
	switch (usage)
	{
		case eng::gfx::gl::ArrayBuffer::Usage::STATIC_DRAW:
			return GL_STATIC_DRAW;
		case eng::gfx::gl::ArrayBuffer::Usage::DYNAMIC_DRAW:
			return GL_DYNAMIC_DRAW;
		case eng::gfx::gl::ArrayBuffer::Usage::STREAM_DRAW:
			return GL_STREAM_DRAW;
	}
}

GLenum primitiveTypeToGlPrimitiveType(const eng::gfx::PrimitiveType& pt)
{
	switch (pt)
	{
		case eng::gfx::PrimitiveType::POINTS:
			return GL_POINTS;
		case eng::gfx::PrimitiveType::LINES:
			return GL_LINES;
		case eng::gfx::PrimitiveType::LINE_STRIP:
			return GL_LINE_STRIP;
		case eng::gfx::PrimitiveType::LINE_LOOP:
			return GL_LINE_LOOP;
		case eng::gfx::PrimitiveType::TRIANGLES:
			return GL_TRIANGLES;
		case eng::gfx::PrimitiveType::TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		case eng::gfx::PrimitiveType::TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
	}
}

} //namespace

namespace eng::gfx::gl::OpenGL33
{

ArrayBuffer::ArrayBuffer():
	gl::ArrayBuffer(),
	m_EBO(0),
	m_instance_VBO(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

ArrayBuffer::~ArrayBuffer()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}


void ArrayBuffer::loadData(const void* data, unsigned int size, gl::ArrayBuffer::Usage usage)
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, usageToGlUsage(usage));
}

void ArrayBuffer::loadIndexes(const void* data, unsigned int count, gl::ArrayBuffer::Usage usage)
{
	if (m_EBO != 0)
	{
		glDeleteBuffers(1, &m_EBO);
	}
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*count, data, usageToGlUsage(usage));
	m_indexes_count = count;
}

void ArrayBuffer::loadInstances(mth::Mat4* transforms, unsigned int count)
{
	if (m_instance_VBO != 0)
	{
		glDeleteBuffers(1, &m_instance_VBO);
	}
	glGenBuffers(1, &m_instance_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_VBO);

	if (!transforms || !count)
	{
		mth::Mat4 identity = mth::Mat4::getIdentity();
		glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Mat4), &identity, GL_STATIC_DRAW);
		m_instances_count = 1;
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(mth::Mat4)*count, transforms, GL_STATIC_DRAW);
		m_instances_count = count;
	}
}


void ArrayBuffer::configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset)
{
	glBindVertexArray(m_VAO);
	glVertexAttribPointer(index, components, GL_FLOAT, normalized, stride, reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
}

void ArrayBuffer::configureInstanceAttrib(unsigned int index, unsigned int divisor, unsigned int components, bool normalized, unsigned int stride, unsigned int offset)
{
	if (m_instance_VBO == 0) return;

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_VBO);
	glVertexAttribPointer(index, components, GL_FLOAT, normalized, stride, reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
	glVertexAttribDivisor(index, divisor);
}


void ArrayBuffer::draw(PrimitiveType p_type, unsigned int first, unsigned int count)
{
	GLenum gl_p_type = primitiveTypeToGlPrimitiveType(p_type);
	glBindVertexArray(m_VAO);

	if (m_EBO == 0)
	{
		if (m_instance_VBO == 0)
			glDrawArrays(gl_p_type, first, count);
		else
			glDrawArraysInstanced(gl_p_type, first, count, m_instances_count);
	}
	else
	{
		unsigned int draw_count = (count == 0) ? m_indexes_count : count;
		void* pointer = reinterpret_cast<void*>(first * sizeof(unsigned int));

		if (m_instance_VBO == 0)
			glDrawElements(gl_p_type, draw_count, GL_UNSIGNED_INT, pointer);
		else
			glDrawElementsInstanced(gl_p_type, draw_count, GL_UNSIGNED_INT, pointer, m_instances_count);
	}
}

} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3
