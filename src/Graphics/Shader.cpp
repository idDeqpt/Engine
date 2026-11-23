#include <Engine/Graphics/Shader.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/Shader.hpp>
#include <Engine/Math/Vec.hpp>

#include <fstream>
#include <sstream>
#include <string>


namespace eng
{

gfx::Shader::Shader():
	m_last_error_log("Shaders sources have not been loaded"),
	m_shader(gl::Api::getInstance()->createShader())
{
	m_last_error = Shader::Error::UNINITED;
}

gfx::Shader::Shader(const char* vertex_buffer, const char* fragment_buffer):
	Shader()
{
	loadFromBuffer(vertex_buffer, fragment_buffer);
}


bool gfx::Shader::loadFromFile(std::initializer_list<std::string> paths)
{
	return loadFromFile(*paths.begin(), *(paths.begin() + 1));
}

bool gfx::Shader::loadFromFile(const std::string& vertex_path, const std::string& fragment_path)
{
	std::string vertex_source;
	std::ifstream vertex_file(vertex_path);
	if (!vertex_file)
	{
		m_last_error = Shader::Error::VERTEX_FILE_NOT_EXIST;
		m_last_error_log = "Vertex shader file does not exist in the path: " + vertex_path;
		return false;
	}
	std::stringstream vertex_buffer;
	vertex_buffer << vertex_file.rdbuf();
	vertex_file.close();

	std::string fragment_source;
	std::ifstream fragment_file(fragment_path);
	if (!fragment_file)
	{
		m_last_error = Shader::Error::FRAGMENT_FILE_NOT_EXIST;
		m_last_error_log = "Fragment shader file does not exist in the path: " + vertex_path;
		return false;
	}
	std::stringstream fragment_buffer;
	fragment_buffer << fragment_file.rdbuf();
	fragment_file.close();

	return loadFromBuffer(vertex_buffer.str().c_str(), fragment_buffer.str().c_str());
}


int gfx::Shader::getLastError()
{
	return m_last_error;
}

const std::string& gfx::Shader::getLastErrorLog()
{
	return m_last_error_log;
}


bool gfx::Shader::use()
{
	if (m_last_error != Shader::Error::NO_ERROR)
		return false;

	m_shader->use();
	return true;
}

} //namespace eng