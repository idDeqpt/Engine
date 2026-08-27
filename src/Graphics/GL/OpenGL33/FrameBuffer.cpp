#ifdef USE_OPENGL_3_3

#include "FrameBuffer.hpp"

#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>

#include <glad/glad.h>


namespace
{

unsigned int OPENGL_ATTACHMENTS[] = {
	GL_COLOR_ATTACHMENT0,  GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,  GL_COLOR_ATTACHMENT5,  GL_COLOR_ATTACHMENT6,  GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,  GL_COLOR_ATTACHMENT9,  GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15,
	GL_COLOR_ATTACHMENT16, GL_COLOR_ATTACHMENT17, GL_COLOR_ATTACHMENT18, GL_COLOR_ATTACHMENT19,
	GL_COLOR_ATTACHMENT20, GL_COLOR_ATTACHMENT21, GL_COLOR_ATTACHMENT22, GL_COLOR_ATTACHMENT23,
	GL_COLOR_ATTACHMENT24, GL_COLOR_ATTACHMENT25, GL_COLOR_ATTACHMENT26, GL_COLOR_ATTACHMENT27,
	GL_COLOR_ATTACHMENT28, GL_COLOR_ATTACHMENT29, GL_COLOR_ATTACHMENT30, GL_COLOR_ATTACHMENT31
};

} //namespace

namespace eng::gfx::gl::OpenGL33
{

FrameBuffer::FrameBuffer():
    gl::FrameBuffer(),
    m_handle(0),
    m_depth_buffer_handle(0),
    m_color_buffers_count(0),
    m_buffer_textures(nullptr)
{}

FrameBuffer::~FrameBuffer()
{
	remove();
}


void FrameBuffer::create(unsigned int buffers_count, gl::PixelFormat* formats)
{
	remove();

    m_color_buffers_count = buffers_count;

	glGenFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	glGenRenderbuffers(1, &m_depth_buffer_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer_handle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1, 1);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer_handle);

	if (m_color_buffers_count)
	{
		m_buffer_textures = new Texture[m_color_buffers_count];
		for (unsigned int i = 0; i < m_color_buffers_count; i++)
		{
			m_buffer_textures[i].create(formats[i]);
			m_buffer_textures[i].setSmooth(false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_buffer_textures[i].getNativeHandle(), 0);
		}
		glDrawBuffers(m_color_buffers_count, OPENGL_ATTACHMENTS);
	}
}

void FrameBuffer::remove()
{
	if (m_handle)              glDeleteFramebuffers(1, &m_handle);
	if (m_depth_buffer_handle) glDeleteRenderbuffers(1, &m_depth_buffer_handle);
	if (m_buffer_textures)     delete[] m_buffer_textures;
}


void FrameBuffer::setViewportSize(unsigned int width, unsigned int height)
{
	if (!m_handle) return;
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer_handle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, width);

	for (unsigned int i = 0; i < m_color_buffers_count; i++)
		if (m_buffer_textures) m_buffer_textures[i].resize(mth::Vec2(width, height));
}


void FrameBuffer::clear(const Color& color)
{
	bind();
	glClearColor(color.r/float(COLOR_MAX_VALUE), color.g/float(COLOR_MAX_VALUE), color.b/float(COLOR_MAX_VALUE), color.a/float(COLOR_MAX_VALUE));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


Texture* FrameBuffer::getTexture(unsigned int index)
{
	if (index < m_color_buffers_count)
		return &m_buffer_textures[index];
	return nullptr;
}


void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
}

} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3
