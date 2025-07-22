#ifndef DRAWABLE_CLASS_HEADER
#define DRAWABLE_CLASS_HEADER

namespace gfx
{
	class Window;
	class RenderStates;

	class Drawable
	{
	public:
		virtual void draw(Window* window, RenderStates& states) = 0;
	};
}

#endif //DRAWABLE_CLASS_HEADER