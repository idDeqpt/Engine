#ifndef MOUSE_CLASS_HEADER
#define MOUSE_CLASS_HEADER

#include <Engine/System/ButtonDevice.hpp>
#include <Engine/System/AxisDevice.hpp>
#include <Engine/Math/Vec2.hpp>

namespace eng::sys
{
	class Mouse : public ButtonDevice<8>, public AxisDevice<3>
	{
	public:
		enum Button
		{
			BUTTON_1 = 0,
			BUTTON_2 = 1,
			BUTTON_3 = 2,
			BUTTON_4 = 3,
			BUTTON_5 = 4,
			BUTTON_6 = 5,
			BUTTON_7 = 6,
			BUTTON_8 = 7,
			LEFT   = BUTTON_1,
			RIGHT  = BUTTON_2,
			MIDDLE = BUTTON_3
		};

		enum Axis
		{
			X,
			Y,
			WHEEL
		};

		enum CursorMode
		{
			NORMAL   = 0x00034001,
			DISABLE  = 0x00034003,
			CAPTURED = 0x00034004,
			HIDDEN   = 0x00034002
		};

		static Mouse& getInstance();

		bool isPressed(Mouse::Button button);
		bool isJustPressed(Mouse::Button button);
		bool isReleased(Mouse::Button button);
		bool isJustReleased(Mouse::Button button);

		bool moved();
		mth::Vec2 getPosition();
		mth::Vec2 getDelta();

		bool axisChanged(Mouse::Axis axis);
		float getValue(Mouse::Axis axis);
		float getDelta(Mouse::Axis axis);

		void click_handler(int button, int action, int mode);
		void move_handler(int x, int y);

		void nextFrame();

	protected:
		bool m_moved;

	private:
		Mouse();
	};
}

#endif //MOUSE_CLASS_HEADER