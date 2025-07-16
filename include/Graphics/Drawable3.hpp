#ifndef DRAWABLE_3_CLASS_HEADER
#define DRAWABLE_3_CLASS_HEADER

#include <Math/Transformable3.hpp>

namespace gfx
{
	class Window;

	class Drawable3 : public mth::Transformable3
	{
	public:
		Drawable3();
		Drawable3(Drawable3& parent);

		virtual void draw(Window* window) const = 0;
	};
}

#endif //DRAWABLE_3_CLASS_HEADER