#include <Graphics/Color.hpp>

#include <stdexcept>


gfx::Color::Color():
	r(0), g(0), b(0), a(0) {}

gfx::Color::Color(color_t common_value):
	r(common_value), g(common_value), b(common_value), a(common_value) {}

gfx::Color::Color(color_t red, color_t green, color_t blue):
	r(red), g(green), b(blue), a(COLOR_MAX_VALUE) {}

gfx::Color::Color(color_t red, color_t green, color_t blue, color_t alpha):
	r(red), g(green), b(blue), a(alpha) {}

gfx::Color::Color(const Color& color):
	r(color.r), g(color.g), b(color.b), a(color.a) {}