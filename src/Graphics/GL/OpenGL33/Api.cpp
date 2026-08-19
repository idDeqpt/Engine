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

namespace eng::gfx::gl::OpenGL33
{

void Api::init()
{
	gladLoadGL(glfwGetProcAddress);
}

} //namespace eng::gfx::gl::OpenGL33

#endif