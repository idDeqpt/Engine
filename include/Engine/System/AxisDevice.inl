#ifndef AXIS_DEVICE_CLASS_INLINE
#define AXIS_DEVICE_CLASS_INLINE

#include <Engine/System/InputDevice.hpp>

namespace eng
{

template<unsigned int AXES_COUNT>
sys::AxisDevice<AXES_COUNT>::AxisDevice() : sys::InputDevice() {}


template<unsigned int AXES_COUNT>
bool sys::AxisDevice<AXES_COUNT>::axisChanged(unsigned int axis)
{
	return (m_axes_states[axis].last_change_frame == m_current_frame);
}

template<unsigned int AXES_COUNT>
float sys::AxisDevice<AXES_COUNT>::getValue(unsigned int axis)
{
	return m_axes_states[axis].value;
}

template<unsigned int AXES_COUNT>
float sys::AxisDevice<AXES_COUNT>::getDelta(unsigned int axis)
{
	return m_axes_states[axis].delta;
}

} //namespace eng

#endif //AXIS_DEVICE_CLASS_INLINE