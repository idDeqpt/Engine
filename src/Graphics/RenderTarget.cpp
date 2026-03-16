#include <Engine/Graphics/RenderTarget.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/RenderStates.hpp>

#include <glad/glad.h>

namespace eng
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


gfx::RenderTarget::RenderTarget():
	m_native_handle(0),
	m_depth_buffer_handle(0),
	m_color_buffers_count(0),
	m_buffer_textures(nullptr),
	m_viewport_pos(0, 0),
	m_viewport_size(800, 600) {}

gfx::RenderTarget::RenderTarget(unsigned int buffers_count, Texture::PixelFormat* formats):
	m_color_buffers_count(buffers_count),
	m_buffer_textures(nullptr),
	m_viewport_pos(0, 0),
	m_viewport_size(800, 600)
{
	glGenFramebuffers(1, &m_native_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_native_handle);

	glGenRenderbuffers(1, &m_depth_buffer_handle);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer_handle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_viewport_size.x, m_viewport_size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer_handle);

	if (m_color_buffers_count)
	{
		m_buffer_textures = new Texture[m_color_buffers_count];
		for (unsigned int i = 0; i < m_color_buffers_count; i++)
		{
			m_buffer_textures[i].create(formats[i]);
			m_buffer_textures[i].resize(m_viewport_size);
			m_buffer_textures[i].setSmooth(true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_buffer_textures[i].getNativeHandle(), 0);
		}
		glDrawBuffers(m_color_buffers_count, OPENGL_ATTACHMENTS);
	}
}

gfx::RenderTarget::~RenderTarget()
{
	if (m_native_handle)		glDeleteFramebuffers(1, &m_native_handle);
	if (m_depth_buffer_handle)  glDeleteRenderbuffers(1, &m_depth_buffer_handle);
	if (m_buffer_textures)		delete[] m_buffer_textures;
}


void gfx::RenderTarget::setViewport(int x, int y, int width, int height)
{
	m_viewport_pos  = mth::Vec2(x, y);
	m_viewport_size = mth::Vec2(width, height);
}


void gfx::RenderTarget::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_native_handle);
}

void gfx::RenderTarget::clear(const gfx::Color& color)
{
	bind();
	glClearColor(color.r/float(COLOR_MAX_VALUE), color.g/float(COLOR_MAX_VALUE), color.b/float(COLOR_MAX_VALUE), color.a/float(COLOR_MAX_VALUE));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void gfx::RenderTarget::draw(gfx::Drawable& drawable, gfx::RenderStates& states)
{
	bind();
	glViewport(m_viewport_pos.x, m_viewport_pos.y, m_viewport_size.x, m_viewport_size.y);
	drawable.draw(this, states);
}


gfx::Texture& gfx::RenderTarget::getTexture(unsigned int index)
{
	if (index < m_color_buffers_count)
		return m_buffer_textures[index];
	return m_buffer_textures[0];
}

} //namespace eng