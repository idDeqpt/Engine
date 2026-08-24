#include <Engine/System/Window.hpp>

#include <Engine/Graphics/GL/Api.hpp>
#include <Engine/Math/Vec2.hpp>
#include <Engine/Core/Logger.hpp>

#include <algorithm>
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
	setViewportSize(m_size);
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


bool sys::Window::isOpen() const
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

void sys::Window::setViewportSize(const mth::Vec2& new_size)
{
	m_window_viewport_size = new_size;
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


mth::Vec2 sys::Window::getViewportSize() const
{
	return m_viewport_size;
}


void sys::Window::clear(const gfx::Color& color)
{
	gfx::gl::Api::getInstance()->enable(gfx::gl::Capability::SCISSOR_TEST);
	gfx::gl::Api::getInstance()->setScissor(0, 0, m_size.x, m_size.y);
	RenderTarget::clear(gfx::Color(0));

	gfx::gl::Api::getInstance()->setScissor(m_viewport_pos.x, m_viewport_pos.y, m_viewport_size.x, m_viewport_size.y);
	RenderTarget::clear(color);
	gfx::gl::Api::getInstance()->disable(gfx::gl::Capability::SCISSOR_TEST);
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