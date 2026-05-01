#include <Engine/System/Mouse.hpp>

#include <Engine/Core/SignalBus.hpp>
#include <Engine/System/ButtonDevice.hpp>
#include <Engine/System/AxisDevice.hpp>
#include <Engine/Math/Vec2.hpp>


namespace eng
{

sys::Mouse::Mouse(core::SignalBus& sbus):
	m_moved(false),
	m_sbus(sbus),
	sys::ButtonDevice<8>(),
	sys::AxisDevice<3>() {}


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
	{
		m_buttons_states[button] = {true,  m_current_frame};
		m_sbus.emit<Mouse::Button>("mouse_clicked", Mouse::Button(button));
		if (isJustPressed(Mouse::Button(button)))
			m_sbus.emit<Mouse::Button>("mouse_just_clicked", Mouse::Button(button));
	}
	else if (action == Action::RELEASED)
	{
		m_buttons_states[button] = {false, m_current_frame};
		m_sbus.emit<Mouse::Button>("mouse_pressed", Mouse::Button(button));
		if (isJustReleased(Mouse::Button(button)))
			m_sbus.emit<Mouse::Button>("mouse_just_pressed", Mouse::Button(button));
	}
}

void sys::Mouse::move_handler(int x, int y)
{
	float last_x = m_axes_states[Mouse::Axis::X].value;
	float last_y = m_axes_states[Mouse::Axis::Y].value;

	mth::Vec2 pos(x, y);
	mth::Vec2 delta(x - last_x, y - last_y);

	m_axes_states[Mouse::Axis::X] = {
		pos.x,
		delta.x,
		m_current_frame
	};

	m_axes_states[Mouse::Axis::Y] = {
		pos.y,
		delta.y,
		m_current_frame
	};

	m_sbus.emit<const mth::Vec2&, const mth::Vec2&>("mouse_moved", pos, delta);
}

void sys::Mouse::nextFrame()
{
	InputDevice::nextFrame();
	m_moved = false;
}

} //namespace eng>