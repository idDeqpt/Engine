#include <Engine/Graphics/Shader.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Math/Vec3.hpp>

#include <fstream>
#include <sstream>
#include <string>


namespace eng
{

gfx::Shader* gfx::Shader::s_active = nullptr;


gfx::Shader::Shader()
{
	m_last_error = Shader::Error::UNINITED;
	m_last_error_log = "Shaders sources have not been loaded";
}

gfx::Shader::Shader(const char* vertex_buffer, const char* fragment_buffer)
{
	loadFromBuffer(vertex_buffer, fragment_buffer);
}


bool gfx::Shader::loadFromFile(std::initializer_list<std::string> paths)
{
	return loadFromFile(*paths.begin(), *(paths.begin() + 1));
}

bool gfx::Shader::loadFromFile(std::string vertex_path, std::string fragment_path)
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

bool gfx::Shader::loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer)
{
	constexpr unsigned int LOG_BUFFER_SIZE = 512;
	char log_buffer[LOG_BUFFER_SIZE];
	unsigned int vertex_shader = gfx::gl::Api::getInstance()->compileVertexShader(vertex_buffer, log_buffer, LOG_BUFFER_SIZE);
	if (vertex_shader == 0)
	{
		m_last_error = Shader::Error::VERTEX_COMPILE_FAILED;
		m_last_error_log = log_buffer;
		return false;
	}

	unsigned int fragment_shader = gfx::gl::Api::getInstance()->compileFrahmentShader(fragment_buffer, log_buffer, LOG_BUFFER_SIZE);
	if (fragment_shader == 0)
	{
		m_last_error = Shader::Error::FRAGMENT_COMPILE_FAILED;
		m_last_error_log = log_buffer;
		return false;
	}

	m_shader_program_id = gfx::gl::Api::getInstance()->linkShader(vertex_shader, fragment_shader, log_buffer, LOG_BUFFER_SIZE);
	if (m_shader_program_id == 0)
	{
		m_last_error = Shader::Error::PROGRAM_LINKING_FAILED;
		m_last_error_log = log_buffer;
		return false;
	}

	m_last_error = Shader::Error::NO_ERROR;
	return true;
}


int gfx::Shader::getLastError()
{
	return m_last_error;
}

std::string gfx::Shader::getLastErrorLog()
{
	return m_last_error_log;
}


bool gfx::Shader::setUniform1i(std::string name, int value)
{
	return gfx::gl::Api::getInstance()->setUniform1i(m_shader_program_id, name.c_str(), value);
}

bool gfx::Shader::setUniform1ui(std::string name, unsigned int value)
{
	return gfx::gl::Api::getInstance()->setUniform1ui(m_shader_program_id, name.c_str(), value);
}

bool gfx::Shader::setUniform1f(std::string name, float value)
{
	return gfx::gl::Api::getInstance()->setUniform1f(m_shader_program_id, name.c_str(), value);
}

bool gfx::Shader::setUniformVec3(std::string name, const mth::Vec3& vec)
{
	return gfx::gl::Api::getInstance()->setUniform3fv(m_shader_program_id, name.c_str(), &vec.x);
}

bool gfx::Shader::setUniform3fv(std::string name, const float* values_ptr)
{
	return gfx::gl::Api::getInstance()->setUniform3fv(m_shader_program_id, name.c_str(), values_ptr);
}

bool gfx::Shader::setUniform4fv(std::string name, const float* values_ptr)
{
	return gfx::gl::Api::getInstance()->setUniform4fv(m_shader_program_id, name.c_str(), values_ptr);
}

bool gfx::Shader::setUniformMatrix3fv(std::string name, const float* mat_ptr)
{
	return gfx::gl::Api::getInstance()->setUniformMatrix3fv(m_shader_program_id, name.c_str(), mat_ptr);
}

bool gfx::Shader::setUniformMatrix4fv(std::string name, const float* mat_ptr)
{
	return gfx::gl::Api::getInstance()->setUniformMatrix4fv(m_shader_program_id, name.c_str(), mat_ptr);
}


bool gfx::Shader::use()
{
	if (m_last_error != Shader::Error::NO_ERROR)
		return false;

	gfx::gl::Api::getInstance()->useShader(m_shader_program_id);
	return true;
}



void gfx::Shader::setActive(Shader* target)
{
	s_active = target;
}

gfx::Shader* gfx::Shader::getActive()
{
	return s_active;
}

} //namespace eng