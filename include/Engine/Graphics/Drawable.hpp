#ifndef DRAWABLE_CLASS_HEADER
#define DRAWABLE_CLASS_HEADER

namespace gfx
{
	class RenderTarget;
	class RenderStates;

	class Drawable
	{
	public:
		virtual void draw(RenderTarget* target, RenderStates& states) = 0;
	};
}

#endif //DRAWABLE_CLASS_HEADER