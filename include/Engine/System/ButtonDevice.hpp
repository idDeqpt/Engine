#ifndef BUTTON_DEVICE_CLASS_HEADER
#define BUTTON_DEVICE_CLASS_HEADER

#include <Engine/System/InputDevice.hpp>

namespace eng::sys
{
	template<unsigned int BUTTONS_COUNT>
	class ButtonDevice : public InputDevice
	{
	public:
		enum Action
		{
			PRESSED = 1,
			RELEASED = 0
		};

		struct ButtonState
		{
			bool is_pressed = false;
			unsigned int last_change_frame = 0;
		};

		ButtonDevice();
		virtual ~ButtonDevice() = default;

		bool isPressed(int key_code);
		bool isJustPressed(int key_code);
		bool isReleased(int key_code);
		bool isJustReleased(int key_code);

		void action_handler(int key, int scancode, int action, int mode);

	protected:
		ButtonState m_buttons_states[BUTTONS_COUNT];
	};
}

#include <Engine/System/ButtonDevice.inl>

#endif //BUTTON_DEVICE_CLASS_HEADER