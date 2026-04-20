#include <Engine/System/EventManager.hpp>

#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>
#include <Engine/System/Window.hpp>

#include <GLFW/glfw3.h>


namespace eng
{

sys::EventManager::EventManager():
	m_keyboard(   sys::Keyboard::getInstance()),
	m_mouse(      sys::Mouse::getInstance()),
	m_cursor_mode(sys::Mouse::CursorMode::NORMAL),
	m_active_window(nullptr) {};

void sys::EventManager::setActiveWindow(sys::Window& window)
{
	m_active_window = &window;

    GLFWwindow* handle = window.getHandler();
    glfwSetWindowUserPointer(handle, this);

	glfwSetKeyCallback(        handle, key_callback);
	glfwSetMouseButtonCallback(handle, mouse_button_callback);
	glfwSetCursorPosCallback(  handle, cursor_position_callback);
}

void sys::EventManager::pull()
{
	m_keyboard.nextFrame();
	m_mouse.nextFrame();

	glfwPollEvents();
}


sys::Keyboard& sys::EventManager::getKeyboard()
{
	return m_keyboard;
}

sys::Mouse& sys::EventManager::getMouse()
{
	return m_mouse;
}


void sys::EventManager::setCursorMode(Mouse::CursorMode mode)
{
	m_cursor_mode = mode;
	glfwSetInputMode(m_active_window->getHandler(), GLFW_CURSOR, mode);
}

sys::Mouse::CursorMode sys::EventManager::getCursorMode()
{
	return m_cursor_mode;
}



void sys::EventManager::key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode)
{
    EventManager* self = static_cast<EventManager*>(glfwGetWindowUserPointer(window_ptr));
    if (self)
        self->m_keyboard.action_handler(key, scancode, action, mode);
}

void sys::EventManager::mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode)
{
    EventManager* self = static_cast<EventManager*>(glfwGetWindowUserPointer(window_ptr));
    if (self)
        self->m_mouse.click_handler(button, action, mode);
}

void sys::EventManager::cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos)
{
    EventManager* self = static_cast<EventManager*>(glfwGetWindowUserPointer(window_ptr));
    if (self)
        self->m_mouse.move_handler(xpos, ypos);
}

} //namespace eng