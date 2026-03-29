#include <Engine/System/Mouse.hpp>

#include <Engine/System/ButtonDevice.hpp>
#include <Engine/System/AxisDevice.hpp>
#include <Engine/Math/Vec2.hpp>

namespace eng
{

sys::Mouse::Mouse():
	m_moved(false),
	sys::ButtonDevice<8>(),
	sys::AxisDevice<3>() {}


sys::Mouse& sys::Mouse::getInstance()
{
	static Mouse instance;
	return instance;
}


bool sys::Mouse::isPressed(Mouse::Button button)
{
	return ButtonDevice::isPressed(int(button));
}

bool sys::Mouse::isJustPressed(Mouse::Button button)
{
	return ButtonDevice::isJustPressed(int(button));
}

bool sys::Mouse::isReleased(Mouse::Button button)
{
	return ButtonDevice::isReleased(int(button));
}

bool sys::Mouse::isJustReleased(Mouse::Button button)
{
	return ButtonDevice::isJustReleased(int(button));
}


bool sys::Mouse::moved()
{
	return ((m_axes_states[Mouse::Axis::X].last_change_frame == m_current_frame) || (m_axes_states[Mouse::Axis::Y].last_change_frame == m_current_frame));
}

mth::Vec2 sys::Mouse::getPosition()
{
	return mth::Vec2(m_axes_states[Mouse::Axis::X].value, m_axes_states[Mouse::Axis::Y].value);
}

mth::Vec2 sys::Mouse::getDelta()
{
	return mth::Vec2(m_axes_states[Mouse::Axis::X].delta, m_axes_states[Mouse::Axis::Y].delta);
}


bool sys::Mouse::axisChanged(Mouse::Axis axis)
{
	return AxisDevice::axisChanged(int(axis));
}

float sys::Mouse::getValue(Mouse::Axis axis)
{
	return AxisDevice::getValue(int(axis));
}

float sys::Mouse::getDelta(Mouse::Axis axis)
{
	return AxisDevice::getDelta(int(axis));
}


void sys::Mouse::click_handler(int button, int action, int mode)
{
	if (action == Action::PRESSED)
		m_buttons_states[button] = {true,  m_current_frame};
	else if (action == Action::RELEASED)
		m_buttons_states[button] = {false, m_current_frame};
}

void sys::Mouse::move_handler(int x, int y)
{
	float last_x = m_axes_states[Mouse::Axis::X].value;
	m_axes_states[Mouse::Axis::X] = {
		float(x),
		float(x - last_x),
		m_current_frame
	};

	float last_y = m_axes_states[Mouse::Axis::Y].value;
	m_axes_states[Mouse::Axis::Y] = {
		float(y),
		float(y - last_y),
		m_current_frame
	};
}

void sys::Mouse::nextFrame()
{
	InputDevice::nextFrame();
	m_moved = false;
}

} //namespace eng>