#include "TextureImpl.hpp"

#include <Engine/Graphics/GL/TextureImpl.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>


namespace eng::gfx::gl::Null
{

void TextureImpl::create(gl::PixelFormat pixel_format) {}
void TextureImpl::remove() {}

bool TextureImpl::setSmooth(bool flag) {return true;}
void TextureImpl::setFlip(bool flip_x, bool flip_y) {}

bool TextureImpl::loadFromBuffer(void* image_data, unsigned int width, unsigned int height) {return true;}
bool TextureImpl::loadSubTexture(void* subimage_data, int x, int y, unsigned int width, unsigned int height) {return true;}

bool TextureImpl::resize(unsigned int width, unsigned int height) {return true;}

bool TextureImpl::isTransparent() {return true;}
bool TextureImpl::getFlipX() {return true;}
bool TextureImpl::getFlipY() {return true;}
unsigned int TextureImpl::getWidth() {return 0;}
unsigned int TextureImpl::getHeight() {return 0;}
gl::PixelFormat TextureImpl::getPixelFormat() {return gl::PixelFormat::RED;}

void TextureImpl::bind() {}

} //namespace eng::gfx::gl::OpenGL33