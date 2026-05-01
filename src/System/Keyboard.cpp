#include <Engine/System/Keyboard.hpp>

#include <Engine/Core/SignalBus.hpp>
#include <Engine/System/ButtonDevice.hpp>


namespace eng
{

sys::Keyboard::Keyboard(core::SignalBus& sbus):
	m_sbus(sbus),
	sys::ButtonDevice<348>() {}


bool sys::Keyboard::isPressed(Keyboard::Key key)
{
	return ButtonDevice::isPressed(int(key));
}

bool sys::Keyboard::isJustPressed(Keyboard::Key key)
{
	return ButtonDevice::isJustPressed(int(key));
}

bool sys::Keyboard::isReleased(Keyboard::Key key)
{
	return ButtonDevice::isReleased(int(key));
}

bool sys::Keyboard::isJustReleased(Keyboard::Key key)
{
	return ButtonDevice::isJustReleased(int(key));
}


void sys::Keyboard::action_handler(int key, int scancode, int action, int mode)
{
	if (action == Action::PRESSED)
	{
		m_buttons_states[key] = {true,  m_current_frame};
		m_sbus.emit<Keyboard::Key>("keyboard_pressed", Keyboard::Key(key));
		if (isJustPressed(Keyboard::Key(key)))
			m_sbus.emit<Keyboard::Key>("keyboard_just_pressed", Keyboard::Key(key));
	}
	else if (action == Action::RELEASED)
	{
		m_buttons_states[key] = {false, m_current_frame};
		m_sbus.emit<Keyboard::Key>("keyboard_released", Keyboard::Key(key));
		if (isJustReleased(Keyboard::Key(key)))
			m_sbus.emit<Keyboard::Key>("keyboard_just_released", Keyboard::Key(key));
	}
}

} //namespace eng>