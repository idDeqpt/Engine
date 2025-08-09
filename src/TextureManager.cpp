#include <Graphics/TextureManager.hpp>

#include <Math/Vec2.hpp>

#include <glad/glad.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


std::vector<gfx::TextureId> gfx::TextureManager::m_textures;
std::vector<gfx::TextureManager::TextureData> gfx::TextureManager::m_textures_data;


void gfx::TextureManager::initialize() {}

void gfx::TextureManager::finalize()
{
	glDeleteTextures(m_textures.size(), m_textures.data());
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

	for (unsigned int i = 0; i < m_textures.size(); i++)
		if (m_textures[i] == id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, format[0], data.width, data.height, 0, format[1], format[2], image_data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

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
			m_textures.erase(m_textures.begin() + i);
			m_textures_data.erase(m_textures_data.begin() + i);
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