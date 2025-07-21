#ifndef DRAWABLE_CLASS_HEADER
#define DRAWABLE_CLASS_HEADER

#include <Math/Transformable3.hpp>

namespace gfx
{
	class Window;

	class Drawable : public mth::Transformable3
	{
	public:
		Drawable();
		Drawable(Drawable& parent);

		virtual void draw(Window* window) const = 0;
	};
}

#endif //DRAWABLE_CLASS_HEADER