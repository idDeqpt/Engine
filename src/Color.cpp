#include <Graphics/Color.hpp>

#include <stdexcept>


gfx::Color::Color()
	: components{0, 0, 0, 0} {}

gfx::Color::Color(color_t common_value)
	: components{common_value, common_value, common_value, common_value} {}

gfx::Color::Color(color_t red, color_t green, color_t blue)
	: components{red, green, blue, COLOR_MAX_VALUE} {}

gfx::Color::Color(color_t red, color_t green, color_t blue, color_t alpha)
	: components{red, green, blue, alpha} {}

gfx::Color::Color(const Color& another)
{
	for (unsigned int i = 0; i < 4; i++)
		components[i] = another[i];
}


color_t* gfx::Color::getComponentsPtr()
{
	return components;
}


color_t gfx::Color::operator[](unsigned int index) const
{
	if (index > 3)
		throw std::out_of_range("Color < 4");
	return components[index];
}

color_t& gfx::Color::operator[](unsigned int index)
{
	if (index > 3)
		throw std::out_of_range("Color < 4");
	return components[index];
}