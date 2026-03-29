#ifndef BUTTON_DEVICE_CLASS_INLINE
#define BUTTON_DEVICE_CLASS_INLINE

#include <Engine/System/InputDevice.hpp>

namespace eng
{

template<unsigned int BUTTONS_COUNT>
sys::ButtonDevice<BUTTONS_COUNT>::ButtonDevice() : sys::InputDevice() {}


template<unsigned int BUTTONS_COUNT>
bool sys::ButtonDevice<BUTTONS_COUNT>::isPressed(int key_code)
{
	return m_buttons_states[key_code].is_pressed;
}

template<unsigned int BUTTONS_COUNT>
bool sys::ButtonDevice<BUTTONS_COUNT>::isJustPressed(int key_code)
{
	return m_buttons_states[key_code].is_pressed && (m_buttons_states[key_code].last_change_frame == m_current_frame);
}

template<unsigned int BUTTONS_COUNT>
bool sys::ButtonDevice<BUTTONS_COUNT>::isReleased(int key_code)
{
	return !m_buttons_states[key_code].is_pressed;
}

template<unsigned int BUTTONS_COUNT>
bool sys::ButtonDevice<BUTTONS_COUNT>::isJustReleased(int key_code)
{
	return !m_buttons_states[key_code].is_pressed && (m_buttons_states[key_code].last_change_frame == m_current_frame);
}

} //namespace eng

#endif //BUTTON_DEVICE_CLASS_INLINE