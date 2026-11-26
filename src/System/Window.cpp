#include <Engine/System/Window.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Graphics/GL/Capability.hpp>
#include <Engine/Math/Vec.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>
#include <GLFW/glfw3.h>


namespace eng::sys
{

void Window::initialize()
{
	glfwInit();
}

void Window::finalize()
{
	glfwTerminate();
}

Window::Window() : Window(800, 600, "Window"){}

Window::Window(int width, int height, const std::string& title):
	m_size(width, height),
	gfx::RenderTarget()
{
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
		return;
	}
	glfwMakeContextCurrent(window_ptr);
	glfwSwapInterval(0);

	setViewport(0, 0, width, height);
	setViewportSize(m_size);
	setViewportCentering(0.5);
	setViewportScaling(ViewportScaling::FIXED);
	updateViewport();
}


void Window::setTitle(const std::string& new_title)
{
	glfwSetWindowTitle(window_ptr, new_title.c_str());
}


GLFWwindow* Window::getHandler()
{
	return window_ptr;
}


bool Window::isOpen() const
{
	return !glfwWindowShouldClose(window_ptr);
}

void Window::close()
{
	glfwSetWindowShouldClose(window_ptr, true);
}


void Window::resize(mth::Vec2 new_size)
{
	m_size = new_size;
	glfwSetWindowSize(window_ptr, new_size.x, new_size.y);
	updateViewport();
}

void Window::setViewportCentering(mth::Vec2 ratio)
{
	m_centering_ratio = ratio;
}

void Window::setViewportSize(mth::Vec2 new_size)
{
	m_window_viewport_size = new_size;
}

void Window::setViewportScaling(Window::ViewportScaling mode)
{
	m_scaling_mode = mode;
}

void Window::updateViewport()
{
	switch (m_scaling_mode)
	{
	case ViewportScaling::FIXED:
		{
			m_viewport_size = m_window_viewport_size;
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


void Window::clear(const gfx::Color& color)
{
	gfx::gl::Capability cap;
	cap.scissor_test = gfx::gl::Capability::Mode::ENABLED;
	gfx::gl::Api::getInstance()->setCapability(cap);
	gfx::gl::Api::getInstance()->setScissor(0, 0, m_size.x, m_size.y);
	RenderTarget::clear(gfx::Color(0));

	gfx::gl::Api::getInstance()->setScissor(m_viewport_pos.x, m_viewport_pos.y, m_viewport_size.x, m_viewport_size.y);
	RenderTarget::clear(color);
}


void Window::display()
{
	glfwSwapBuffers(window_ptr);
}


void* (*Window::getProcAddressLoader())(const char*)
{
    return [](const char* name) -> void* {
        return reinterpret_cast<void*>(glfwGetProcAddress(name));
    };
}

} //namespace eng::sys