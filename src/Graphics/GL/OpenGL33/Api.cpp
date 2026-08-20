#include "Api.hpp"

namespace eng::gfx::gl::OpenGL33
{

bool Api::isAvailable()
{
	#ifdef USE_OPENGL_3_3
	return true;
	#endif
	return false;
}

} //namespace eng::gfx::gl::OpenGL33


#ifdef USE_OPENGL_3_3

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{

GLenum capabilityToGlCapability(eng::gfx::gl::Capability capability)
{
	switch (capability)
	{
		case eng::gfx::gl::Capability::DEPTH_TEST:
			return GL_DEPTH_TEST;
		case eng::gfx::gl::Capability::BLEND:
			return GL_BLEND;
		case eng::gfx::gl::Capability::SCISSOR_TEST:
			return GL_SCISSOR_TEST;
		case eng::gfx::gl::Capability::CULL_FACE:
			return GL_CULL_FACE;
		case eng::gfx::gl::Capability::POLYGON_OFFSET_FILL:
			return GL_POLYGON_OFFSET_FILL;
		case eng::gfx::gl::Capability::STENCIL_TEST:
			return GL_STENCIL_TEST;
		case eng::gfx::gl::Capability::DITHER:
			return GL_DITHER;
		case eng::gfx::gl::Capability::MULTISAMPLE:
			return GL_MULTISAMPLE;
	}
	return GL_INVALID_ENUM;
}

} //namespace

namespace eng::gfx::gl::OpenGL33
{

void Api::init()
{
	gladLoadGL(glfwGetProcAddress);
}


void Api::enable(Capability capability)
{
	glEnable(capabilityToGlCapability(capability));
}

void Api::disable(Capability capability)
{
	glDisable(capabilityToGlCapability(capability));
}


void Api::setActiveTexture(unsigned int texrure_i)
{
	glActiveTexture(GL_TEXTURE0 + texrure_i);
}


void Api::useShader(unsigned int shader_id)
{
	glUseProgram(shader_id);
}

unsigned int Api::compileVertexShader(const char* vertex_buffer, char* log_buffer, unsigned int log_buffer_size)
{
	unsigned int shader_id = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(shader_id, 1, &vertex_buffer, NULL);
	glCompileShader(shader_id);

	GLint success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader_id, log_buffer_size, NULL, log_buffer);
		return 0;
	}
	return shader_id;
}

unsigned int Api::compileFrahmentShader(const char* fragment_buffer, char* log_buffer, unsigned int log_buffer_size)
{
	unsigned int shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(shader_id, 1, &fragment_buffer, NULL);
	glCompileShader(shader_id);

	GLint success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader_id, log_buffer_size, NULL, log_buffer);
		return 0;
	}
	return shader_id;
}

unsigned int Api::linkShader(unsigned int vertex_id, unsigned int fragment_id, char* log_buffer, unsigned int log_buffer_size)
{
	unsigned int program_id = glCreateProgram();

	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);

	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);

	GLint success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program_id, log_buffer_size, NULL, log_buffer);
		return 0;
	}
	return program_id;
}

bool Api::setUniform1i(unsigned int shader_id, const char* name, int value)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniform1i(location, value);
	return true;
}

bool Api::setUniform1ui(unsigned int shader_id, const char* name, unsigned int value)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniform1ui(location, value);
	return true;
}

bool Api::setUniform1f(unsigned int shader_id, const char* name, float value)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniform1f(location, value);
	return true;
}

bool Api::setUniform3fv(unsigned int shader_id, const char* name, const float* values_ptr)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniform3fv(location, 1, values_ptr);
	return true;
}

bool Api::setUniform4fv(unsigned int shader_id, const char* name, const float* values_ptr)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniform4fv(location, 1, values_ptr);
	return true;
}

bool Api::setUniformMatrix3fv(unsigned int shader_id, const char* name, const float* matrix_ptr)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniformMatrix3fv(location, 1, GL_TRUE, matrix_ptr);
	return true;
}

bool Api::setUniformMatrix4fv(unsigned int shader_id, const char* name, const float* matrix_ptr)
{
	int location = glGetUniformLocation(shader_id, name);
	if (location == -1)
		return false;

	glUniformMatrix4fv(location, 1, GL_TRUE, matrix_ptr);
	return true;
}


} //namespace eng::gfx::gl::OpenGL33

#endif