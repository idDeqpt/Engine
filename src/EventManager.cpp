#include <Graphics/EventManager.hpp>

#include <GLFW/glfw3.h>
#include <string.h>


bool gfx::EventManager::m_key_pressed[1032];
uint gfx::EventManager::m_key_change_frames[1032];
uint gfx::EventManager::m_current_frame;


void gfx::EventManager::key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		m_key_pressed[key] = true;
		m_key_change_frames[key] = m_current_frame;
	}
	else if (action == GLFW_RELEASE)
	{
		m_key_pressed[key] = false;
		m_key_change_frames[key] = m_current_frame;
	}
}

void gfx::EventManager::mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		m_key_pressed[GFX_MOUSE_BUTTONS_START + button] = true;
		m_key_change_frames[GFX_MOUSE_BUTTONS_START + button] = m_current_frame;
	}
	else if (action == GLFW_RELEASE)
	{
		m_key_pressed[GFX_MOUSE_BUTTONS_START + button] = false;
		m_key_change_frames[GFX_MOUSE_BUTTONS_START + button] = m_current_frame;
	}
}



void gfx::EventManager::initialize(GLFWwindow* window_ptr)
{
	m_current_frame = 0;
	memset(m_key_pressed, false, 1032);
	memset(m_key_change_frames, 0, 1032);

	glfwSetKeyCallback(window_ptr, key_callback);
	glfwSetMouseButtonCallback(window_ptr, mouse_button_callback);
}

void gfx::EventManager::pull()
{
	m_current_frame++;
	glfwPollEvents();
}


bool gfx::EventManager::isPressed(int key_code)
{
	return m_key_pressed[key_code];
}

bool gfx::EventManager::isJustPressed(int key_code)
{
	return m_key_pressed[key_code] && (m_key_change_frames[key_code] == m_current_frame);
}

bool gfx::EventManager::isReleased(int key_code)
{
	return !m_key_pressed[key_code];
}

bool gfx::EventManager::isJustReleased(int key_code)
{
	return !m_key_pressed[key_code] && (m_key_change_frames[key_code] == m_current_frame);
}