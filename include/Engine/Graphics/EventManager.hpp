#ifndef EVENT_MANAGER_STATIC_CLASS_HEADER
#define EVENT_MANAGER_STATIC_CLASS_HEADER

#include <Engine/Math/Vec2.hpp>

#define GFX_MOUSE_BUTTONS_START 1024

typedef unsigned int uint;
struct GLFWwindow;

namespace gfx
{
	class EventManager
	{
	public:
		class Mouse
		{
		public:
			enum Button
			{
				BUTTON_1 = GFX_MOUSE_BUTTONS_START,
				BUTTON_2 = GFX_MOUSE_BUTTONS_START + 1,
				BUTTON_3 = GFX_MOUSE_BUTTONS_START + 2,
				BUTTON_4 = GFX_MOUSE_BUTTONS_START + 3,
				BUTTON_5 = GFX_MOUSE_BUTTONS_START + 4,
				BUTTON_6 = GFX_MOUSE_BUTTONS_START + 5,
				BUTTON_7 = GFX_MOUSE_BUTTONS_START + 6,
				BUTTON_8 = GFX_MOUSE_BUTTONS_START + 7,
				LEFT = BUTTON_1,
				RIGTH = BUTTON_2,
				MIDDLE = BUTTON_3
			};

			static bool moved();
			static mth::Vec2 getPosition();
			static mth::Vec2 getDelta();

			friend class EventManager;

		protected:
			static bool m_moved;
			static bool m_locked;
			static mth::Vec2 m_position;
			static mth::Vec2 m_delta;
		};

		static void initialize(GLFWwindow* window_ptr);
		static void pull();

		static bool isPressed(int key_code);
		static bool isJustPressed(int key_code);
		static bool isReleased(int key_code);
		static bool isJustReleased(int key_code);

		static void lockCursor();
		static void unlockCursor();
		static void setCursorLock(bool state);
		static bool getCursorLock();

	protected:
		static GLFWwindow* m_active_window_ptr;
		static bool m_key_pressed[1032];
		static uint m_key_change_frames[1032];
		static uint m_current_frame;
		static bool m_cursor_locked;

		static void key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode);
		static void cursor_position_callback(GLFWwindow* window_ptr, double xpos, double ypos);
	};
}

#endif //EVENT_MANAGER_STATIC_CLASS_HEADER