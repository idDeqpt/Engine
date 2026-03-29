#ifndef AXIS_DEVICE_CLASS_HEADER
#define AXIS_DEVICE_CLASS_HEADER

#include <Engine/System/InputDevice.hpp>

namespace eng::sys
{
	template<unsigned int AXES_COUNT>
	class AxisDevice : public virtual InputDevice
	{
	public:
		struct AxisState
		{
			float value = 0;
			float delta = 0;
			unsigned int last_change_frame = 0;
		};

		AxisDevice();
		virtual ~AxisDevice() = default;

		bool axisChanged(unsigned int axis);
		float getValue(unsigned int axis);
		float getDelta(unsigned int axis);

	protected:
		AxisState m_axes_states[AXES_COUNT];
	};
}

#include <Engine/System/AxisDevice.inl>

#endif //AXIS_DEVICE_CLASS_HEADER