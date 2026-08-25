#include "TextureImpl.hpp"

#include <Engine/Graphics/GL/TextureImpl.hpp>
#include <Engine/Graphics/GL/PixelFormat.hpp>

#include <glad/glad.h>
#include <cstring>


namespace
{

static constexpr GLenum OPENGL_TEXTURE_FORMATS[][3] = { //{internal_format, format, type}
	{GL_RED,   GL_RED,   GL_UNSIGNED_BYTE},
	{GL_BLUE,  GL_BLUE,  GL_UNSIGNED_BYTE},
	{GL_GREEN, GL_GREEN, GL_UNSIGNED_BYTE},
	{GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE},

	{GL_RGB,    GL_RGB, GL_UNSIGNED_BYTE},
	{GL_SRGB,   GL_RGB, GL_UNSIGNED_BYTE},
	{GL_RGB32F, GL_RGB, GL_FLOAT},

	{GL_RGBA,       GL_RGBA, GL_UNSIGNED_BYTE},
	{GL_SRGB_ALPHA, GL_RGBA, GL_UNSIGNED_BYTE},
	{GL_RGBA32F,    GL_RGBA, GL_FLOAT},
};
static constexpr unsigned int OPENGL_TEXTURE_TYPE_SIZES[] = {
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(unsigned char),
	
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(float),
	
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(float)
};


unsigned int ChannelEnumToChannelCount(eng::gfx::gl::PixelFormat channel)
{
	switch(channel)
	{
	case eng::gfx::gl::PixelFormat::RED:
	case eng::gfx::gl::PixelFormat::BLUE:
	case eng::gfx::gl::PixelFormat::GREEN:
	case eng::gfx::gl::PixelFormat::ALPHA:
		return 1; break;

	case eng::gfx::gl::PixelFormat::RGB:
	case eng::gfx::gl::PixelFormat::RGB32F:
		return 3; break;

	case eng::gfx::gl::PixelFormat::RGBA:
	case eng::gfx::gl::PixelFormat::RGBA32F:
		return 4; break;

	default: return 0; break;
	}
}

} //namespace

namespace eng::gfx::gl::OpenGL33
{


TextureImpl::TextureImpl():
	m_handle(0),
	gl::TextureImpl()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

TextureImpl::~TextureImpl()
{
	remove();
}


void TextureImpl::create(gl::PixelFormat pixel_format)
{
	remove();

	glGenTextures(1, &m_handle);
	m_width  = 0;
	m_height = 0;
	m_format = pixel_format;
}

void TextureImpl::remove()
{
	if (m_handle)      glDeleteTextures(1, &m_handle);
	if (m_pixels != nullptr) {delete[] m_pixels; m_pixels = nullptr;}
}


bool TextureImpl::setSmooth(bool flag)
{
	if (!m_handle) return false;

	glBindTexture(GL_TEXTURE_2D, m_handle);
	if (flag)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void TextureImpl::setFlip(bool flip_x, bool flip_y)
{
	m_flip_x = flip_x;
	m_flip_y = flip_y;
}


bool TextureImpl::loadFromBuffer(void* image_data, unsigned int width, unsigned int height)
{
	if ((image_data == nullptr) || !m_handle)
		return false;

	const int format_index = int(m_format);
	const GLenum pixel_format[3] = {
		OPENGL_TEXTURE_FORMATS[format_index][0],
		OPENGL_TEXTURE_FORMATS[format_index][1],
		OPENGL_TEXTURE_FORMATS[format_index][2]
	};
	const unsigned int type_size = OPENGL_TEXTURE_TYPE_SIZES[format_index];

	glBindTexture(GL_TEXTURE_2D, m_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, pixel_format[0], width, height, 0, pixel_format[1], pixel_format[2], image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int data_size     = width*height*ChannelEnumToChannelCount(m_format)*type_size;
	unsigned int old_data_size = m_width*m_height*ChannelEnumToChannelCount(m_format)*OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];

	if (data_size > old_data_size)
	{
		if (m_pixels != nullptr) delete[] m_pixels;
		m_pixels = new unsigned char[data_size];
	}
	memcpy(m_pixels, image_data, data_size);

	m_width  = width;
	m_height = height;
	return true;
}

bool TextureImpl::loadSubTexture(void* subimage_data, int x, int y, unsigned int width, unsigned int height)
{
	if (!m_handle ||
		(subimage_data == nullptr) ||
		(x < 0) ||
		(y < 0) ||
		((x + width) > m_width) ||
		((y + height) > m_height)) return false;

	const unsigned int type_size    = OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];
	unsigned int pixel_size         = ChannelEnumToChannelCount(m_format)*type_size;
	unsigned int pixel_row_size     = width*pixel_size;
	unsigned int old_pixel_row_size = m_width*pixel_size;

	unsigned char* subimage_bytes = static_cast<unsigned char*>(subimage_data);
	for (unsigned int py = 0; py < height; py++)
		memcpy(m_pixels + int((y + py)*old_pixel_row_size) + int(x*pixel_size), subimage_bytes + int(py*pixel_row_size), pixel_row_size);

	return loadFromBuffer(m_pixels, m_width, m_height);
}


bool TextureImpl::resize(unsigned int width, unsigned int height)
{
	const unsigned int type_size     = OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];
	unsigned int pixel_bytes         = ChannelEnumToChannelCount(m_format)*type_size;
	unsigned int pixel_row_bytes     = width*pixel_bytes;
	unsigned int old_pixel_row_bytes = m_width*pixel_bytes;
	unsigned int pixels_bytes        = pixel_row_bytes*height;

	unsigned char* new_pixels = new unsigned char[pixels_bytes];
	if ((width > m_width) || (height > m_height))
		memset(new_pixels, 0, pixels_bytes);

	unsigned int min_row_bytes  = (pixel_row_bytes < old_pixel_row_bytes) ? pixel_row_bytes : old_pixel_row_bytes;
	unsigned int min_rows_count = (height          < m_height)            ? height          : m_height;

	for (unsigned int y = 0; y < min_rows_count; y++)
		memcpy(new_pixels + int(y*pixel_row_bytes), m_pixels + int(y*old_pixel_row_bytes), min_row_bytes);

	bool result = loadFromBuffer(new_pixels, width, height);
	
	delete[] new_pixels;
	return result;
}


bool TextureImpl::isTransparent()
{
	return ChannelEnumToChannelCount(m_format) > 3;
}

bool TextureImpl::getFlipX()
{
	return m_flip_x;
}

bool TextureImpl::getFlipY()
{
	return m_flip_y;
}

unsigned int TextureImpl::getWidth()
{
	return m_width;
}

unsigned int TextureImpl::getHeight()
{
	return m_height;
}

gl::PixelFormat TextureImpl::getPixelFormat()
{
	return m_format;
}

unsigned int TextureImpl::getNativeHandle()
{
	return m_handle;
}


void TextureImpl::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

} //namespace eng::gfx::gl::OpenGL33