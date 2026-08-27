#include "FrameBuffer.hpp"

#include <Engine/Graphics/GL/FrameBuffer.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include <Engine/Graphics/Color.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng::gfx::gl::Null
{

void FrameBuffer::create(unsigned int buffers_count, gl::PixelFormat* formats) {m_texture.create(PixelFormat::RED);}
void FrameBuffer::remove() {}

void FrameBuffer::setViewportSize(unsigned int width, unsigned int height) {}

void FrameBuffer::clear(const Color& color) {}

Texture* FrameBuffer::getTexture(unsigned int index) {return &m_texture;}

void FrameBuffer::bind() {}

} //namespace eng::gfx::gl::OpenGL33
