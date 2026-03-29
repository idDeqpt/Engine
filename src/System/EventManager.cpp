#include <Engine/System/EventManager.hpp>

#include <Engine/Math/Vec2.hpp>
#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>

#include <GLFW/glfw3.h>
#include <string.h>
#include <memory>


namespace eng
{

GLFWwindow* sys::EventManager::s_active_window_ptr = nullptr;
sys::Keyboard& sys::EventManager::s_keyboard = sys::Keyboard::getInstance();
sys::Mouse&    sys::EventManager::s_mouse    = sys::Mouse::getInstance();
sys::Mouse::CursorMode sys::EventManager::s_cursor_mode = sys::Mouse::CursorMode::NORMAL;


void sys::EventManager::initialize(GLFWwindow* window_ptr)
{
	s_active_window_ptr = window_ptr;

	glfwSetKeyCallback(s_active_window_ptr,         key_callback);
	glfwSetMouseButtonCallback(s_active_window_ptr, mouse_button_callback);
	glfwSetCursorPosCallback(s_active_window_ptr,   cursor_position_callback);
}

void sys::EventManager::pull()
{
	s_keyboard.nextFrame();
	s_mouse.nextFrame();

	glfwPollEvents();
}


sys::Keyboard& sys::EventManager::getKeyboard()
{
	return s_keyboard;
}

sys::Mouse& sys::EventManager::getMouse()
{
	return s_mouse;
}


void sys::EventManager::setCursorMode(Mouse::CursorMode mode)
{
	s_cursor_mode = mode;
	glfwSetInputMode(s_active_window_ptr, GLFW_CURSOR, mode);
}

sys::Mouse::CursorMode sys::EventManager::getCursorMode()
{
	return s_cursor_mode;
}



void sys::EventManager::key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode)
{
	s_keyboard.action_handler(key, scancode, action, mode);
}

void sys::EventManager::mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode)
{
	s_mouse.click_handler(button, action, mode);
}

void sys::EventManager::cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos)
{
	s_mouse.move_handler(xpos, ypos);
}

} //namespace eng