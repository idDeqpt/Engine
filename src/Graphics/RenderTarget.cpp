#include <Engine/Graphics/RenderTarget.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/Drawable.hpp>
#include <Engine/Graphics/RenderStates.hpp>

namespace eng::gfx
{

RenderTarget::RenderTarget():
	m_frame_buffer(gl::Api::getInstance()->createFrameBuffer()),
	m_viewport_pos(0, 0),
	m_viewport_size(100, 100) {}

RenderTarget::RenderTarget(unsigned int buffers_count, gl::PixelFormat* formats):
	m_frame_buffer(gl::Api::getInstance()->createFrameBuffer()),
	m_viewport_pos(0, 0),
	m_viewport_size(100, 100)
{
	m_frame_buffer->create(buffers_count, formats);
	setViewport(0, 0, 100, 100);
}


void RenderTarget::setViewport(int x, int y, int width, int height)
{
	m_viewport_pos  = mth::Vec2(x, y);
	m_viewport_size = mth::Vec2(width, height);

	m_frame_buffer->setViewportSize(width, height);
}


void RenderTarget::clear(const Color& color)
{
	m_frame_buffer->clear(color);
}


void RenderTarget::draw(Drawable& drawable, const RenderStates& states)
{
	m_frame_buffer->bind();
	gl::Api::getInstance()->setViewport(m_viewport_pos.x, m_viewport_pos.y, m_viewport_size.x, m_viewport_size.y);
	drawable.draw(this, states);
}


Texture* RenderTarget::getTexture(unsigned int index)
{
	return m_frame_buffer->getTexture(index);
}

} //namespace eng::gfx