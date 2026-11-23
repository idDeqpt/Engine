#ifdef USE_OPENGL_3_3

#include "Shader.hpp"

#include <Engine/Math/Vec.hpp>
#include <Engine/Math/Mat.hpp>

#include <string>

#include <glad/gl.h>


namespace eng::gfx::gl::OpenGL33
{

Shader::Error Shader::loadFromBuffer(const char* vertex_buffer, const char* fragment_buffer)
{
	if (m_shader_program_id)
	{
		glDeleteProgram(m_shader_program_id);
		m_shader_program_id = 0;
	}
	
	constexpr unsigned int LOG_BUFFER_SIZE = 512;
	std::string log;
	GLint success;

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_buffer, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei log_length = 0;
		log.resize(LOG_BUFFER_SIZE);
		glGetShaderInfoLog(vertex_shader, LOG_BUFFER_SIZE, &log_length, log.data());
		log.resize(log_length);
		glDeleteShader(vertex_shader);
		return {1, std::move(log)};
	}

	unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_buffer, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLsizei log_length = 0;
		log.resize(LOG_BUFFER_SIZE);
		glGetShaderInfoLog(fragment_shader, LOG_BUFFER_SIZE, &log_length, log.data());
		log.resize(log_length);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
		return {2, std::move(log)};
		
	}

	m_shader_program_id = glCreateProgram();
	glAttachShader(m_shader_program_id, vertex_shader);
	glAttachShader(m_shader_program_id, fragment_shader);
	glLinkProgram(m_shader_program_id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLsizei log_length = 0;
		log.resize(LOG_BUFFER_SIZE);
		glGetProgramInfoLog(m_shader_program_id, LOG_BUFFER_SIZE, &log_length, log.data());
		log.resize(log_length);
		glDeleteProgram(m_shader_program_id);
		m_shader_program_id = 0;
		return {3, std::move(log)};
	}

	return {0, std::move(log)};
}


bool Shader::setUniform(const std::string& name, bool value)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1i(location, value);
	return true;
}

bool Shader::setUniform(const std::string& name, int value)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1i(location, value);
	return true;
}

bool Shader::setUniform(const std::string& name, unsigned int value)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1ui(location, value);
	return true;
}

bool Shader::setUniform(const std::string& name, float value)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1f(location, value);
	return true;
}

bool Shader::setUniform(const std::string& name, const mth::Vec3& vec)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform3fv(location, 1, &vec.x);
	return true;
}

bool Shader::setUniform(const std::string& name, const mth::Mat3& mat)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniformMatrix3fv(location, 1, GL_TRUE, mat.values);
	return true;
}

bool Shader::setUniform(const std::string& name, const mth::Mat4& mat)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniformMatrix4fv(location, 1, GL_TRUE, mat.values);
	return true;
}

bool Shader::setUniform3fv(const std::string& name, const float* values_ptr)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform3fv(location, 1, values_ptr);
	return true;
}

bool Shader::setUniform4fv(const std::string& name, const float* values_ptr)
{
	int location = glGetUniformLocation(m_shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform4fv(location, 1, values_ptr);
	return true;
}


void Shader::use()
{
	return glUseProgram(m_shader_program_id);
}

} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3
