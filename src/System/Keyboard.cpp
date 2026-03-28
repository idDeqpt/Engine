#include <Engine/System/Keyboard.hpp>

#include <Engine/System/ButtonDevice.hpp>

namespace eng
{

sys::Keyboard::Keyboard() : sys::ButtonDevice<348>() {}


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

} //namespace eng>