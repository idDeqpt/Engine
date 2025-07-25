#ifndef EVENT_MANAGER_STATIC_CLASS_HEADER
#define EVENT_MANAGER_STATIC_CLASS_HEADER

#define GFX_MOUSE_BUTTONS_START 1024

typedef unsigned int uint;
struct GLFWwindow;

namespace gfx
{
	class EventManager
	{
	public:
		enum Mouse
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

		static void initialize(GLFWwindow* window_ptr);
		static void pull();

		static bool isPressed(int key_code);
		static bool isJustPressed(int key_code);
		static bool isReleased(int key_code);
		static bool isJustReleased(int key_code);

	protected:
		static bool m_key_pressed[1032];
		static uint m_key_change_frames[1032];
		static uint m_current_frame;

		static void key_callback(GLFWwindow* window_ptr, int key, int scancode, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window_ptr, int button, int action, int mode);
	};
}

#endif //EVENT_MANAGER_STATIC_CLASS_HEADER