#include <Graphics/TextureManager.hpp>

#include <Math/Vec2.hpp>

#include <glad/glad.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


std::vector<gfx::TextureId> gfx::TextureManager::m_textures;
std::vector<gfx::TextureManager::TextureData> gfx::TextureManager::m_textures_data;
std::vector<unsigned char*> gfx::TextureManager::m_textures_pixels;


unsigned int ChannelEnumToChannelCount(gfx::TextureManager::TextureData::Channel channel)
{
  switch(channel)
  {
	case gfx::TextureManager::TextureData::Channel::RED:
	case gfx::TextureManager::TextureData::Channel::BLUE:
	case gfx::TextureManager::TextureData::Channel::GREEN:
	case gfx::TextureManager::TextureData::Channel::ALPHA:
	  return 1; break;

	case gfx::TextureManager::TextureData::Channel::RGB:
	case gfx::TextureManager::TextureData::Channel::RGB32F:
	  return 3; break;

	case gfx::TextureManager::TextureData::Channel::RGBA:
	case gfx::TextureManager::TextureData::Channel::RGBA32F:
	  return 4; break;

	default: return 0; break;
  }
}


void gfx::TextureManager::initialize() {}

void gfx::TextureManager::finalize()
{
	glDeleteTextures(m_textures.size(), m_textures.data());
	for (unsigned int i = 0; i < m_textures_pixels.size(); i++)
		if (m_textures_pixels[i] != nullptr) delete[] m_textures_pixels[i];
}


bool gfx::TextureManager::bind(TextureId id)
{
	if (id == 0) {glBindTexture(GL_TEXTURE_2D, 0); return true;}

	for (unsigned int i = 0; i < m_textures.size(); i++)
		if (id == m_textures[i])
		{
			glBindTexture(GL_TEXTURE_2D, id);
			return true;
		}
	return false;
}


gfx::TextureId gfx::TextureManager::create()
{
	TextureId id;
	glGenTextures(1, &id);
	m_textures.push_back(id);
	m_textures_data.emplace_back();
	m_textures_pixels.push_back(nullptr);
	return id;
}

gfx::TextureId gfx::TextureManager::loadFromFile(std::string path)
{
	TextureId id = create();
	TextureData data;
	int channels;
	unsigned char *image_data = stbi_load(path.c_str(), &(data.width), &(data.height), &channels, 0);

	static constexpr TextureData::Channel formats[] = {
		TextureData::Channel::RED,
		TextureData::Channel::RED,
		TextureData::Channel::RED,
		TextureData::Channel::RGB,
		TextureData::Channel::RGBA
	};
	data.format = formats[channels];

	loadFromBuffer(id, image_data, data);

	stbi_image_free(image_data);
	return id;
}

bool gfx::TextureManager::loadFromBuffer(TextureId id, void* image_data, TextureData data)
{
	if (image_data == nullptr)
		return false;

	static constexpr GLenum formats[][3] = { //{internal_format, format, type}
		{GL_RED,   GL_RED,   GL_UNSIGNED_BYTE},
		{GL_BLUE,  GL_BLUE,  GL_UNSIGNED_BYTE},
		{GL_GREEN, GL_GREEN, GL_UNSIGNED_BYTE},
		{GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE},

		{GL_RGB,    GL_RGB, GL_UNSIGNED_BYTE},
		{GL_RGB32F, GL_RGB, GL_FLOAT},

		{GL_RGBA,    GL_RGBA, GL_UNSIGNED_BYTE},
		{GL_RGBA32F, GL_RGBA, GL_FLOAT},
	};
	static constexpr unsigned int type_sizes[] = {
		sizeof(unsigned char),
		sizeof(unsigned char),
		sizeof(unsigned char),
		sizeof(unsigned char),
		
		sizeof(unsigned char),
		sizeof(float),
		
		sizeof(unsigned char),
		sizeof(float)
	};
	const int format_index = int(data.format);
	const GLenum format[3] = {formats[format_index][0], formats[format_index][1], formats[format_index][2]};
	const unsigned int type_size = type_sizes[format_index];

	for (int i = m_textures.size() - 1; i >= 0; i--)
		if (m_textures[i] == id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format[0], data.width, data.height, 0, format[1], format[2], image_data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			unsigned int data_size = data.width*data.height*ChannelEnumToChannelCount(data.format)*type_size;
			unsigned int old_data_size = m_textures_data[i].width*m_textures_data[i].height*ChannelEnumToChannelCount(m_textures_data[i].format)*type_sizes[int(m_textures_data[i].format)];

			if (data_size > old_data_size)
			{
				if (m_textures_pixels[i] != nullptr) delete[] m_textures_pixels[i];
				m_textures_pixels[i] = new unsigned char[data_size];
			}
			memcpy(m_textures_pixels[i], image_data, data_size);

			m_textures_data[i] = data;
			return true;
		}
	return false;
}


void gfx::TextureManager::deleteTexture(TextureId id)
{
	for (unsigned int i = 0; i < m_textures.size(); i++)
		if (m_textures[i] == id)
		{
			if (m_textures_pixels[i] != nullptr) delete[] m_textures_pixels[i];
			m_textures.erase(m_textures.begin() + i);
			m_textures_data.erase(m_textures_data.begin() + i);
			m_textures_pixels.erase(m_textures_pixels.begin() + i);
			glDeleteTextures(1, (GLuint*)&id);
		}
}




gfx::TextureManager::TextureData::TextureData():
	width(0),
	height(0),
	format(Channel::RED) {}

gfx::TextureManager::TextureData::TextureData(int width, int height, Channel format):
	width(width),
	height(height),
	format(format) {}