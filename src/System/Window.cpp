#include <Engine/System/Window.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>
#include <iostream>
#include <GLFW/glfw3.h>


namespace eng
{

sys::Window::Window() : Window(800, 600, "Window"){}

sys::Window::Window(int width, int height, const std::string& title):
	m_size(width, height),
	gfx::RenderTarget()
{
	glfwInit();

	glfwWindowHint(GLFW_POSITION_X, 0);
	glfwWindowHint(GLFW_POSITION_Y, 30);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	window_ptr = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window_ptr == nullptr)
	{
		core::Logger::error("Failed to create window");
		destroy();
		return;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSwapInterval(0);

	setViewport(0, 0, width, height);
	setViewportCentering(0.5);
	setViewportScaling(ViewportScaling::FIXED);
	updateViewport();
}

sys::Window::~Window()
{
	destroy();
}


void sys::Window::setTitle(const std::string& new_title)
{
	glfwSetWindowTitle(window_ptr, new_title.c_str());
}


GLFWwindow* sys::Window::getHandler()
{
	return window_ptr;
}


bool sys::Window::isOpen()
{
	return !glfwWindowShouldClose(window_ptr);
}

void sys::Window::close()
{
	glfwSetWindowShouldClose(window_ptr, true);
}


void sys::Window::resize(const mth::Vec2& new_size)
{
	m_size = new_size;
	glfwSetWindowSize(window_ptr, new_size.x, new_size.y);
	updateViewport();
}

void sys::Window::setViewportCentering(const mth::Vec2& ratio)
{
	m_centering_ratio = ratio;
}

void sys::Window::setViewportScaling(Window::ViewportScaling mode)
{
	m_scaling_mode = mode;
}

void sys::Window::updateViewport()
{
	switch (m_scaling_mode)
	{
	case ViewportScaling::FIXED:
		{
			int framebuffer_width, framebuffer_height;
			glfwGetFramebufferSize(window_ptr, &framebuffer_width, &framebuffer_height);
			m_viewport_size = mth::Vec2(framebuffer_width, framebuffer_height);
			break;
		}
	case ViewportScaling::STRETCH:
		{
			m_viewport_size = m_size;
			break;
		}
	}

	mth::Vec2 pocket = m_size - m_viewport_size;
	m_viewport_pos.x = std::clamp<float>(m_centering_ratio.x, 0, 1)*pocket.x;
	m_viewport_pos.y = std::clamp<float>(m_centering_ratio.y, 0, 1)*pocket.y;
	setViewport(m_viewport_pos.x, m_viewport_pos.y, m_viewport_size.x, m_viewport_size.y);
}


void sys::Window::display()
{
	glfwSwapBuffers(window_ptr);
}

void sys::Window::destroy()
{
	glfwTerminate();
}

} //namespace eng