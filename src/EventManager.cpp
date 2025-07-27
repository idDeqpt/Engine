#include <Graphics/EventManager.hpp>

#include <GLFW/glfw3.h>
#include <string.h>
#include <Math/Vec2.hpp>


GLFWwindow* gfx::EventManager::m_active_window_ptr;
bool gfx::EventManager::m_key_pressed[1032];
uint gfx::EventManager::m_key_change_frames[1032];
uint gfx::EventManager::m_current_frame;
bool gfx::EventManager::m_cursor_locked;

bool gfx::EventManager::Mouse::m_moved;
mth::Vec2 gfx::EventManager::Mouse::m_position;
mth::Vec2 gfx::EventManager::Mouse::m_delta;


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

void gfx::EventManager::cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos){
	Mouse::m_moved = true;
	mth::Vec2 new_pos = mth::Vec2(xpos, ypos);

	Mouse::m_delta = new_pos - Mouse::m_position;
	Mouse::m_position = new_pos;
}



void gfx::EventManager::initialize(GLFWwindow* window_ptr)
{
	m_cursor_locked = false;
	Mouse::m_moved = false;
	m_current_frame = 0;
	memset(m_key_pressed, false, 1032);
	memset(m_key_change_frames, 0, 1032);
	m_active_window_ptr = window_ptr;

	glfwSetKeyCallback(m_active_window_ptr, key_callback);
	glfwSetMouseButtonCallback(m_active_window_ptr, mouse_button_callback);
	glfwSetCursorPosCallback(m_active_window_ptr, cursor_position_callback);
}

void gfx::EventManager::pull()
{
	m_current_frame++;
	Mouse::m_moved = false;
	Mouse::m_delta = mth::Vec2();
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


void gfx::EventManager::lockCursor()
{
	m_cursor_locked = true;
	glfwSetInputMode(m_active_window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void gfx::EventManager::unlockCursor()
{
	m_cursor_locked = false;
	glfwSetInputMode(m_active_window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void gfx::EventManager::setCursorLock(bool state)
{
	m_cursor_locked = state;
	if (state) lockCursor();
	else     unlockCursor();
}

bool gfx::EventManager::getCursorLock()
{
	return m_cursor_locked;
}



bool gfx::EventManager::Mouse::moved()
{
	return m_moved;
}

mth::Vec2 gfx::EventManager::Mouse::getPosition()
{
	return m_position;
}

mth::Vec2 gfx::EventManager::Mouse::getDelta()
{
	return m_delta;
}