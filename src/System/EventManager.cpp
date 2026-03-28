#include <Engine/System/EventManager.hpp>

#include <Engine/Math/Vec2.hpp>
#include <GLFW/glfw3.h>
#include <string.h>
#include <memory>


namespace eng
{

GLFWwindow* sys::EventManager::m_active_window_ptr;
bool sys::EventManager::m_key_pressed[10];
uint sys::EventManager::m_key_change_frames[10];
uint sys::EventManager::m_current_frame;
bool sys::EventManager::m_cursor_locked;

bool sys::EventManager::Mouse::m_moved;
mth::Vec2 sys::EventManager::Mouse::m_position;
mth::Vec2 sys::EventManager::Mouse::m_delta;

std::unique_ptr<sys::Keyboard> sys::EventManager::s_keyboard = nullptr;



void sys::EventManager::initialize(GLFWwindow* window_ptr)
{
	m_cursor_locked = false;
	Mouse::m_moved = false;
	m_current_frame = 0;
	memset(m_key_pressed, false, 10);
	memset(m_key_change_frames, 0, 10);
	m_active_window_ptr = window_ptr;

	s_keyboard = std::make_unique<Keyboard>();

	glfwSetKeyCallback(m_active_window_ptr, key_callback);
	glfwSetMouseButtonCallback(m_active_window_ptr, mouse_button_callback);
	glfwSetCursorPosCallback(m_active_window_ptr, cursor_position_callback);
}

void sys::EventManager::pull()
{
	m_current_frame++;
	Mouse::m_moved = false;
	Mouse::m_delta = mth::Vec2();
	s_keyboard->nextFrame();
	glfwPollEvents();
}


bool sys::EventManager::isPressed(int key_code)
{
	return m_key_pressed[int(key_code)];
}

bool sys::EventManager::isJustPressed(int key_code)
{
	return m_key_pressed[int(key_code)] && (m_key_change_frames[int(key_code)] == m_current_frame);
}

bool sys::EventManager::isReleased(int key_code)
{
	return !m_key_pressed[int(key_code)];
}

bool sys::EventManager::isJustReleased(int key_code)
{
	return !m_key_pressed[int(key_code)] && (m_key_change_frames[int(key_code)] == m_current_frame);
}


bool sys::EventManager::isPressed(Mouse::Button key_code)
{
	return m_key_pressed[int(key_code)];
}

bool sys::EventManager::isJustPressed(Mouse::Button key_code)
{
	return m_key_pressed[int(key_code)] && (m_key_change_frames[int(key_code)] == m_current_frame);
}

bool sys::EventManager::isReleased(Mouse::Button key_code)
{
	return !m_key_pressed[int(key_code)];
}

bool sys::EventManager::isJustReleased(Mouse::Button key_code)
{
	return !m_key_pressed[int(key_code)] && (m_key_change_frames[int(key_code)] == m_current_frame);
}


void sys::EventManager::lockCursor()
{
	m_cursor_locked = true;
	glfwSetInputMode(m_active_window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void sys::EventManager::unlockCursor()
{
	m_cursor_locked = false;
	glfwSetInputMode(m_active_window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void sys::EventManager::setCursorLock(bool state)
{
	m_cursor_locked = state;
	if (state) lockCursor();
	else     unlockCursor();
}

bool sys::EventManager::getCursorLock()
{
	return m_cursor_locked;
}


sys::Keyboard& sys::EventManager::getKeyboard()
{
	return *s_keyboard;
}



bool sys::EventManager::Mouse::moved()
{
	return m_moved;
}

mth::Vec2 sys::EventManager::Mouse::getPosition()
{
	return m_position;
}

mth::Vec2 sys::EventManager::Mouse::getDelta()
{
	return m_delta;
}



void sys::EventManager::key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode)
{
	s_keyboard->action_handler(key, scancode, action, mode);
}

void sys::EventManager::mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode)
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

void sys::EventManager::cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos){
	Mouse::m_moved = true;
	mth::Vec2 new_pos = mth::Vec2(xpos, ypos);

	Mouse::m_delta = new_pos - Mouse::m_position;
	Mouse::m_position = new_pos;
}

} //namespace eng