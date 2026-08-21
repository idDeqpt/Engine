#ifdef USE_OPENGL_3_3

#include "ArrayBuffer.hpp"

#include <Engine/Graphics/GL/ArrayBuffer.hpp>
#include <Engine/Graphics/PrimitiveType.hpp>

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

ArrayBuffer::ArrayBuffer() : gl::ArrayBuffer()
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

void ArrayBuffer::configureAttrib(unsigned int index, unsigned int components, bool normalized, unsigned int stride, unsigned int offset)
{
	glBindVertexArray(m_VAO);
	glVertexAttribPointer(index, components, GL_FLOAT, normalized, stride, reinterpret_cast<const void*>(offset));
	glEnableVertexAttribArray(index);
}

void ArrayBuffer::draw(PrimitiveType p_type, unsigned int first, unsigned int count)
{
	glBindVertexArray(m_VAO);
	glDrawArrays(primitiveTypeToGlPrimitiveType(p_type), first, count);
}

} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3
