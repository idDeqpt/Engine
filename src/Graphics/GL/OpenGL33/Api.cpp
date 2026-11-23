#include <chrono>
#include <iostream>
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
	return gl::Api::Type::OPENGL_3_3;
}

} //namespace eng::gfx::gl::OpenGL33


#ifdef USE_OPENGL_3_3

#include <memory>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "ArrayBuffer.hpp"
#include "FrameBuffer.hpp"
#include "Shader.hpp"
#include "TextureImpl.hpp"

namespace
{

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

Api::Cache Api::s_cache;

void Api::initialize()
{
	gladLoadGL(glfwGetProcAddress);

	Capability init_cap(false);
	setCapability(init_cap);
}


void Api::setCapability(const Capability& capability)
{
	#define APPLY_IF_CHANGED(c, v, m)\
	if (v != Capability::Mode::ANY)\
		if (c != v)\
		{\
			if (v == Capability::Mode::ENABLED)\
				glEnable(m);\
			else if (v == Capability::Mode::DISABLED)\
				glDisable(m);\
			c = v;\
		}
	
	APPLY_IF_CHANGED(s_cache.capability.depth_test,   capability.depth_test,                 GL_DEPTH_TEST);
	APPLY_IF_CHANGED(s_cache.capability.blend,        capability.blend,                      GL_BLEND);
	APPLY_IF_CHANGED(s_cache.capability.scissor_test, capability.scissor_test,               GL_SCISSOR_TEST);
	APPLY_IF_CHANGED(s_cache.capability.cull_face,    capability.cull_face,                  GL_CULL_FACE);
	APPLY_IF_CHANGED(s_cache.capability.polygon_offset_fill, capability.polygon_offset_fill, GL_POLYGON_OFFSET_FILL);
	APPLY_IF_CHANGED(s_cache.capability.stencil_test, capability.stencil_test,               GL_STENCIL_TEST);
	APPLY_IF_CHANGED(s_cache.capability.dither,       capability.dither,                     GL_DITHER);
	APPLY_IF_CHANGED(s_cache.capability.multisample,  capability.multisample,                GL_MULTISAMPLE);

	#undef APPLY_IF_CHANGED
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

void Api::setViewport(int x, int y, unsigned int width, unsigned int height)
{
	glViewport(x, y, width, height);
}


void Api::setActiveTexture(unsigned int texrure_i)
{
	glActiveTexture(GL_TEXTURE0 + texrure_i);
}


std::unique_ptr<gl::ArrayBuffer> Api::createArrayBuffer()
{
	return std::make_unique<OpenGL33::ArrayBuffer>();
}

std::unique_ptr<gl::FrameBuffer> Api::createFrameBuffer()
{
	return std::make_unique<OpenGL33::FrameBuffer>();
}

std::unique_ptr<gl::Shader> Api::createShader()
{
	return std::make_unique<OpenGL33::Shader>();
}


std::unique_ptr<gl::TextureImpl> Api::createTextureImpl()
{
	return std::make_unique<OpenGL33::TextureImpl>();
}


} //namespace eng::gfx::gl::OpenGL33

#endif //USE_OPENGL_3_3