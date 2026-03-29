#ifndef EVENT_MANAGER_STATIC_CLASS_HEADER
#define EVENT_MANAGER_STATIC_CLASS_HEADER

#include <Engine/System/Keyboard.hpp>
#include <Engine/System/Mouse.hpp>

#include <memory>


struct GLFWwindow;

namespace eng::sys
{
	class EventManager
	{
	public:
		static void initialize(GLFWwindow* window_ptr);
		static void pull();

		static Keyboard& getKeyboard();
		static Mouse&    getMouse();

		static void setCursorMode(Mouse::CursorMode mode);
		static Mouse::CursorMode getCursorMode();

	protected:
		static GLFWwindow* s_active_window_ptr;
		static Keyboard&   s_keyboard;
		static Mouse&      s_mouse;
		static Mouse::CursorMode s_cursor_mode;

		static void key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode);
		static void cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos);
	};
}

#endif //EVENT_MANAGER_STATIC_CLASS_HEADER