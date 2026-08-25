#include <Engine/Graphics/Texture.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/TexturePool.hpp>
#include <Engine/Graphics/GL/TextureImpl.hpp>
#include <Engine/Graphics/GL/TextureHandle.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>
#include "GL/OpenGL33/TextureImpl.hpp"

#include <Engine/Core/Resource.hpp>
#include <Engine/Math/Vec2.hpp>

#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace eng::gfx
{


Texture::Texture():
	m_texture_pool(*gl::Api::getInstance()->getTexturePool())
{
	m_texture_handle = m_texture_pool.generateTexture();
	m_last_error = Texture::Error::NO_ERROR;
}

Texture::~Texture()
{
	m_texture_pool.releaseTexture(m_texture_handle);
}


void Texture::create(gl::PixelFormat pixel_format)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) tex->create(pixel_format);
}

void Texture::remove()
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) tex->remove();
}


bool Texture::setSmooth(bool flag)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->setSmooth(flag);
	return false;
}

void Texture::setFlip(bool flip_x, bool flip_y)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) tex->setFlip(flip_x, flip_y);
}


bool Texture::loadFromFile(std::initializer_list<std::string> paths)
{
	return loadFromFile(*paths.begin());
}

bool Texture::loadFromFile(const std::string& path)
{
	int width, height, channels;
	unsigned char* image_data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!image_data)
	{
		m_last_error = Texture::Error::FILE_NOT_FOUND;
		return false;
	}

	static constexpr gl::PixelFormat formats[] = {
		gl::PixelFormat::RED,
		gl::PixelFormat::RED,
		gl::PixelFormat::RED,
		gl::PixelFormat::RGB,
		gl::PixelFormat::RGBA
	};
	gl::PixelFormat format = formats[channels];

	create(format);
	bool result = loadFromBuffer(image_data, width, height);

	stbi_image_free(image_data);
	return result;
}

bool Texture::loadFromBuffer(void* image_data, unsigned int width, unsigned int height)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->loadFromBuffer(image_data, width, height);
	return false;
}

bool Texture::loadSubTexture(void* subimage_data, const mth::Vec2& position, unsigned int width, unsigned int height)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->loadSubTexture(subimage_data, position.x, position.y, width, height);
	return false;
}


bool Texture::resize(const mth::Vec2& new_size)
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->resize(new_size.x, new_size.y);
	return false;
}


bool Texture::isTransparent() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->isTransparent();
	return false;
}

bool Texture::getFlipX() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->getFlipX();
	return false;
}

bool Texture::getFlipY() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->getFlipY();
	return false;
}

mth::Vec2 Texture::getSize() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return mth::Vec2(tex->getWidth(), tex->getHeight());
	return mth::Vec2();
}

gl::PixelFormat Texture::getPixelFormat() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) return tex->getPixelFormat();
	return gl::PixelFormat::RED;
}

unsigned int gfx::Texture::getNativeHandle() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex && gl::Api::getInstance()->getType() == gl::Api::Type::OPENGL_3_3) return static_cast<gl::OpenGL33::TextureImpl*>(tex.get())->getNativeHandle();
	return 0;
}

int Texture::getLastError()
{
	return m_last_error;
}


void Texture::bind() const
{
	auto tex = m_texture_pool.getTexture(m_texture_handle);
	if (tex) tex->bind();
}

} //namespace eng::gfx