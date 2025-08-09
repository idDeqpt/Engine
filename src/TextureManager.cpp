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

	static constexpr GLenum formats[] = {
		GL_RED,  GL_BLUE, GL_GREEN, GL_ALPHA,
		GL_RGB,  GL_RGB,
		GL_RGBA, GL_RGBA
	};
	static constexpr GLenum internal_formats[] = {
		GL_RED,  GL_BLUE,  GL_GREEN, GL_ALPHA,
		GL_RGB,  GL_RGB32F,
		GL_RGBA, GL_RGBA32F
	};
	static constexpr GLenum types[] = {
		GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE, GL_FLOAT,
		GL_UNSIGNED_BYTE, GL_FLOAT
	};
	const GLenum format          = formats[int(data.format)];
	const GLenum internal_format = internal_formats[int(data.format)];
	const GLenum type            = types[int(data.format)];

	for (unsigned int i = 0; i < m_textures.size(); i++)
		if (m_textures[i] == id)
		{
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, data.width, data.height, 0, format, type, image_data);
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