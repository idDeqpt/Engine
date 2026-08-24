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

gl::Api::Type Api::getType()
{
	return Api::Type::OPENGL_3_3;
}

} //namespace eng::gfx::gl::OpenGL33


#ifdef USE_OPENGL_3_3

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ArrayBuffer.hpp"

namespace
{

GLenum capabilityToGlCapability(const eng::gfx::gl::Capability& capability)
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

GLenum depthFunctionToGlDepthFunction(const eng::gfx::gl::DepthFunction& func)
{
	switch (func)
	{
		case eng::gfx::gl::DepthFunction::NEVER:
			return GL_NEVER;
		case eng::gfx::gl::DepthFunction::LESS:
			return GL_LESS;
		case eng::gfx::gl::DepthFunction::EQUAL:
			return GL_EQUAL;
		case eng::gfx::gl::DepthFunction::LEQUAL:
			return GL_LEQUAL;
		case eng::gfx::gl::DepthFunction::GREATER:
			return GL_GREATER;
		case eng::gfx::gl::DepthFunction::NOTEQUAL:
			return GL_NOTEQUAL;
		case eng::gfx::gl::DepthFunction::GEQUAL:
			return GL_GEQUAL;
		case eng::gfx::gl::DepthFunction::ALWAYS:
			return GL_ALWAYS;
	}
	return GL_INVALID_ENUM;
}

GLenum blendFactorToGlBlendFactor(const eng::gfx::gl::BlendFactor& factor)
{
	switch (factor)
	{
		case eng::gfx::gl::BlendFactor::ZERO:
			return GL_ZERO;
		case eng::gfx::gl::BlendFactor::ONE:
			return GL_ONE;
		case eng::gfx::gl::BlendFactor::SRC_COLOR:
			return GL_SRC_COLOR;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_SRC_COLOR:
			return GL_ONE_MINUS_SRC_COLOR;
		case eng::gfx::gl::BlendFactor::DST_COLOR:
			return GL_DST_COLOR;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_DST_COLOR:
			return GL_ONE_MINUS_DST_COLOR;
		case eng::gfx::gl::BlendFactor::SRC_ALPHA:
			return GL_SRC_ALPHA;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_SRC_ALPHA:
			return GL_ONE_MINUS_SRC_ALPHA;
		case eng::gfx::gl::BlendFactor::DST_ALPHA:
			return GL_DST_ALPHA;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_DST_ALPHA:
			return GL_ONE_MINUS_DST_ALPHA;
		case eng::gfx::gl::BlendFactor::CONSTANT_COLOR:
			return GL_CONSTANT_COLOR;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_CONSTANT_COLOR:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case eng::gfx::gl::BlendFactor::CONSTANT_ALPHA:
			return GL_CONSTANT_ALPHA;
		case eng::gfx::gl::BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case eng::gfx::gl::BlendFactor::SRC_ALPHA_SATURATE:
			return GL_SRC_ALPHA_SATURATE;
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


void Api::enable(const Capability& capability)
{
	glEnable(capabilityToGlCapability(capability));
}

void Api::disable(const Capability& capability)
{
	glDisable(capabilityToGlCapability(capability));
}


void Api::useDepthMask(bool flag)
{
	glDepthMask(flag);
}

void Api::setDepthFunction(const DepthFunction& function)
{
	glDepthFunc(depthFunctionToGlDepthFunction(function));
}

void Api::setBlendFunction(const BlendFactor& source, const BlendFactor& destination)
{
	glBlendFunc(blendFactorToGlBlendFactor(source), blendFactorToGlBlendFactor(destination));
}


void Api::setScissor(int x, int y, unsigned int width, unsigned int height)
{
	glScissor(x, y, width, height);
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


std::unique_ptr<gl::ArrayBuffer> Api::createArrayBuffer()
{
	return std::make_unique<OpenGL33::ArrayBuffer>();
}


} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3