#include <Graphics/Shader.hpp>

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <Math/Vec3.hpp>


gfx::Shader* gfx::Shader::active = nullptr;


gfx::Shader::Shader()
{
	last_error = Shader::Error::UNINITED;
	last_error_log = "Shaders sources have not been loaded";
}

gfx::Shader::Shader(const GLchar* vertex_buffer, const GLchar* fragment_buffer)
{
	loadFromBuffer(vertex_buffer, fragment_buffer);
}


bool gfx::Shader::loadFromBuffer(const GLchar* vertex_buffer, const GLchar* fragment_buffer)
{
	GLuint vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertex_shader, 1, &vertex_buffer, NULL);
	glCompileShader(vertex_shader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		last_error = Shader::Error::VERTEX_COMPILE_FAILED;
		last_error_log = infoLog;
		return false;
	}

	GLuint fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_buffer, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glDeleteShader(vertex_shader);
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		last_error = Shader::Error::FRAGMENT_COMPILE_FAILED;
		last_error_log = infoLog;
		return false;
	}

	shader_program_id = glCreateProgram();

	glAttachShader(shader_program_id, vertex_shader);
	glAttachShader(shader_program_id, fragment_shader);
	glLinkProgram(shader_program_id);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program_id, 512, NULL, infoLog);
		last_error = Shader::Error::PROGRAM_LINKING_FAILED;
		last_error_log = infoLog;
		return false;
	}

	last_error = Shader::Error::NO_ERROR;
	return true;
}

bool gfx::Shader::loadFromFile(std::string vertex_path, std::string fragment_path)
{
	std::string vertex_source;
	std::ifstream vertex_file(vertex_path);
	if (!vertex_file)
	{
		last_error = Shader::Error::VERTEX_FILE_NOT_EXIST;
		last_error_log = "Vertex shader file does not exist in the path: " + vertex_path;
		return false;
	}
	std::stringstream vertex_buffer;
	vertex_buffer << vertex_file.rdbuf();
	vertex_file.close();

	std::string fragment_source;
	std::ifstream fragment_file(fragment_path);
	if (!fragment_file)
	{
		last_error = Shader::Error::FRAGMENT_FILE_NOT_EXIST;
		last_error_log = "Fragment shader file does not exist in the path: " + vertex_path;
		return false;
	}
	std::stringstream fragment_buffer;
	fragment_buffer << fragment_file.rdbuf();
	fragment_file.close();

	return loadFromBuffer(vertex_buffer.str().c_str(), fragment_buffer.str().c_str());
}


gfx::Shader::Error gfx::Shader::getLastError()
{
	return last_error;
}

std::string gfx::Shader::getLastErrorLog()
{
	return last_error_log;
}


bool gfx::Shader::setUniform1i(std::string name, int value)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1i(location, value);
	return true;
}

bool gfx::Shader::setUniform1f(std::string name, float value)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform1f(glGetUniformLocation(shader_program_id, name.c_str()), value);
}

bool gfx::Shader::setUniformVec3(std::string name, const mth::Vec3& vec)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform3f(location, vec.x, vec.y, vec.z);
	return true;
}

bool gfx::Shader::setUniform3fv(std::string name, int count, float* values_ptr)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniform3fv(location, count, values_ptr);
	return true;
}

bool gfx::Shader::setUniformMatrix3fv(std::string name, float* mat_ptr)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniformMatrix3fv(location, 1, GL_TRUE, mat_ptr);
	return true;
}

bool gfx::Shader::setUniformMatrix4fv(std::string name, float* mat_ptr)
{
	int location = glGetUniformLocation(shader_program_id, name.c_str());
	if (location == -1)
		return false;

	glUniformMatrix4fv(location, 1, GL_TRUE, mat_ptr);
	return true;
}


bool gfx::Shader::use()
{
	if (last_error != Shader::Error::NO_ERROR)
		return false;

	glUseProgram(shader_program_id);
	return true;
}



void gfx::Shader::setActive(Shader* target)
{
	active = target;
}

gfx::Shader* gfx::Shader::getActive()
{
	return active;
}