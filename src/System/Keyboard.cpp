#include <Engine/System/Keyboard.hpp>

#include <Engine/System/ButtonDevice.hpp>

namespace eng
{

sys::Keyboard::Keyboard() : sys::ButtonDevice<348>() {}


sys::Keyboard& sys::Keyboard::getInstance()
{
	static Keyboard instance;
	return instance;
}


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
		m_buttons_states[key] = {true,  m_current_frame};
	else if (action == Action::RELEASED)
		m_buttons_states[key] = {false, m_current_frame};
}

} //namespace eng>