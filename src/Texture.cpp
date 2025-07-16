#include <Graphics/Texture.hpp>

#include <glad/glad.h>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


gfx::Texture::Texture()
{
	last_error = Texture::Error::UNINITED;
}

gfx::Texture::Texture(std::string path)
{
	loadFromFile(path);
}


bool gfx::Texture::loadFromFile(std::string path)
{
	unsigned char *image_data = stbi_load(path.c_str(), &width, &height, 0, 0);

    if (image_data == nullptr)
    {
    	last_error = Texture::Error::LOAD_FAILED;
    	return false;
    }

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_data);
	glBindTexture(GL_TEXTURE_2D, 0);

	last_error = Texture::Error::NO_ERROR;
	return true;
}


GLuint gfx::Texture::getId()
{
	return id;
}


gfx::Texture::Error gfx::Texture::getLastError()
{
	return last_error;
}