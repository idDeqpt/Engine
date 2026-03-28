#ifndef INPUT_DEVICE_CLASS_HEADER
#define INPUT_DEVICE_CLASS_HEADER

namespace eng::sys
{
	class InputDevice
	{
	public:
		InputDevice() : m_current_frame(0) {}
		virtual ~InputDevice() = default;

		void nextFrame() {m_current_frame++;}

	protected:
		unsigned int m_current_frame;
	};
}

#endif //INPUT_DEVICE_CLASS_HEADER