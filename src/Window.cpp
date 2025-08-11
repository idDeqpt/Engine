#include <Graphics/Window.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Graphics/TextureManager.hpp>
#include <Graphics/Shader.hpp>
#include <Graphics/View.hpp>
#include <Math/Transform3.hpp>


gfx::Window::Window() : Window(800, 600, "Window"){}

gfx::Window::Window(int width, int height, std::string title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	window_ptr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ptr == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		destroy();
		return;
	}
	glfwMakeContextCurrent(window_ptr);
	gladLoadGL();

	setViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

gfx::Window::~Window()
{
	destroy();
}


GLFWwindow* gfx::Window::getHandler()
{
	return window_ptr;
}


void gfx::Window::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

bool gfx::Window::isOpen()
{
	return !glfwWindowShouldClose(window_ptr);
}

void gfx::Window::close()
{
	glfwSetWindowShouldClose(window_ptr, true);
}


void gfx::Window::clear(const gfx::Color& color)
{
	glClearColor(color.r/float(COLOR_MAX_VALUE), color.g/float(COLOR_MAX_VALUE), color.b/float(COLOR_MAX_VALUE), color.a/float(COLOR_MAX_VALUE));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfx::Window::display()
{
	glfwSwapBuffers(window_ptr);
}

void gfx::Window::destroy()
{
	glfwTerminate();
}


void gfx::Window::draw(Drawable& drawable, RenderStates& states)
{
	drawable.draw(this, states);
}