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


bool gfx::TextureManager::bind(TextureId uid)
{
	if (uid == 0) {glBindTexture(GL_TEXTURE_2D, 0); return true;}

	for (unsigned int i = 0; i < m_textures.size(); i++)
		if (uid == m_textures[i])
		{
			glBindTexture(GL_TEXTURE_2D, uid);
			return true;
		}
	return false;
}


gfx::TextureId gfx::TextureManager::loadFromFile(std::string path)
{
    TextureData data;
	unsigned char *image_data = stbi_load(path.c_str(), &(data.width), &(data.height), &(data.channels), 0);
	TextureId id = loadFromBuffer(image_data, data);
	stbi_image_free(image_data);
	return id;
}

gfx::TextureId gfx::TextureManager::loadFromBuffer(unsigned char* image_data, TextureData data)
{
	if (image_data == nullptr)
    	return 0;

    static constexpr int modes[] = {0, 0, 0, GL_RGB, GL_RGBA};
    const GLenum mode = modes[data.channels];
    TextureId uid;

	glGenTextures(1, &uid);
	glBindTexture(GL_TEXTURE_2D, uid);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, data.width, data.height, 0, mode, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    m_textures.push_back(uid);
    m_textures_data.push_back(data);
	return uid;
}

gfx::TextureId gfx::TextureManager::loadFromBuffer(float* image_data, TextureData data)
{
	if (image_data == nullptr)
    	return 0;

    static constexpr int modes[] = {0, 0, 0, GL_RGB32F, GL_RGBA32F};
    const GLenum mode = modes[data.channels];
    TextureId uid;

	glGenTextures(1, &uid);
	glBindTexture(GL_TEXTURE_2D, uid);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, data.width, data.height, 0, GL_RGBA, GL_FLOAT, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

    m_textures.push_back(uid);
    m_textures_data.push_back(data);
	return uid;
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