#include <Graphics/Texture.hpp>

#include <Math/Vec2.hpp>

#include <glad/glad.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


static constexpr GLenum OPENGL_TEXTURE_FORMATS[][3] = { //{internal_format, format, type}
	{GL_RED,   GL_RED,   GL_UNSIGNED_BYTE},
	{GL_BLUE,  GL_BLUE,  GL_UNSIGNED_BYTE},
	{GL_GREEN, GL_GREEN, GL_UNSIGNED_BYTE},
	{GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE},

	{GL_RGB,    GL_RGB, GL_UNSIGNED_BYTE},
	{GL_RGB32F, GL_RGB, GL_FLOAT},

	{GL_RGBA,    GL_RGBA, GL_UNSIGNED_BYTE},
	{GL_RGBA32F, GL_RGBA, GL_FLOAT},
};
static constexpr unsigned int OPENGL_TEXTURE_TYPE_SIZES[] = {
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(unsigned char),
	sizeof(unsigned char),
	
	sizeof(unsigned char),
	sizeof(float),
	
	sizeof(unsigned char),
	sizeof(float)
};


unsigned int ChannelEnumToChannelCount(gfx::Texture::PixelFormat channel)
{
  switch(channel)
  {
	case gfx::Texture::PixelFormat::RED:
	case gfx::Texture::PixelFormat::BLUE:
	case gfx::Texture::PixelFormat::GREEN:
	case gfx::Texture::PixelFormat::ALPHA:
		return 1; break;

	case gfx::Texture::PixelFormat::RGB:
	case gfx::Texture::PixelFormat::RGB32F:
		return 3; break;

	case gfx::Texture::PixelFormat::RGBA:
	case gfx::Texture::PixelFormat::RGBA32F:
		return 4; break;

	default: return 0; break;
  }
}


gfx::Texture::Texture():
	m_native_handle(0),
	m_width(0),
	m_height(0),
	m_format(PixelFormat::RED),
	m_pixels(nullptr)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

gfx::Texture::~Texture()
{
	remove();
}


void gfx::Texture::create()
{
	remove();

	glGenTextures(1, &m_native_handle);
	m_width  = 0;
	m_height = 0;
	m_format = PixelFormat::RED;
}

void gfx::Texture::remove()
{
	if (m_native_handle)      glDeleteTextures(1, &m_native_handle);
	if (m_pixels != nullptr) {delete[] m_pixels; m_pixels = nullptr;}
}

bool gfx::Texture::loadFromFile(std::string path)
{
	create();

	int width, height, channels;
	unsigned char *image_data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	static constexpr PixelFormat formats[] = {
		PixelFormat::RED,
		PixelFormat::RED,
		PixelFormat::RED,
		PixelFormat::RGB,
		PixelFormat::RGBA
	};
	PixelFormat format = formats[channels];

	bool result = loadFromBuffer(image_data, width, height, format);

	stbi_image_free(image_data);
	return result;
}

bool gfx::Texture::loadFromBuffer(void* image_data, unsigned int width, unsigned int height, PixelFormat format)
{
	if ((image_data == nullptr) || !m_native_handle)
		return false;

	const int format_index = int(format);
	const GLenum pixel_format[3] = {
		OPENGL_TEXTURE_FORMATS[format_index][0],
		OPENGL_TEXTURE_FORMATS[format_index][1],
		OPENGL_TEXTURE_FORMATS[format_index][2]
	};
	const unsigned int type_size = OPENGL_TEXTURE_TYPE_SIZES[format_index];

	glBindTexture(GL_TEXTURE_2D, m_native_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, pixel_format[0], width, height, 0, pixel_format[1], pixel_format[2], image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int data_size     = width*height*ChannelEnumToChannelCount(format)*type_size;
	unsigned int old_data_size = m_width*m_height*ChannelEnumToChannelCount(m_format)*OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];

	if (data_size > old_data_size)
	{
		if (m_pixels != nullptr) delete[] m_pixels;
		m_pixels = new unsigned char[data_size];
	}
	memcpy(m_pixels, image_data, data_size);

	m_width  = width;
	m_height = height;
	m_format = format;
	return true;
}

bool gfx::Texture::loadSubTexture(void* subimage_data, const mth::Vec2& position, unsigned int width, unsigned int height)
{
	if (!m_native_handle ||
		(subimage_data == nullptr) ||
		(position.x < 0) ||
		(position.y < 0) ||
		((position.x + width) > m_width) ||
		((position.y + height) > m_height)) return false;

	const unsigned int type_size    = OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];
	unsigned int pixel_size         = ChannelEnumToChannelCount(m_format)*type_size;
	unsigned int pixel_row_size     = width*pixel_size;
	unsigned int old_pixel_row_size = m_width*pixel_size;

	unsigned char* subimage_bytes = static_cast<unsigned char*>(subimage_data);
	for (unsigned int y = 0; y < height; y++)
		memcpy(m_pixels + int((y + position.y)*old_pixel_row_size) + int(position.x*pixel_size), subimage_bytes + int(y*pixel_row_size), pixel_row_size);

	return loadFromBuffer(m_pixels, m_width, m_height, m_format);
}


bool gfx::Texture::resize(const mth::Vec2& new_size)
{
	if ((new_size.x <= 0) || (new_size.y <= 0)) return false;

	const unsigned int type_size     = OPENGL_TEXTURE_TYPE_SIZES[int(m_format)];
	unsigned int pixel_bytes         = ChannelEnumToChannelCount(m_format)*type_size;
	unsigned int pixel_row_bytes     = new_size.x*pixel_bytes;
	unsigned int old_pixel_row_bytes = m_width*pixel_bytes;
	unsigned int pixels_bytes        = pixel_row_bytes*new_size.y;

	unsigned char* new_pixels = new unsigned char[pixels_bytes];
	if ((new_size.x > m_width) || (new_size.y > m_height))
		memset(new_pixels, 0, pixels_bytes);

	unsigned int min_row_bytes  = (pixel_row_bytes < old_pixel_row_bytes) ? pixel_row_bytes : old_pixel_row_bytes;
	unsigned int min_rows_count = (new_size.y      < m_height)            ? new_size.y      : m_height;

	for (unsigned int y = 0; y < min_rows_count; y++)
		memcpy(new_pixels + int(y*pixel_row_bytes), m_pixels + int(y*old_pixel_row_bytes), min_row_bytes);

	bool result = loadFromBuffer(new_pixels, new_size.x, new_size.y, m_format);
	
	delete[] new_pixels;
	return result;
}


mth::Vec2 gfx::Texture::getSize()
{
	return mth::Vec2(m_width, m_height);
}

gfx::Texture::PixelFormat gfx::Texture::getPixelFormat()
{
	return m_format;
}



void gfx::Texture::bind(Texture* texture_ptr)
{
	if (texture_ptr) glBindTexture(GL_TEXTURE_2D, texture_ptr->m_native_handle);
	else             glBindTexture(GL_TEXTURE_2D, 0);
}