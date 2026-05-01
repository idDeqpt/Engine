#ifndef EVENT_MANAGER_CLASS_HEADER
#define EVENT_MANAGER_CLASS_HEADER

#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>


struct GLFWwindow;

namespace eng::core
{
	class SignalBus;
}

namespace eng::sys
{
	class Window;

	class EventManager
	{
	public:
		EventManager(core::SignalBus& sbus);

		void setActiveWindow(Window& window);
		void pull();

		Keyboard& getKeyboard();
		Mouse&    getMouse();

		void setCursorMode(Mouse::CursorMode mode);
		Mouse::CursorMode getCursorMode();

	protected:
		Window*  m_active_window;
		Keyboard m_keyboard;
		Mouse    m_mouse;
		Mouse::CursorMode m_cursor_mode;

		static void key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode);
		static void cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos);
	};
}

#endif //EVENT_MANAGER_CLASS_HEADER