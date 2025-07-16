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
		color_t& r = components[0];
		color_t& g = components[1];
		color_t& b = components[2];
		color_t& a = components[3];

		Color();
		Color(color_t common_value);
		Color(color_t red, color_t green, color_t blue);
		Color(color_t red, color_t green, color_t blue, color_t alpha);
		Color(const Color& another);

		color_t* getComponentsPtr();

		color_t operator[](unsigned int index) const;
		color_t& operator[](unsigned int index);

	protected:
		color_t components[4];
	};
}

#endif //COLOR_CLASS_HEADER