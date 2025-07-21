#ifndef COLOR_CLASS_HEADER
#define COLOR_CLASS_HEADER

#define COLOR_MIN_VALUE 0
#define COLOR_MAX_VALUE 255

typedef unsigned char color_t;

namespace gfx
{
	class Color
	{
	public:
		color_t r, g, b, a;

		Color();
		Color(color_t common_value);
		Color(color_t red, color_t green, color_t blue);
		Color(color_t red, color_t green, color_t blue, color_t alpha);
		Color(const Color& color);
	};
}

#endif //COLOR_CLASS_HEADER